---
title: "HW 3: Host Firewalls"
---

Your computer has a host firewall—or, at least it should. A *host firewall* is a software firewall that runs on your computer. This is in contrast to a network firewall that usually runs on dedicated hardware on the border between a local area network and the internet.

For **Windows** the host firewall is, **Windows Defender Firewall**. It's part of the **Windows Defender** toolset which also includes **Windows Defender Antivirus** and **Windows Defender SmartScreen**.

On **macOS**, the host firewall has two parts, **Application Firewall** which manages network access by applications and **Packet Filter** which controls network access at the packet and port levels.

On **Linux** the host firewall is the **Netfilter** system which is built into the kernel. On most distros, you configure **Netfilter** using **UFW (Uncomplicated Firewall)** which is an optional application.

In this homework assignment you will review the existing settings on your host firewall, experiment with changing a few settings, and write up your findings. What you learn here will inform your **Lab 3** assignment in which you will use a host firewall and related tools to harden a web server running on Ubuntu.

## Windows

To perform these tasks you need administrator access to Windows. Thus, in order to do this homework on a lab computer (where you don't have admin access) you need to use a Windows VM on which you *do* have admin rights.

Open `Windows Defender Firewall` in your settings. It is most easily found by opening `settings` and searching for `Windows Defender Firewall`.

The settings here are rather limited. Take note of the following:
* There are distinct settings for private networks and public networks. When you connect to a network, you can indicate whether it is private or public in your network settings. All networks are public by default.
* You can turn the firewall on and off.
* You can change whether you are notified when an app is blocked from accessing the network.

Click on `Advanced settings`. This is where things get more interesting.

Take note of the following:
* The top entry in the left column will display the overall profile. In particular, you can see what the defaults are for inbound and outbound connections that do not match a rule.
* Inbound Rules in the left column will list all active rules controlling inbound network connections.
* Outbound Rules does the same for outbound connections.

Do the following:

1. To your outbound rules, add a rule that prevents any access to TCP port 80. (That is the default port for HTTP).
2. Open your browser and attempt to browse to [http://echo.dicax.org](http://echo.dicax.org) (Be sure to specify HTTP, not HTTPS.) It should fail.
3. Open your browser and attempt to browse to [https://echo.dicax.org](https://echo.dicax.org) (with HTTPS) It should succeed.
4. Disable your new rule and attempt access those same URLs.

Proceed to the Writeup and Submission section below to complete this homework.

## macOS

*These instructions have not been validated. If you are a **macOS** user, you may consider following the **Windows** instructions on your Windows VM. If you have problems with these instructions, consult [this article](https://inventivehq.com/knowledge-base/macos/how-to-configure-macos-firewall-pf) and/or AI for help. Also, consider adding detail as described in the extra credit options below.*

**macOS** has two services that jointly fill the host firewall role. The *Application Firewall* controls the application permissions to access the network. The *Packet Filter* handles network-level rules.

Open the **macOS Application Firewall** at `Apple menu > System Settings > Network > Firewall`

Take note of the following:
* How to turn the firewall on or off
* How to block all inbound connections
* What can you control regarding application permissions?
* What does stealth mode do?

For the following steps, you need to configure the **Packet Filter** which is done through configuration files and the CLI.

1. Create a custom anchor file named `/etc/pf.anchors/block-http`
2. In that file, add the following rule:
```
block out proto tcp from any to any port 80
```
3. Edit `/etc/pf.conf` to reference the anchor file by adding the following line
```
anchor "block-http"
load anchor "block-http" from "/etc/pf.anchors/block-http"
```
4. Open a terminal and validate your changes using the following CLI command
```sh
sudo pfctl -nf /etc/pf.conf
```
(The `-n` option indicates to do a dry run, validating the configuration without loading it.)
5. From the CLI terminal, load the configuration and enable the packet filter
```sh
sudo pfctl -f /etc/pf.conf
sudo pfctl -e
```
6. You can verify the operational packet filter rules with the following command.
```sh
sudo pfctl -s rules
```
7. Open your browser and attempt to browse to [http://echo.dicax.org](http://echo.dicax.org) (Be sure to specify HTTP, not HTTPS.) It should fail.
8. Open your browser and attempt to browse to [https://echo.dicax.org](https://echo.dicax.org) (With HTTPS) It should succeed.
9. Disable your packet filter rule by removing it or commenting it in `/etc/pf.conf` then reloading the configuration with `sudo pfctl -f /etc/pf.conf`
10. In your browser, verify that HTTP (unencrypted) access has been restored.

> These packet filter settings only last until the next reboot. If you want packet filter settings to be persistent, you must use a LaunchDaemon to automatically load the packet filter.

Proceed to the Writeup and Submission section below to complete this homework.

## Linux

If you use **Linux** for your daily driver, you should be accustomed to adapting instructions to your platform. You will need to do the same here. However, you will have an advantage on **Lab 3** which follows since that will all be on a Linux VM.

Here is some info to get you started:
* The host firewall on Linux **NetFilter** which is in the kernel and always present.
* You configure the host firewall with **UCW (Uncomplicated Firewall)** which must be installed:
    * Debian/Ubuntu: `sudo apt install ufw`
    * Arch: `sudo pacman -S ufw`
* Once installed you enable it with this command
```sh
sudo systemctl enable --now ufw.service
```
* You can check its status
```sh
sudo ufw status verbose
```

Explore UFW configuration and options sufficiently to answer the associated submission question.

Use UFW to do the following:
1. Add a rule that prevents any access to TCP port 80. (That is the default port for HTTP).
2. Open your browser and attempt to browse to [http://echo.dicax.org](http://echo.dicax.org) (Be sure to specify HTTP, not HTTPS.) It should fail.
3. Open your browser and attempt to browse to [https://echo.dicax.org](https://echo.dicax.org) (with HTTPS) It should succeed.
4. Disable your new rule and attempt access those same URLs.

Proceed to the Writeup and Submission section below to complete this homework.

## Writeup and Submission

To complete this homework, you will write up what you did and submit it **in PDF format** to LearningSuite.
You may use the word processing or writing software of your choice so long as it can produce a **PDF** (Markdown, Google Docs, LibreOffice, LaTeX, Microsoft Word, etc.).

Please follow this outline and rubric:
* [2 Points] Name, Date, and Homework Title
* [8 Points] What did you observe about your host firewall before you made any changes?
    * What default settings apply when no rule is matched?
    * What applications (if any) were enabled or blocked?
    * What IP addresses or ports (if any) were enabled or blocked?
    * Any other observations?
* [10 Points] Blocking Port 80
    * How did you block port 80?<br/>(Include a configuration setting or a screenshot.)
    * What did your browser report when you attempted to browse to [http://echo.dicax.org](http://echo.dicax.org) with the port blocked?
* [10 Points] Unblocking port 80
    * How did you unblock port 80?<br/>(Delete rule, disable rule, other?)

Upload your **PDF** writeup to LearningSuite.
