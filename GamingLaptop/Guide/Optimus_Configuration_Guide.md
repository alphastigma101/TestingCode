# Nvidia Optimus:

* **DRM KERNEL LOADING**
```
DRM kernel mode setting

To enable DRM (Direct Rendering Manager) kernel mode setting, set modeset=1 and fbdev=1 kernel module parameters for the nvidia_drm module. The latter is required to tell the nvidia driver to provide its own framebuffer device instead of relying on efifb or vesafb, which don't work under simpledrm. For nvidia driver version < 545, the nvidia_drm.modeset=1 option must be set through kernel parameters, in order to disable simpledrm [1] (for more information, refer to FS#73720).
```
* **EARLY LOADING**:
```
Early loading

For basic functionality, just adding the kernel parameter should suffice. If you want to ensure it is loaded at the earliest possible occasion, or are noticing startup issues (such as the nvidia kernel module being loaded after the display manager) you can add nvidia, nvidia_modeset, nvidia_uvm and nvidia_drm to the initramfs.
mkinitcpio

If you use mkinitcpio initramfs, follow mkinitcpio#MODULES to add modules.

If added to the initramfs, do not forget to run mkinitcpio every time there is a nvidia driver update. See #pacman hook to automate these steps. 
```
* **VULKAN DRIVERS**:
- For archlinux, you can install *vulkan-tools* will be differnt for each linux distro
* **Hardware accelerated video decoding**:



- **SOURCES**:
    - **BREIF DESCRIPTION GOES HERE**
        - *https://wiki.archlinux.org/title/Hardware_video_acceleration*



- **SOURCES**:
    - **BREIF DESCRIPTION GOES HERE**
        - *https://wiki.archlinux.org/title/NVIDIA*

