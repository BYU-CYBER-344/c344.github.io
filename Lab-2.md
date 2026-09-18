---
title: "Lab 2: Containers"
---

In this lab, you will use containers to build and host a static website.

[Jekyll](https://jekyllrb.com/) is MarkDown-native, blog-aware, static website generator. It is the native environment for [GitHub Pages](https://pages.github.com) and a popular way to create websites of all sorts. Trouble is, Jekyll is written in [Ruby](https://www.ruby-lang.org/). Unless you are using Ruby for other stuff, getting it installed and configured to run Jekyll is a time-consuming and frustrating experience. Besides, it clutters your computer with a bunch of extra stuff.

This is just the kind of problem that containers were designed to handle.

In this lab you will do two tasks:

1. Use the [official Jekyll container]() to test and build a website from the Jekyll source files.
2. Use the [httpd Apache container]() to host the website you just built.

In doing so, you will depend on the container commands you learned in [Homework 2](HW-2).

### Resources and References

Unlike the Homework 2, we are not giving you step-by-step instructions or CLI. We intend for you to look up the references and figure out the tasks. Nevertheless, check the tips on each step to avoid pitfalls.

Here are the resources and references you will need:

* [Homework 2](HW-2)<br/>(Use this as a reference for Docker commands.)
* [BYU CYBER 344 Website Source Repo](https://github.com/BYU-CYBER-344/c344.github.io)
* [Jekyll Official Documentation](https://jekyllrb.com/docs/)
* [Jekyll Container Image on Docker Hub](https://hub.docker.com/r/jekyll/jekyll)
* [Jekyll Docker Readme on GitHub](https://github.com/envygeeks/jekyll-docker/blob/master/README.md)<br/>Be sure to check out the **Usage** section.
* [httpd Apache HTTP Server Image on Docker Hub](https://hub.docker.com/_/httpd)<br/>(This is the same one we used in **Homework 2**).
* [Dockerfile overview](https://docs.docker.com/build/concepts/dockerfile/)

## Part 1: Test and Build the CYBER 344 web site from Jekyll sources

The [BYU CYBER 344](https://c344.byucyber.net/map) website, *including these lab instructions* is written in MarkDown/Jekyll and hosted on GitHub Pages. GitHub will host a pages website for free if the source code is kept public. You will build and host a copy of the class website.

1. Get a copy of the web site source by cloning the [repo](https://github.com/BYU-CYBER-344/c344.github.io) to a directory to which your container solution has access.
2. Serve the site locally using `docker run` with the the `jekyll serve` command as described in the **Jekyll Docker Readme**.
    * Also see the tips below.
3. With the site running locally, start with the **/map** page and browse the website to see that it is all working.
    * The map page should be at [http://localhost:4000/map](http://localhost:4000/map)
4. Experiment with one or two changes and see that they are reflected in the site.
5. Add a new file called `mylab.md`. In that file, put your name, the date, and any comments you want to make.
    * Because this is Jekyll, the MarkDown file must start with [front matter](https://jekyllrb.com/docs/front-matter/). That indicate to Jekyll that it should convert `mylab.md` file to `mylab.html`. Front matter starts with a `---` line and ends with another `---` line. See the other `.md` files in the source for examples.
6. Make sure your new `mylab` page is served.
    * It should appear at [http://localhost:8080/mylab](http://localhost:8080/mylab).
5. Exit the local server.
6. Build a the static version of the website using the `jekyll build` command as described in the **Jekyll Docker Readme**.
7. Review the generated site. It should appear in the `_site` subdirectory.

### Tips for Part 1

* Look in the [Jekyll Docker Readme on GitHub](https://github.com/envygeeks/jekyll-docker/blob/master/README.md) for details on how to serve the site locally and how to build the static site.
* The Jekyll Docker readme assumes a Bash shell. Hence, it uses `$PWD` to refer to the current working directory and backslashes to continue a command across multiple lines. If you are using the **Windows command prompt** or **Powershell** you need to adapt the commands from their examples. Use "." (period) for the current directory and either keep the whole command to one line or use the appropriate continuation characters for those shells.
* The Jekyll Docker readme uses the `--publish` and `--volume` and Docker options. Those are the verbose forms of `-p` and `-v` that we used in **Homework 2**.
* If you are serving your site from a Windows file system, the Jekyll `--force_polling` option is necessary to cause the site to auto-update when you make changes.
* When serving your site, the `--livereload` option will cause pages to auto-refresh if you make changes. Those updates are communicated through port 35729.

## Part 2: Create a new docker image that bundles httpd with the CYBER 344 site.

For this step, you should use the [Homework 2](HW-2) instructions as a reference.

1. Create a **Dockerfile** for your new image.
    * Put this in some directory **other** than the Jekyll source.
    * It should use `httpd:latest` as its `FROM` source.
    * The website contents should come from the `_site` you created at the end of Part 1
    * In the Apache configuration file, `httpd.conf` you must add the MultiViews option. (See below)
2. Build the new image. The **tag** for the image should be `c344`.
3. Run the image and browse the website. Make sure it works properly.
4. Export the image to a .tar file.
5. Submit the .tar file on LearningSuite for grading.

### Patching the httpd.conf file

When GitHub pages gets a bare name in the path like `/about`, it will return a found `about.html`. The class website depends on that substitution. To gain the equivalent behavior on **httpd** you need to make two changes to the `httpd.conf` Apache configuration file.

<div>Change this line:</div>
```
#LoadModule negotiation_module modules/mod_negotiation.so
```
<div>to this:</div>
```
LoadModule negotiation_module modules/mod_negotiation.so
```

<div>Also, change this line:</div>
```
    Options Indexes FollowSymLinks
```
<div>to this:</div>
```
    Options FollowSymLinks MultiViews
```

The patches to httpd.conf needs to happen in your Dockerfile. There are a couple of ways to accomplish that.

**Method 1: Export, edit, and replace the file.**

Export the existing `httpd.conf` file with the following command:
```sh
docker run --rm httpd:latest cat /usr/local/apache2/conf/httpd.conf > httpd.conf
```

Once it has been exported, you can edit the file and incorporate the new version by including a line like this in your `Dockerfile`:
```
COPY ./httpd.conf /usr/local/apache2/conf/httpd.conf
```

**Method 2: Use sed to patch the file in place**

The Dockerfile 'RUN' command can execute arbitrary Bash commands with the image. By using `RUN` with `set` and some appropriate regular expressions you can patch the file in place.

### Part 2 Tips

* Make sure that the tag of your new image is `c344`
* `docker build` can only reference files that are in the directory tree where it finds `Dockerfile`. So, to avoid including your `Dockerfile` and edited `httpd.conf` in your site, you need to either place those files in a parent directory of your repo (awkward) or copy the `_site` directory over to the directory where you will run `docker build`.
* If you choose to use the `RUN sed` option for patching `httpd.conf`, consider asking AI to help you compose the appropriate commands.
* After building your new image, you can use the following command to verify that your patches are in place: `docker run --rm c344 cat /usr/local/apache2/conf/httpd.conf`

## Submission and Grading

Submit this lab by uploading your container image `.tar` file to LearningSuite under the Lab 2 assignment.<br/>(Yes, the file will be big, in the neighborhood of 45MiB. But LearningSuite should accept it.)

### Scoring:
* [10 points] The tag of the image is `c344` (The filename can be anything with a '.tar' extension).
* [10 points] The image that loads and serves a web site.<br>(It should work with no more than `docker run --rm -p 8080:80 c344`)
* [20 points] The image is of the CYBER 344 web site.
* [10 points] You have added a `mylab.md` file to the source code which results in a `mylab.html` page in the image.
* [10 points] When browsing to `http://localhost:8080.mylab` it presents a web page with your name and the date you created that page.