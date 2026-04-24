# Saleae I2C Capture Script Quick Guide

This note explains how to run the automation capture script:
- Script: `Scripts/saleae_i2c_capture.py`
- Output folder: `captures/`

## Prerequisites

1. Logic 2 AppImage exists at:
   `/home/blob/Downloads/Logic-2.4.44-linux-x64.AppImage`
2. Python package is installed:
   `logic2-automation`
3. Saleae udev rules are installed:
   `/etc/udev/rules.d/99-SaleaeLogic.rules`

## Start Logic 2 Automation Server

Run this in a terminal if Logic 2 is not already running:

```bash
/home/blob/Downloads/Logic-2.4.44-linux-x64.AppImage --automation
```

## Basic Capture Command

From repo root:

```bash
python3 Scripts/saleae_i2c_capture.py --duration 5 --scl 0 --sda 1 --name i2c_run
```

This records for 5 seconds and saves:
- `.sal` capture file
- decoded I2C CSV

## Common Options

```bash
python3 Scripts/saleae_i2c_capture.py \
  --duration 10 \
  --scl 0 \
  --sda 1 \
  --sample-rate 10000000 \
  --name meter_poll \
  --output-dir /home/blob/Documents/repos/esp/captures
```

Options:
- `--duration`: capture length in seconds
- `--scl`: SCL digital channel index
- `--sda`: SDA digital channel index
- `--sample-rate`: digital sample rate (default 10000000)
- `--skip-raw`: skip raw digital CSV export

## Where Files Go

Files are written to:

`captures/<name>_<timestamp>/`

Example:
- `captures/i2c_run_20260422_233246/i2c_run_20260422_233246.sal`
- `captures/i2c_run_20260422_233246/i2c_run_20260422_233246_i2c.csv`

## Troubleshooting

- If you see connection errors, start Logic 2 with `--automation` first.
- If sample rate errors appear, use one of the supported rates for your hardware (for Logic 8, 10000000 is known-good).
- If USB permission errors appear, replug the device and reload udev rules:

```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```
