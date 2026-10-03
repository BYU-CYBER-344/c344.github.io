---
title: "Lab 3: System Hardening"
---

In this lab you will install the Apache web server on the Ubuntu virtual machine you created in [Homework-1](/HW-1). You will load and configure a sample web site that we configured for you. Then you will harden the Ubuntu operating system against attacks using a combination of [Uncomplicated Firewall (UFW)](https://wiki.ubuntu.com/UFW) and [Fail2Ban](https://en.wikipedia.org/wiki/Fail2ban).

## A. Prepare your OS and VM

You should be able to use the Ubuntu VM instance you created in [Homework-1](/HW-1). Check that it boots, is stable and does not have any unusual configuration or applications installed. If you have any concerns, create a new instance and re-install Ubuntu from the installation .iso at [https://ubuntu.com/desktop](https://ubuntu.com/desktop).

We recommend that you create a *checkpoint* (Hyper-V) or *snapshot* (VMWare, ProxMox) so that you can return your VM to the baseline state if you have any problems. Much like making Git commits, you may make multiple snapshots as you progress to save time in case of problems.

## B. Install Apache and Verify Access

Open a terminal and use the following commands to install the Apache web server. Be sure you know what each command does. If necessary, look up the commands or ask an AI what each does. Remember that `#` in bash indicates a comment. So in the commands below, `# optional, recommended` is telling you that upgrading the Ubuntu components is an optional step but we recommend upgrading the Ubuntu components to the latest.

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

> You can also get the address using `ip addr` or `ifconfig`, but `hostname -i` is usually simpler.

On the computer hosting the VM, or on another computer on your LAN, browse to that IP address and verify that you see the **Apache2 Default Page**.

> Most of the time this just works. However, if you cannot access the web server from another computer, it may be due to the way your hypervisor bridges virtual machines to the network. Use the documentation regarding *network settings* for your hypervisor and/or AI chat support as needed to update your network settings and ensure that at least one other computer can access your web server. You will use a browser on that other computer to test your hardening.

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

If something doesn't work, troubleshoot the problem before moving on to the next step.

> Understanding what each of the above commands does will help in troubleshooting. AI can also provide valuable help, especially of you provide the exact error messages. However, it is not reliable so don't let AI take you too far down a rabbit hole before trying something else. You can also return to a VM snapshot and start over. The installation commands can be repeated pretty quickly.

## D. Harden your server

To help protect your server from attacks, we will use two tools: Uncomplicated Firewall (UFW) and Fail2Ban. Each of these is intended to be used as a layer of protection following a [defense in depth](https://en.wikipedia.org/wiki/Defense_in_depth_(computing)) strategy. In the case of UFW, you will block network ports that are not needed in this web application. Any software listening on those ports should have its own layer of security so this is an additional layer that can prevent exploitation of undiscovered vulnerabilities. In the case of Fail2Ban, you will protect your server from repeated attempts to guess a password thereby increasing the utility of weak passwords.

Install UFW and Fail2Ban with the following commands:
```sh
sudo apt update
sudo apt install ufw fail2ban
```

### 1. Configure UFW

[UFW](https://wiki.ubuntu.com/UFW) is a configuration tool for the `iptables` firewall built into the Linux kernel. Before enabling UFW, you must first open any important ports. Otherwise you could lock yourself out of your own server. This is less of a risk when using a VM on a local host because you have direct access to the console. When running a cloud server such as an Amazon [EC2](https://aws.amazon.com/pm/ec2/) instance, people have accidentally blocked [SSH](https://ubuntu.com/server/docs/how-to/security/openssh-server/) and find themselves unable to access their servers.

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

Test your web server to make sure it still works.

### 2. Prep and Review Fail2Ban Configuration

In Fail2Ban, `.local` files take precedence over `.conf` files. Because of this, we will make copies of the `.conf` files and edit the new `.local` files, leaving the `.conf` files untouched.

```sh
sudo cp /etc/fail2ban/fail2ban.conf /etc/fail2ban/fail2ban.local
sudo cp /etc/fail2ban/jail.conf /etc/fail2ban/jail.local
```

Jails can be found at `/etc/fail2ban/jail.local`. Read over the comments at the top of this file. They are very helpful for understanding how this file works. More information about the default jails and how they work can be found [here](https://docs.plesk.com/en-US/obsidian/administrator-guide/server-administration/protection-against-brute-force-attacks-fail2ban/fail2ban-jails-management.73382/).

### 3. Observe the Apache Access Logs to Inform Your Configuration

Apache is configured to store logs at `/var/log/apache2/`. You can view a live output of your Apache connections using:

```sh
sudo tail -f /var/log/apache2/access.log
```

With that command running, visit your website. Do a few good good and bad login attempts.

> Remember, a correct password is the same as the username with "-9455" appended. A bad password is anything else.

Your requests should show up in the terminal from the `tail` command. You can use these access logs to search for patterns to block. You should notice `200` status codes for successful logins and `401` status codes for bad logins. Use <kbd>Ctrl-c</kbd> to end the live log view.

### 4. Create a Jail Filter

Fail2Ban uses "jails" to detect malicious connections and deal with them. Pre-configured jail filters can be found in `/etc/fail2ban/filter.d/`. Jails are configured in `/etc/fail2ban/jail.local`. Within the latter file, a jail starts with a name in square brackets. By default, jails are disabled. Jails can be enabled by adding the line `enabled = true` to the jail reference.

Since you will be creating your own jail, create a filter file in `/etc/fail2ban/filter.d/` called `http-401.conf` with the following contents:

**/etc/fail2ban/filter.d/http-401.conf**
```conf
[Definition]
failregex = ^<HOST> \S+ \S+ \[[^\]]*\] "[^"]*" 401 
ignoreregex =
```

The `failregex` line uses a regular expression to flag server requests. In this case, it will be looking at our Apache access logs. You can compare this regex to the lines you viewd in `/var/log/apache2/access.log`

* `^` Indicates to start at the beginning of a line in the log file.
* `<HOST>` Fail2Ban uses this custom pattern to capture the IP address of the client. That way it knows what source to ban.
* `\S+` This is the `identd` value which is rarely used any more and has a simple `-` when empty. `\S+` matches any sequence of non-space characters.
* `\S+` Authenticated user. In our case, always `-` since the user does not authenticate to the Apache server but only to a web page.
* `\[[^\]]*\]` In the log, date/time is enclosed in square brackets. This pattern matches anything in square brackets.
* `"[^"]*"` The first line of the HTTP request which includes the verb (e.g. `GET`, `POST`), the path and query from the URL, and the HTTP protocol version. This pattern simply matches anything in double quotes.
* `401` The status code. This pattern only matches responses with a 401 status code.

In your `.conf` file, `ignoreregex` is left blank which means to apply the filter to anything that matches `failregex`.

### 5. Create Your New Jail

Edit `/etc/fail2ban/jail.local` and scroll down to the **JAILS** section and before the **SSH servers** section (both denoted in comments). That section usually starts empty. Create a jail named `[http-401]`. Following the examples of other jails, enter it as follows:

```conf
[http-401]
# Turns on the jail
enabled = true
# Only listen to ports 80 and 443
port = http,https
# Name of our jail filter file
filter = http-401
# Path to the log to be watched
logpath = /var/log/apache2/access.log
# Ban after 2 offenses
maxretry = 2
# The user will be banned for 60 seconds
bantime = 60
# Max retry counter will be reset to 0 after 180 seconds
findtime = 180
```

### 6. Restart Fail2Ban to Apply the Configuration

```sh
sudo service fail2ban restart
```

Check its status:
```sh
sudo service fail2ban status
```

If it is not active (running), then you probably have a syntax error in your config files

Verify that your new jail is running

```sh
sudo fail2ban-client status
```
If you see `http-401`, then it is actively looking for a user to ban!

### 7. Test the jail

Open your website on a computer other than the Ubuntu server where it is running. Log in correctly to see that works. Then use bad passwords at least twice in 60 seconds. Your computer should get locked out.

You can check how many people are banned by a specific jail with:

```bash
sudo fail2ban-client status http-401 # or any jail name
```

> If you want to unban an IP, use `sudo fail2ban-client set <JailName> unbanip <IP address>`

### 8. Create Two More Jails

Using the patterns from above, create two more jails. Consider what suspicious activity you might want to trigger a ban. Here are some ideas:

* Frequent 404 errors indicating an attempt to find hidden pages.
* Retrieval of `robots.txt` indicating that the request is coming from a indexing service (e.g. Google) or a web crawler of some sort.
* Limit the 401 filter you created to just the `/cgi-bin/login.cgi` page.
* Prohibit certain browsers.

## Writeup and Submission

To complete this lab, write up what you did and what you observed and submit it **in PDF format** to LearningSuite.
You may use the word processing or writing software of your choice so long as it can produce a **PDF** (Markdown, Google Docs, LibreOffice, LaTeX, Microsoft Word, etc.).

Please follow this outline to show evidence of your work:
* [2 Points] Name, Date, and Lab Title
* [15 Points] Functioning Web Server
    * Could you access your web server from another system (the VM host or another computer on the LAN)? (yes/no)
    * Did login and logout work? (yes/no)
* [8 Points] UFW Installation and Configuration
    * Use `sudo ufw status verbose` to verify its function.
    * Is it working properly? (yes/no)
* [15 Points] Fail2Ban Installation and Configuration
    * Open `Developer Tools > Network` in your browser.
    * Do two bad logins in a row and then try to access any page in the site.
    * Capture a screenshot of the **Network** tab of in your browser showing the failed logins (with 401 errors) and then the blocked access that follows. Insert that screenshot into this part of your writeup.
* [20 Points] Two more Fail2Ban Jails
    * What did you block?
    * Paste in the regular expressions you used to detect those patterns.

Upload your **PDF** writeup to LearningSuite.
