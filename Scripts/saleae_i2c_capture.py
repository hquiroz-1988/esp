#!/usr/bin/env python3
"""Capture I2C traffic from Saleae Logic 2 using the automation API.

This script can launch Logic 2 from an AppImage (or connect to an already
running instance), run a timed capture, add the I2C analyzer, and export:
- A .sal capture file
- A decoded I2C CSV table
- Optional raw digital CSV files
"""

from __future__ import annotations

import argparse
import datetime as dt
import os
import pathlib
import sys

from saleae import automation


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Timed I2C capture with Saleae Logic 2")
    parser.add_argument(
        "--logic-appimage",
        default="/home/blob/Applications/Logic-2.4.44-linux-x64.AppImage",
        help="Path to Logic 2 AppImage used when launching Logic 2.",
    )
    parser.add_argument(
        "--host",
        default="127.0.0.1",
        help="Automation host for Logic 2 manager connection.",
    )
    parser.add_argument(
        "--port",
        type=int,
        default=10430,
        help="Automation port for Logic 2 manager connection.",
    )
    parser.add_argument(
        "--duration",
        type=float,
        default=5.0,
        help="Capture duration in seconds.",
    )
    parser.add_argument(
        "--scl",
        type=int,
        default=0,
        help="Digital channel index carrying I2C SCL.",
    )
    parser.add_argument(
        "--sda",
        type=int,
        default=1,
        help="Digital channel index carrying I2C SDA.",
    )
    parser.add_argument(
        "--sample-rate",
        type=int,
        default=10_000_000,
        help="Digital sample rate in samples/second.",
    )
    parser.add_argument(
        "--device-id",
        default=None,
        help="Optional Logic device ID (leave empty to auto-select).",
    )
    parser.add_argument(
        "--output-dir",
        default="/home/blob/Documents/repos/esp/captures",
        help="Directory where capture artifacts are saved.",
    )
    parser.add_argument(
        "--name",
        default="i2c_capture",
        help="Capture name prefix.",
    )
    parser.add_argument(
        "--skip-raw",
        action="store_true",
        help="Do not export raw digital CSV files.",
    )
    return parser.parse_args()


def connect_or_launch(args: argparse.Namespace) -> automation.Manager:
    try:
        print(f"Connecting to Logic 2 automation at {args.host}:{args.port}...")
        return automation.Manager.connect(address=args.host, port=args.port)
    except Exception:
        logic_path = pathlib.Path(args.logic_appimage)
        if not logic_path.exists():
            raise FileNotFoundError(
                f"Logic AppImage not found at {logic_path}. Provide --logic-appimage."
            )
        print(f"Launching Logic 2 from {logic_path} on port {args.port}...")
        return automation.Manager.launch(application_path=str(logic_path), port=args.port)


def main() -> int:
    args = parse_args()

    channels = sorted({args.scl, args.sda})
    ts = dt.datetime.now().strftime("%Y%m%d_%H%M%S")
    capture_name = f"{args.name}_{ts}"
    out_dir = pathlib.Path(args.output_dir) / capture_name
    out_dir.mkdir(parents=True, exist_ok=True)

    manager = None
    capture = None

    try:
        manager = connect_or_launch(args)
        device_config = automation.LogicDeviceConfiguration(
            enabled_digital_channels=channels,
            digital_sample_rate=args.sample_rate,
        )
        capture_config = automation.CaptureConfiguration(
            capture_mode=automation.TimedCaptureMode(args.duration)
        )

        print(
            f"Starting capture for {args.duration:.2f}s on channels {channels} "
            f"(SCL={args.scl}, SDA={args.sda})"
        )
        capture = manager.start_capture(
            device_id=args.device_id,
            device_configuration=device_config,
            capture_configuration=capture_config,
        )

        i2c = capture.add_analyzer(
            "I2C",
            label="I2C",
            settings={"SCL": args.scl, "SDA": args.sda},
        )

        capture.wait()

        sal_path = out_dir / f"{capture_name}.sal"
        csv_path = out_dir / f"{capture_name}_i2c.csv"

        capture.save_capture(str(sal_path))
        capture.export_data_table(str(csv_path), analyzers=[i2c])

        if not args.skip_raw:
            capture.export_raw_data_csv(str(out_dir), digital_channels=channels)

        print("Capture complete.")
        print(f"Saved .sal: {sal_path}")
        print(f"Saved I2C CSV: {csv_path}")
        if not args.skip_raw:
            print(f"Saved raw CSV files in: {out_dir}")
        return 0

    except Exception as exc:
        print("Capture failed:", exc, file=sys.stderr)
        return 1

    finally:
        if capture is not None:
            try:
                capture.close()
            except Exception:
                pass
        if manager is not None:
            try:
                manager.close()
            except Exception:
                pass


if __name__ == "__main__":
    raise SystemExit(main())
