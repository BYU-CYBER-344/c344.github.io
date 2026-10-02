---
title: "Lab 3: System Hardening"
---

Under construction!

```sh
sudo apt update
sudo apt install apache2
sudo systemctl enable --now apache2
```

Copy over the source to /var/www/html/
Browse to the site

Enable CGI
```sh
sudo a2enmod cgid
sudo a2enconf serve-cgi-bin # Didn't seem necessary
```

Copy cgi to /usr/lib/cgi-bin

TODO:
Need to find out when it is a2enmod cgid or a2nmod cgi
On ubuntu it is cgid

