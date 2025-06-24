# App Note: Common Installation Issues (WSL & MSYS)

## WSL (Windows Subsystem for Linux)

- **Missing Linux Kernel Update**  
    Ensure Windows is up to date and install the latest WSL kernel from [Microsoft](https://aka.ms/wsl2kernel).

- **WSL Not Enabled**  
    Enable WSL and Virtual Machine Platform via PowerShell:
    ```powershell
    dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart
    dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
    ```

- **Distribution Not Found**  
    Install a Linux distribution from the Microsoft Store.

- **Network Issues**  
    Restart WSL with `wsl --shutdown` and check firewall settings.

---

## MSYS

- **Path Conflicts**  
    Ensure MSYS is not conflicting with other Unix-like tools in your system `PATH`.

- **Missing Packages**  
    Use `pacman -Syu` to update and install required packages.

- **Permission Errors**  
    Run MSYS as administrator if you encounter permission issues.

- **Corrupted Installation**  
    Reinstall MSYS if persistent errors occur.


### Issues Compiling <a name="bullet2.2"></a>

Some issues encountered while compiling are listed below, Windows Toolchain Only! (Mac and Linux installs dont have to deal with MSYS)

##### Python Version <a name="bullet2.2.1"></a>

While compiling within the mingw32 environment ensure that the correct python and pip versions will be invoked. The invoked versions should be within the ```mingw32/bin``` folder.

![alt text](/Documents/Images/whichPythonScreenshot.png)


##### Xtensa Compiler <a name="bullet2.2.2"></a>

If you are getting an issue when calling ```make menuconfig``` and receive the following error message.

```make: xtensa-lx106-elf-gcc: Command not found```

![alt text](/Documents/Images/xtensa_not_found.png)

Then follow instructions below:

After downloading the ```xtensa-lx106-elf``` ```.zip``` or ```.tar.gz```  as instructed in the [Windows Toolchain Setup](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/windows-setup.html#download-the-toolchain-for-the-esp8266), extract the ```xtensa-lx106-elf``` folder into your ```msys32/opt folder```

![alt text](/Documents/Images/extract.png)
 
 Your ```msys32/opt``` folder should now look like below.

![alt text](/Documents/Images/opt.png)

Next follow steps 2 & 3  in the [Linux Toolchain Setup](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/linux-setup.html#toolchain-setup) and ```make``` should now be able to invoke ```xtensa-lx106-elf-gcc```.

---

**Tip:** Always consult the official documentation for troubleshooting steps.