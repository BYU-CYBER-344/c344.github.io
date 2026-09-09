---
title: "Lab 1: Hypervisors and Operating Systems"
---

>These are preliminary instructions. In this lab you will install two operating systems on virtual machines. One will be a version of Windows that you choose. The other will be Arch Linux. Your submission will be a writeup answering specific questions about each installation.
>These preliminary instructions tell how to do the installations which is enough to get you started. The balance of the instructions will be added by end-of-day on Tuesday, September 8.

Contemporary operating systems share many similarities and a few important differences. Linux, macOS, Android, and BSD all share a common [Unix](https://en.wikipedia.org/wiki/Unix) heritage. Linux, in turn, has numerous distributions. [See this interactive graphic](https://rreinold.github.io/explore-linux/) for one visualization of the of the Linux family tree. [Linux](https://www.linux.org/) is an open-source operating system Kernel. Most distributions combine it with [GNU](https://www.gnu.org) open source tools [GNU Coreutils](https://www.gnu.org/software/coreutils) and [Bash](https://www.gnu.org/software/bash). It also relies on the excellent [GNU C/C++](https://gcc.gnu.org) compiler and associated libraries.

[macOS](https://en.wikipedia.org/wiki/MacOS) is built and managed by Apple for their Mac computers. The kernel is [Darwin](https://en.wikipedia.org/wiki/Darwin_(operating_system)) which is derived from [Mach](https://en.wikipedia.org/wiki/Mach_(kernel)), another Unix-like operating system. While Darwin is open source due to being derived from prior open source projects, Apple layers many proprietary features into macOS. Due to the close binding between macOS and Apple hardware, it is difficult (but [not impossible](https://www.digitalcitizen.life/how-to-use-macos-on-a-windows-pc-using-a-virtual-machine/)) to load macOS into a virtual machine.

Microsoft Windows is the most prominent alternative to Unix-derived operating systems. Unlike the mostly open source Linux world, but more like macOS, Windows is entirely proprietary. However, in recent years, Microsoft has begun embracing open source software development platforms, hosting Linux distros in their Azure cloud, and creating the Windows Subsystem for Linux. But they have shown no interest in open sourcing their operating systems.

In Homework 1 you chose a Hypervisor platform and installed Ubuntu Linux. In this lab you will install two more operating systems on virtual machines and explore their similarities and differences.

## Installing Windows

For this step you will choose a version of Windows to install. If you already run Windows on your personal computer then you may want to choose a different version to get a feel for other versions are like. If you are principally on macOS, you might choose a contemporary version of Windows to get a feel for what some of your peers run.


Even though Windows 10 is in limited support and earlier versions are out of support, there are still official download sites for the following versions of windows. Regardless of version, you will need a .ISO file for installation. Generally, that means downloading and running the Microsoft Media Creation tool. If you are running macOS, you may have to run the media creation tool on a lab computer and then transfer the .ISO to your host.

Tips:
* Recent versions of Windows make you create a Microsoft account during installation. If you leave your VM disconnected from the internet during installation you can usually bypass that requirement. Look online for the latest tips related to installing Windows without a Microsoft account.
* You do not need to activate your Windows installation. You will just run in non-activated mode.

Here are some options:
* Windows 11: [Download Windows 11](https://www.microsoft.com/en-us/software-download/windows11) Choose the ISO for x64 devices.
* Windows Server 2025: [Windows Server 2025 Evaluation Center](https://www.microsoft.com/en-us/evalcenter/download-windows-server-2025) Choose the 64-bit ISO.
* Windows 10: [Windows 10 Media Creation Tool](https://www.microsoft.com/en-us/software-download/windows10) Even though it says you need a license, you can simply download and the media creation tool and use it to build an ISO file.
* Windows Server 2022: [Windows Server 2022 Evaluation Center](https://www.microsoft.com/en-us/evalcenter/download-windows-server-2022) Choose the 64-bit ISO.
* Other Out-of-Support Windows Versions: Check the [Internet Archive](https://archive.org).

### Steps
1. Download a Windows installer from one of the above choices.
2. Install Windows on a VM using the hypervisor you chose in Homework 1
3. During the installation, pick options that make sense to you.

Explore the features of the version of Windows you installed.

### Record Your Success
* *In the Edge web browser included with your new Windows installation* open LearningSuite and log in.
* Under **Assignments** or in the **Schedule** open Lab 1.
* Click `Record an OS for Lab-1`
* Enter the name of the Windows operating system you installed
* Click `Submit`

> **Important** You must record using a web browser in the operating system you just installed. We will use the User-Agent string and IP address to validate that you are running under a new VM. You will repeat this with the Arch Linux OS and will have at least two records in your report.
> From any web browser you can select `View my Lab-1 Report` to see all of the OS records you have reported.

## Installing Arch Linux

Arch Linux is a "lightweight and flexible Linux distribution that tries to Keep It Simple."
Even their [official website](https://archlinux.org/) is bare bones.
You need to already know what's going on to understand what's happening there.
Here's a primer:

* The [Downloads Page](https://archlinux.org/download/) is where you will find Arch installers. Unless you already use BitTorrent, go to the HTTP Direct Downloads area and choose one of the mirrors as your download source.
* Arch publishes new ISO snapshots every month. They use dates, not version numbers. So installing Arch gets you the latest, tested, Linux kernel and tools.
* Once you have installed Arch, you can update to the latest with this command: `pacman -Syu`
* The initial Arch installation is a baseline Linux kernel with a command-line terminal. To that base, Arch adds three repositories of packages, Core, Extra, and Community. These packages are installed using Arch `pacman` which fills a similar role to `apt` on the **Debian** family of Linux distro (including **Ubuntu**).
* The [repositories](https://archlinux.org/packages/) include more than 15,000 packages. Some popular examples are web servers (Apache (httpd), Nginx, Node.js, etc.), GUI shells (GNOME, KDE Plasma, etc.), software development tools, network utilities, games, and more.

Browse to the [Arch Download](https://archlinux.org/download/) page, choose a mirror, and download the Arch installation .iso.

### Steps
1. Create your virtual machine and set the Arch .iso as your boot drive. If your hypervisor has a secure boot option (e.g. Hyper-V) turn that off.
2. Boot the VM. It will load Arch Linux from the .iso file and take you to a command line. At this point, you are just at the installation console. You have not yet installed Arch.
4. Type `archinstall` to launch the interactive installer. Pick options that make sense to you. Here are some recommendations:
    * Archinstall Language: English
    * Locales: (leave the defaults: English, US, and UTF-8)
    * Mirrors and repositories: (Leave blank)
    * Disk configuration: Partitioning
        * Use a best-effort default partition layout
        * ext4 for the main filesystem
        * A separate partition for /home: No
        * LVM: (No setting)
        * Disk encryption: (No setting)
    * Swap: (Leave the default Swap on zram with zstd for the compression algorithm)
    * Bootloader:
        * Bootloader: Systemd-boot (If you have trouble booting, consider using *Grub* instead)
        * Unified kernel images (UKI): Enabled
        * Plymouth: No
    * Kernels: linux-lts (This is Long-Term-Support, most reliable option)\
(You can install more than one and select at boot time.)
    * Hostname: Whatever you want
    * Authentication: (Optionally set a root password. If you leave it blank you can login as root without a password.)
    * Profile: Minimal (You can add packages using pacman later)
    * Applications: None (You can add applications using pacman later)
    * Network configuration: Use Network Manager (default backend)
    * Pacman: Color: True
    * Additional Packages: None (It's a huge list, selecting this can take time and anything can be added later.)
    * Timezone: US/Mountain
    * Automatic time sync: NTP Enabled
5. Start the installation

> The [Arch Installation Guide](https://wiki.archlinux.org/title/Installation_guide) never mentions the `archinstall` interactive installer. Instead, it lists a series of manual steps for partitioning and formatting your hard drive, setting a bootloader, and installing the operating system. Quite frankly, it's challenging and prone to error. There seems to be an ongoing debate in the Arch community between those who think all installation should be manual so that users learn what they are doing and others who think autoinstallation is a valuable feature. The compromise seems to be including `autoinstall` but not promoting it.

### Get Acquainted With the Arch CLI

Try a few commands in Arch such the following:
* `ip addr`
* `uname -a`
* `curl https://echo.dicax.org`

Install traceroute and trace your route to `www.byu.edu`.
```sh
pacman -S traceroute
traceroute www.byu.edu
```

If your hypervisor supports checkpoints, now is a good time power off your VM and save a checkpoint. That way you can return to this state if you ever need to.

### Add a GUI and Browser to Arch

With the minimal install, you are limited to a CLI (Command Line Interface) and to CLI applications.
In this step, you wil add a GUI (Graphical User Interface) and a wab browser.
We will use [GNOME](https://www.gnome.org/), a full graphical shell that is the default UI on Ubuntu, Fedora, and other graphically-oriented distributions. The standard GNOME distribution includes the chromium web browser and it uses the [Wayland](https://wayland.freedesktop.org/) graphics protocol by default.

Install GNOME. The first command, `sudo pacman -Syu` ensures that your system is up to date. The second command installs GNOME, ghe
```sh
sudo pacman -Syu
sudo pacman -S gnome gdm networkmanager
```

Once you have done that, you need to enable the network manager and GDM. The second command will not only enable GDM but it will launch the GNOME environment. In future reboots
```sh
sudo systemctl enable --now NetworkManager
sudo systemctl enable --now gdm
```

Explore the graphical applications included in GNOME. The web browser (called "web") is Epiphany and is based on WebKit.

### Record Your Success
* *In the Web browser included with your new Arch/GNOME installation* open LearningSuite and log in.
* Under **Assignments** or in the **Schedule** open Lab 1.
* Click `Record an OS for Lab-1`
* Enter `Arch Linux` for the name of the operating system.
* Click `Submit`

> **Important** You must record using a web browser in the operating system you just installed. We will use the User-Agent string and IP address to validate that you are running under a new VM.
> From any web browser you can select `View my Lab-1 report` to see all of the OS records you have reported.

## Submission

Your submission is through the "Record an OS for Lab-1" links that you used on each of the operating systems. You can verify your submission by clicking on the `View y Lab-1 report` link in the Lab 1 assignment in LearningSuite. You should have two entries there. One for a version of Windows and one for Arch Linux. Be sure that both are there and that you name which version of Windows you installed.

You cannot delete records. But if one is not what you wanted or expected, you can submit another and we will grade the best versions.

### Points
* 20 Points: Install a version of Windows. Explore the features and submit an online report.
* 40 Points: Install Arch Linux with the GNOME GUI and web browser. Explore the features and submit an online report.

## Extra Credit

Max 40 extra credit on this lab. You may do extra credit work at any time during the semester. Report extra credit work via email to [brandt.redd@byu.edu](mailto:brandt.redd@byu.edu).

### [20 Points] Install React OS

[React OS](https://reactos.org/) is an open source clone of Microsoft Windows. With more than 25 years of development it is still considered to be Alpha. It targets compatibility with Windows Server 2003 and later. The UX is presently modeled on that version. Browse to the [React OS](https://reactos.org/) and download the installer. Unpack the zip to get a .ISO file. Install it on a VM and try it out. Use screenshots as evidence of your success.

### [20 Points] Create a Custom Windows Installer

Due to the advertising and application bloat bundled with Windows, there are several efforts to create trimmed-down versions of the Windows installer. Consider [Bla bla bla](), [Tiny 11](), or a similar project. Use it to customize your Windows installer, install on a VM. In your email, describe what you used, what you did, and what you learned.

### [20 Points] Install Arch Linux Manually

In our instructions above, we recommended using the `archinstall` automated tool. Instead of that, follow the manual steps in the [Arch Installation Guide](https://wiki.archlinux.org/title/Installation_guide) to install on a VM. This includes manually partitioning and formatting the hard drive, installing the bootloader, and installing the operating system. Capture some screenshots along the way as evidence of your work.

### [20 Points] Install macOS in a VM

macOS is not typically run in a VM. Do it anyway. Follow a guide such as [this one](https://www.digitalcitizen.life/how-to-use-macos-on-a-windows-pc-using-a-virtual-machine/) to help you on your quest. Use screenshots to as evidence of your success.



