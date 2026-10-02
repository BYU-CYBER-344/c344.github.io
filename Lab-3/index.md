---
title: "Lab 3: System Hardening"
---

*Under construction!*

In this lab you will install the Apache web server on the Ubuntu virtual machine you created in [Homework-1](/HW-1). You will load and configure a sample web site that we configured for you. Then you will harden the Ubuntu operating system against attacks using a combination of [Uncomplicated Firewall (UFW)](https://wiki.ubuntu.com/UFW) and [Fail2Ban](https://en.wikipedia.org/wiki/Fail2ban).

## A. Prepare your OS and VM

You should be able to use the Ubuntu VM instance you created in [Homework-1](/HW-1). Check that it boots, is stable and doesn't have any unusual configuration or applications installed. If you have any concerns, create a new instance and re-install Ubuntu from the installation .iso at [https://ubuntu.com/desktop](https://ubuntu.com/desktop).

We recommend that you a checkpoint (Hyper-V) or snapshot (VMWare, ProxMox) so that you can return your VM to the baseline state if you have any problems. Much like making Git commits, you may make multiple snapshots as you progress to save time in case of problems.

## B. Install Apache and Verify Access

Open a terminal and use the following commands to install Apache on your server. Be sure you know what each command does. If necessary, look up the commands or ask an AI what each does. Remember that `#` in bash indicates a comment. So in the commands below, `# optional, recommended` is telling you that upgrading the Ubuntu components is an optional step but we recommend upgrading the Ubuntu components to the latest.

```sh
sudo apt update
sudo apt upgrade # optional, recommended
sudo apt install apache2
sudo systemctl enable --now apache2
```

On your Ubuntu VM, launch the web browser (typically Firefox) and browse to `http://localhost`. You should see the **Apache2 Default Page**.

Next, you need to verify that you can access your Ubuntu-hosted Apache web server from another computer. This will be necessary for testing your hardening later in the lab. Find out the IP address of your your Ubuntu VM with the following command:

```sh
hostname -I
```

> You can also get the address using `ip addr` or `ifconfig` but `hostname -i` is usually simpler.

On the computer hosting the VM, or on another computer on your LAN, browse to that IP address and verify that you see the **Apache2 Default Page**.

> Most of the time this just works. However, if you cannot access the web server from another computer, it may be due to the way your hypervisor bridges virtual machines to the network. Use the documentation regarding network settings for your hypervisor and/or AI chat support as needed to update your network settings and ensure that at least one other computer can access your web server. You will use a browser on that other computer to test your hardening.

## C. Load and Configure the Web Site

To simulate a secure web site (which this is not) the sample web site tests passwords on the server side. Therefore, it must use server-side code. To keep things simple, the server-side code is written as a Bash script and called using **Common Gateway Interface (CGI)**.

> CGI dates to the earliest days of the web. It is a way for a web server, like Apache, to activate code, send input to that code, and let the code generate a response to send back to the browser. The API is very simple. The web server launches the program, sends the input via [stdin](https://en.wikipedia.org/wiki/Standard_streams#stdin), captures the output that comes from [stdout](https://en.wikipedia.org/wiki/Standard_streams#stdout) and passes the output back to the browser. The program can be anything that the operating system can run: native binary code, a shell script, Perl, PHP, Python, JavaScript, etc. The output can be anything the browser can handle: html, css, images, video, etc.

Use the following commands to download and unpack the website, and download and apply the Apache configuration. Be sure you know what each command does. As needed, look up the commands or query AI on what they are doing.

```sh
sudo apt install curl
curl -f https://c344.byucyber.net/Lab-3/sample-site.tar | sudo tar -x -C /var/www
sudo chmod +x /var/www/cgi-bin/*.cgi
curl -f https://c344.byucyber.net/Lab-3/apache-config.tar | sudo tar -x -C /etc/apache2/conf-available
sudo a2enmod cgid
sudo a2disconf serve-cgi-bin
sudo a2enconf serve-sample-site
sudo apache2ctl configtest
sudo systemctl restart apache2
```

Test your website by browsing to the IP address of your Ubuntu server from another computer.
* The home page simply says "Sample Site" with a login link.
* To log in, use any username. The password is the same as the username plus "-9455". For example, if the username is "Thag" then the password is "Thag-9455".

If anything doesn't work, troubleshoot the problems before moving on to the next step.

> Understanding what each of the above commands does will help in troubleshooting. AI can also provide valuable help, especially of you provide the exact error messages. However, it is not reliable so don't let AI take you too far down a rabbit hole before trying something else. You can also return to a vm snapshot and start over. These commands can be repeated pretty quickly.

## D. Harden your server

To help protect your server from attacks, we will use two tools: Uncomplicated Firewall (UFW) and Fail2Ban. Each of these is intended to be used as a layer of protection following a [defense in depth](https://en.wikipedia.org/wiki/Defense_in_depth_(computing)) strategy. In the case of UFW, you will block network ports that are not needed in this application. Any software listening on those ports should have its own layer of security so this is an additional layer that can prevent exploitation of undiscovered vulnerabilities. In the case of Fail2Ban, you will protect your server from repeated attempts to guess a password thereby increasing the utility of weak passwords.

Install UFW and Fail2Ban with the following commands:
```sh
sudo apt update
sudo apt install ufw fail2ban
```

[UFW](https://wiki.ubuntu.com/UFW) is a configuration tool for the `iptables` firewall built into the Linux kernel. Before enabling UFW, you must first open any important ports. Otherwise you could lock yourself out of your own server. This is less of a risk when using a VM on a local host because you have direct access to the console. When running a cloud server such as an Amazon [EC2](https://aws.amazon.com/pm/ec2/) instance, people have accidentally blocked [SSH](https://ubuntu.com/server/docs/how-to/security/openssh-server/) to find themselves unable to access their servers.

Configure UFW to allow Apache to serve web sites.
```sh
sudo ufw allow Apache
```

> This command simply enables the `Apache` profile in UFW. Is is not actually connected to the **Apache Web Server** application. And all that profile does is enable inbound connections on TCP port 80. You can observe the parameters of the profile using this command: `sudo ufw app info Apache`. Likewise, of you need to handle secure communications you might use this command `sudo ufw allow 'Apache Full'`. If you examine that profile you will find that it opens TCP ports 80 and 443.

If you are using SSH to access your server then be sure to open the SSH ports:
```sh
sudo ufw allow ssh
```

> The names specified to the `ufw allow` command can either be application profiles which are found in `/etc/ufw/applications.d` or service names which are mapped to ports in the file `\etc\services`.

Now activate UFW and review its configuration with the following commands:
```sh
sudo ufw enable
sudo ufw status verbose
```

