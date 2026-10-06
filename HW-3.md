---
title: "HW 3: Host Firewalls"
---
Your computer has a host firewall—or, at least, it should. A *host firewall* is software that runs on an individual computer and controls network traffic to and from that computer. This contrasts with a network firewall, which usually runs on dedicated hardware at the boundary between a local-area network and the internet.

On **Windows**, the host firewall is **Windows Defender Firewall**. It is part of the **Windows Security** toolset, which also includes **Microsoft Defender Antivirus** and **Microsoft Defender SmartScreen**.

On **macOS**, two services jointly perform the role of a host firewall. The **Application Firewall** controls incoming network access by applications, while **Packet Filter**, or **PF**, controls network traffic at the packet and port levels.

On **Linux**, the host-firewall framework is **Netfilter**, which is built into the Linux kernel. Netfilter is commonly configured through tools such as **UFW (Uncomplicated Firewall)**, although UFW is an optional application and is not used by every Linux distribution.

In this homework assignment, you will review the existing settings on your host firewall, experiment with changing a few settings, and write up your findings. What you learn here will inform your **Lab 3** assignment, in which you will use a host firewall and related tools to harden a web server running on Ubuntu.

Choose one of the three options below: **Windows**, **macOS**, or **Linux**; whichever is most convenient or is your "daily driver."

## Windows

To perform these tasks, you need administrator access to Windows. Therefore, if you are completing this homework on a lab computer where you do not have administrator access, you will need to use a Windows virtual machine on which you do have administrator rights.

Open **Windows Defender Firewall**. The easiest way to find it is to search for it from the Start menu.

The basic settings interface is relatively limited. Take note of the following:

* There are distinct settings for private and public networks. Some computers may also display a domain network profile. When you connect to a network, you can indicate whether it is private or public in your network settings. New networks are generally treated as public by default.
* You can turn the firewall on or off.
* You can control whether Windows notifies you when an application is blocked from accessing the network.

Select **Advanced settings**. This opens the interface containing the firewall rules.

Take note of the following:

* The top entry in the left column displays the overall firewall profile. In particular, it shows the default actions for inbound and outbound connections that do not match a rule.
* **Inbound Rules** lists the active rules controlling inbound network connections.
* **Outbound Rules** lists the active rules controlling outbound network connections.

Do the following:

1. In **Outbound Rules**, add a rule that blocks outbound TCP traffic to port 80, the standard port for HTTP.
2. Open your browser and attempt to browse to [http://echo.dicax.org](http://echo.dicax.org). (Be sure to specify HTTP, not HTTPS.) The request should fail.
3. Open your browser and attempt to browse to [https://echo.dicax.org](https://echo.dicax.org) (with HTTPS). The request should succeed.
4. Disable your new rule and attempt to access the same URLs again. Both requests should succeed.

Proceed to the **Writeup and Submission** section below to complete this homework.

## macOS

*These instructions have not been fully validated. If you are a macOS user, you may consider following the Windows instructions on your Windows virtual machine. If you encounter problems with these instructions, consult [this article](https://inventivehq.com/knowledge-base/macos/how-to-configure-macos-firewall-pf) and/or an AI assistant. You may also consider adding detail based on the extra-credit options below.*

**macOS** has two services that jointly perform the role of a host firewall. The **Application Firewall** controls network access by application, while **Packet Filter** handles network-level rules.

Open the **macOS Application Firewall** at `Apple menu > System Settings > Network > Firewall`

Take note of the following:

* How to turn the firewall on or off
* How to block all incoming connections
* What you can control regarding application permissions
* What stealth mode does

For the following steps, you will configure **Packet Filter** using configuration files and the command line.

1. Create a custom anchor file named `/etc/pf.anchors/block-http`.
2. In that file, add the following rule:
```
block out proto tcp from any to any port 80
```
3. Edit `/etc/pf.conf` to reference the anchor file by adding the following lines:
```
anchor "block-http"
load anchor "block-http" from "/etc/pf.anchors/block-http"
```
4. Open a Terminal window and validate your changes with the following command:
```sh
sudo pfctl -nf /etc/pf.conf
```
> The `-n` option performs a dry run, validating the configuration without loading it.

5. From the CLI Terminal, load the configuration and enable the packet filter:
```sh
sudo pfctl -f /etc/pf.conf
sudo pfctl -e
```
6. You can verify the loaded packet-filter rules with the following command:
```sh
sudo pfctl -s rules
```
7. Open your browser and attempt to browse to [http://echo.dicax.org](http://echo.dicax.org). (Be sure to specify HTTP, not HTTPS.) The request should fail.
8. Open your browser and attempt to browse to [https://echo.dicax.org](https://echo.dicax.org) (with HTTPS). The request should succeed.
9. Disable your packet filter rule by removing it or commenting it in `/etc/pf.conf`. Then reload the configuration:
```sh
sudo pfctl -f /etc/pf.conf`
```
10. In your browser, verify that HTTP (unencrypted) access has been restored.

> Depending on the macOS version and configuration, custom packet-filter settings may not persist across a reboot or system update. If you want the settings to be loaded automatically, you may need to use a LaunchDaemon or another startup mechanism.

Proceed to the Writeup and Submission section below to complete this homework.

## Linux

If you use **Linux** for your daily driver, you should be accustomed to adapting instructions to your platform. You will need to do the same here. However, you will have an advantage on **Lab 3** which follows since that will all be on a Linux VM.

*These instructions were validated with UFW 0.36 on Ubuntu 24.04. To perform these tasks, you need `sudo` (administrator) access. If you do not run Linux natively, you can use a Linux virtual machine.*

The host firewall on Linux is **Netfilter**, which is built into the kernel and always present. Netfilter rules are managed by lower-level tools (`iptables` or `nftables`), and most people configure them through a friendlier front end. On Ubuntu and Debian, that front end is **UFW (Uncomplicated Firewall)**. (Fedora and RHEL use **firewalld** instead. If your distribution uses firewalld, either adapt these steps with `firewall-cmd` or disable firewalld before installing UFW so the two do not conflict.)

Install UFW if it is not already present:
* Debian/Ubuntu: `sudo apt install ufw`
* Arch: `sudo pacman -S ufw`

> **Remote machines:** if you are connected to the machine over SSH, allow SSH **before** enabling UFW, or you will lock yourself out: `sudo ufw allow OpenSSH`.

Check UFW's status before you change anything. On a fresh install it reports `Status: inactive`, meaning UFW is not filtering anything yet:

```sh
sudo ufw status verbose
```

Enable UFW and check its status again:

```sh
sudo ufw enable
sudo ufw status verbose
```

Take note of the following:

* The `Default:` line shows the actions applied to traffic that matches no rule. UFW's defaults are `deny (incoming), allow (outgoing), deny (routed)`. These defaults are stored in `/etc/default/ufw`.
* `sudo ufw app list` lists the **application profiles** (for example `OpenSSH`, `Apache`, `Nginx Full`) that you can allow or deny by name instead of by port. Profiles live in `/etc/ufw/applications.d/`.
* `sudo ufw status numbered` lists the active rules with numbers. Outbound rules are marked `(out)`. Each rule is usually created twice, once for IPv4 and once for IPv6 (`(v6)`).
* `sudo ufw show added` lists the rules you have added in the same syntax you used to create them.
* (Optional) To see the Netfilter rules that UFW generates, run `sudo iptables -S ufw-user-output` (outbound) or `sudo iptables -S ufw-user-input` (inbound).

Use UFW to do the following:

1. Add a rule that blocks outbound TCP traffic to port 80, the standard port for HTTP:
```sh
sudo ufw deny out 80/tcp
sudo ufw status numbered
```
You should see `80/tcp DENY OUT Anywhere (out)` and the same rule for `(v6)`. (`deny` silently drops the packets, so connections time out. `reject` instead sends an immediate error back to the application.)

2. Open your browser and attempt to browse to [http://echo.dicax.org](http://echo.dicax.org). (Be sure to specify HTTP, not HTTPS.) The request should fail.
> Current versions of Chrome and Firefox may automatically upgrade `http://` URLs to `https://`, which would make the page load over port 443 even though port 80 is blocked. If the address bar shows `https://` or the page reports `CLIENT-PROTO: https`, the browser upgraded the request. Test with `curl -v --max-time 10 http://echo.dicax.org` (expect `Connection timed out`), or disable the HTTPS-only/upgrade setting in your browser.

3. Open your browser and attempt to browse to [https://echo.dicax.org](https://echo.dicax.org) (with HTTPS). The request should succeed.
4. UFW does not have a "disable rule" command, so remove the rule instead. You can delete it by its specification or by its number from `sudo ufw status numbered` (delete the higher-numbered `(v6)` rule first, because the numbers shift after each deletion):
```sh
sudo ufw delete deny out 80/tcp
```
5. Attempt to access the same URLs again. Both requests should succeed, and the page should report `CLIENT-PROTO: http` for the HTTP URL.
6. (Optional) Return UFW to its original state. If it was inactive when you started, run `sudo ufw disable`.

Proceed to the **Writeup and Submission** section below to complete this homework.

## Writeup and Submission

To complete this homework, you will write up what you did and submit it **in PDF format** to LearningSuite.
You may use the word processing or writing software of your choice so long as it can produce a **PDF** (Markdown, Google Docs, LibreOffice, LaTeX, Microsoft Word, etc.).

Please follow this outline:
* [2 Points] Name, Date, and Homework Title
* [8 Points] What did you observe about your host firewall before you made any changes?
    * What default settings apply when no rule matches?
    * What applications (if any) were enabled or blocked?
    * What IP addresses or ports (if any) were enabled or blocked?
    * Any other observations?
* [10 Points] Blocking Port 80
    * How did you block port 80?<br/>(Include a configuration setting or a screenshot.)
    * What did your browser report when you attempted to browse to [http://echo.dicax.org](http://echo.dicax.org) with the port blocked?
* [10 Points] Unblocking port 80
    * How did you unblock port 80?<br/>(Delete rule, disable rule, other?)

Upload your **PDF** writeup to LearningSuite.

## Extra Credit

*Extra credit can be done any time before the last regular day of class. It need not be completed before the due date of this lab.*

* [8 Points] I have not validated the macOS instructions. Make improvements as follows:
    * Fork the course website repo [located here](https://github.com/BYU-CYBER-344/c344.github.io).
    * Make corrections and improvements to your fork.
    * Submit a pull request with your fixes.
* [8 Points] Add details to the Linux instructions equivalent to those given for Windows.
    * Fork the course website repo [located here](https://github.com/BYU-CYBER-344/c344.github.io).
    * Make corrections and improvements to your fork.
    * Submit a pull request with your fixes.
* [10 Points] Block one or more websites from reporting to Google Analytics
    * Research how Google Analytics functions.
    * Create one or more host firewalls rules to block that reporting.
