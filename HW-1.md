---
title: "HW 1: VM Hypervisor and Container Host"
---

*This is new. Please bring errors or typos to the attention of Prof. Redd or the TAs.*

In this homework you will select, prepare, and test your platforms of choice for a virtual machine hypervisor and a container host.
You will use these platforms (VM and container) throughout the semester.
For grading, you will submit a short writeup with screenshots.

Here are the options. Details for each one are at the bottom of these instructions.

### Preferred Options for Hypervisors:
* VMWare running on a lab computer.
* Microsoft Hyper-V running on your own Windows computer.
* UTM running on your own MacOS (Apple) computer.
* VMware Workstation running on your own computer.
* ProxMox running on your own computer (bare metal).

### Alternative Options for Hypervisors (untested by us)
* Amazon Web Services EC2 using AWS Academy ($50 credit for the semester).
* Any option of your choice that meets the requirements of the class.

### Preferred Options for Containers
* Docker Desktop running on a lab computer.
* Docker Desktop for Windows running on your own computer.
* Docker Desktop for MacOS running on your own computer.
* Docker Desktop for Linux running on your own computer.
* Docker Desktop for Linux running on your Ubuntu VM.

### Alternative Options for Containers (Untested by us)
* Microsoft WSL Containers (new, untested by us).
* PodMan for Linux
* Any option of your choice that meets the requirements of the class.

## Introduction

The homework, labs, and final project of this class will require you to try out different operating systems and platforms.
To gain experience with a variety of environments, you will need two ways to host them: Virtual Machine Hypervisors and Containers.
You should already be familiar with using containers from IT&C 210 (now CYBER 210).
In this class we will create our own containers for specific purposes.

For this assignment, you will choose a hypervisor and a container host, install each, and test their operation.
Then you will write up your choices and what you observed.

You may and should use internet resources to help you through these steps.
You may use AI in search and help mode but not to do the tasks for you.
Be sure to meet all requirements and pay attention to the rubric.

### Requirements for Hypervisors

If you choose one of the preferred options, *and your host system meets minimum requirements*, then you can be confident that your solution will meet the requirements.

* Must be capable of hosting Ubuntu Linux distribution. Preferably the desktop version with a GUI.
* Must be capable of hosting Windows Home, Windows Pro, or Windows Server.
* System must be capable of hosting at least 3 VM instances, each with a 15 - 20GB virtual drive image.
* System must be capable of allocating at least 4GB to a running VM. (No need to run more than one at a time.)

### Requirements for Container Hosts

If you were able to handle the containers from CYBER 210 then that will be sufficient for this class.

* Must be capable of hosting OCI (Open Container Initiative) format containers. (This is the format used by Docker.)
* System must be capable of allocating at least 1GB of memory 3GB of storage to a container.

## Tasks

Do the following:

### Part 1: Hypervisor

1. Download the Ubuntu Desktop installation .iso file from here: [https://ubuntu.com/desktop](https://ubuntu.com/desktop)
2. From the list above and details below, choose and install a hypervisor. If your first choice doesn't work out, or if you don't like it, you may have to try another option.
3. Following the instructions for your hypervisor, install Ubuntu Desktop.
4. Ensure your copy of Ubuntu works and that it has access to the internet. Use at least one GUI and one CLI app. Here are some good options.
    * Open the bundled Firefox browser and browse to a few websites.
    * From the CLI use some network-related commands such as `ifconfig`, `ping`, `curl`, etc. As needed, use `apt install` to install needed tools.

> A *.ISO* file is an image of a CD, DVD, or Blu-Ray disk. The name comes from the *ISO 9660* standard for file systems on optical device media. Given that there are literally tens of thousands of ISO standards it's interesting that this particular file format captured the *.ISO* extension.

### Part 2: Container Host

1. From the list above and details below, choose and install a container host. If your first choice doesn't work out, or you if don't like it, you may have to try another option.
2. Ensure your container host works, that it has network access, and that you can use an interactive CLI container. Here is one option. The first line assumes you are using Docker for your host.
    * `docker run --rm -it nicolaka/netshoot`
    * `ping byu.edu`
    * `tcptraceroute byu.edu`
    * `nslookup c344.byucyber.net`
    * `exit`
3. Try at least one other container. Here's a fun choice:
    * `docker run --rm -it defnotgustavom/tetris`

> On the docker run command, `--rm` tells docker to remove the stopped container when you exit. `-it` runs the container in interactive-terminal mode which lets you interact with a running container.
> If you want to waste a bunch of time and see what games were like before computer graphics, try `erezbinyamin/sst` or `matsuu/nethack`.

## Writeup and Submission

To complete this homework, you will write up what you did and submit it **in PDF format** to LearningSuite.
It **MUST** have screenshots as evidence of your work.
You may use the word processing or writeup software of your choice so long as it can produce a **PDF** (Markdown, Google Docs, LibreOffice, Microsoft Word, etc.).

Please follow this outline and rubric:
* [2 Points] Name, Date, and Homework Title
* [9 Points] Your choice of hypervisor:
    * Which did you choose?
    * How easy or hard was it to install the hypervisor?
    * How easy or hard was it to install Ubuntu?
    * How satisfied are you with your choice?
    * Other comments (optional).
* [5 Points] One or two hypervisor screenshots.
    * Ubuntu running in a window of your hypervisor.
    * Network-related command in a CLI window under Ubuntu.
* [9 Points] Your choice of container host:
    * Which did you choose?
    * How easy or hard was it to install the container host?
    * How easy or hard was it to run two different containers?
    * How satisfied are you with your choice?
    * Other comments (optional).
* [5 Points] One or two container screenshots.
    * Network-related commands running in your container.

Upload your **PDF** writeup to LearningSuite.

## Hypervisor Options

This should be sufficient information to get you started with each hypervisor option. Look at the documentation for each hypervisor, check with the TAs, or ask your favorite AI for additional information. *But don't turn over the task to an AI of some sort!*

### VMWare running on a lab computer.
This is easiest and quickest.
However, it requires you to be in the lab whenever you are working with your VMs which you will be doing throughout the semester.
You may choose to put your VM configuration and disk image on a (large) thumb drive so that you are not tied to one particular computer.

* Log into one of the lab computers using your BYU netid and password.
* Launch `VMWare Player`. (Be sure it's *VMWare Player*, not VMWare Desktop or any of the other VMWare options on the computer).
* Create a new virtual machine.
* Set it to install from the Ubuntu .iso file you downloaded previously.
* Launch the VM and work through the OS installation process.

### Microsoft Hyper-V running on your own Windows computer.
If you are running Windows 10 or 11 Pro this is probably your most convenient option. Windows Home does not include Hyper-V. In that case, you could consider upgrading to Pro (typically by purchasing a discounted license online) or using VMware Workstation (below).

* Open Settings
* Navigate to System > Optional Features > More Windows Features
* Select `Hyper-V` with all of its tools.
* You might also consider enabling `Windows Sandbox`. It's not needed for this class but it's nice to have.
* Click OK

After rebooting (to install the features):
* Launch Hyper-V Manager
* Select New > Virtual Machine
* Name your VM
* Select Generation 2
* Give it 4096MB of memory (less if your computer is memory constrained)
* Select *Use dynamic memory*
* For networking select *Default Switch*
* Create a new virtual hard disk. Choose a size of at least 20GB. The virtual hard disk image will only take up as much space on your drive as the VM actually uses.
* Under installation options choose "Install an operating system from a bootable image file" and select the Ubuntu .ISO that you previously downloaded.
* Select **Finish**
* Before booting, select **Settings > Security** for your VM, set *Enable secure boot* and change the *Template* to *Microsoft UEFI Certificate Authority*.
* Boot your VM and install Ubuntu.

### UTM running on your own macOS computer.
If you have a Mac, and you don't want to use a lab computer, this is probably your best option.
UTM makes use of QEMU CPU emulation to run x86-64 operating systems like Windows.
This comes with a significant performance penalty, but you may find that the convenience is worth it.
For this exercise, you can install the ARM-based Ubuntu distribution on UTM in which case it will run at full CPU speed.

*These instructions are limited until a mac-running student or TA fills in details.*
* Download UTM from https://mac.getutm.app/ or install it from the app store.
* Follow the UTM instructions to install the Ubuntu OS.

### VMware Workstation running on your own computer.
If your are running Windows Home, or if you have Windows Pro but you prefer the VMware UX then this may be your best solution.

* Follow the installation instructions here: [https://knowledge.broadcom.com/external/article/368734/download-desktop-hypervisor-workstation.html](https://knowledge.broadcom.com/external/article/368734/download-desktop-hypervisor-workstation.html)
* Follow the instructions above for installing Ubuntu on a lab computer.

### ProxMox running on your own computer
This solution is mostly appropriate for a home lab unless you have a very capable laptop.
ProxMox is a *Bare Metal* Type-1 hypervisor.
You install it on a computer before installing one or more virtual machines within ProxMox.
Our instructions here are limited. If you choose this option you will have to figure out much of it on your own.

* Download and install Proxmox Virtual Environment here: https://proxmox.com/en/products/proxmox-virtual-environment/overview
* Install it on a bare computer. (One with no operating system you intend to preserve.)
* Install Ubuntu as one VM.

### Amazon Web Services EC2 using AWS Academy (not preferred)

Virtual machines running on the AWS cloud are usually run from the CLI.
That's the only option for Linux.
For Windows, you can use Remote Desktop to access the GUI on a cloud machine but getting that set up is complicated.
So, while this is an acceptable option, it is not preferred.

* Ask Prof. Redd to invite you to the AWS Academy Learner Lab.
* Once you have accepted the invitation, follow [these instructions from CYBER 210 to create an Ubuntu EC2 instance.](https://byu-itc-210.github.io/AWS-Server-Setup).
