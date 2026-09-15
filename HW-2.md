---
title: "HW 2: Dockerhub"
---
In class, we took a simple website, hosted it using a container, then created a new container image that combined the server with the site, and then saved that image. The first part of this homework is to reproduce those steps on your own. The second part is to experiment with a few prebuilt container images from DockerHub.

Before commencing this homework, jump to the bottom and look at the submission instructions so that you know what information to collect.

## Part 1: Container Walkthrough (from class)

In this walkthrough, you will unpack a sample website and do the following:
* Serve it using a standard Docker image.
* Create a `docker-compose.yml` to serve the sample website.
* Open the docker container interactively to see how it works.
* Create a `Dockerfile` and bundle the server and the website into one container.
* Export the container so that it could be distributed.

### Unpack the website.

(This part is just a prerequisite for the container-related stuff.)

1. Create a directory for this walkthrough. In these instructions we will call the directory `walkthrough-hw2`.
2. Create a subdirectory: `walkthrough-hw2/wwwroot`
3. Unpack the sample website, <a href="/data/samplesite.zip" download="samplesite.zip">samplesite.zip</a> into `wwwroot`.

### Serve the website using the `httpd` Docker image.

[Docker Hub](https://hub.docker.com/) includes a set of [official images](https://hub.docker.com/u/library) which are the canonical references for certain packages. [httpd](https://hub.docker.com/_/httpd) is the canonical image for the Apache HTTP Server. You can launch any Docker Hub image with a simple `docker run` command.

1. Open a terminal and change directories to your `walkthrough-hw2` directory (or whatever you called it).
2. Execute this command:
```sh
docker run --rm -p 8080:80 -v ./wwwroot:/usr/local/apache2/htdocs/ httpd:latest
```
3. Browse to [http://localhost:8080](http://localhost:8080) and view your site.
4. To exit the server, press `Ctrl-C`

Here is what the options in the `docker run` command do:

* `--rm` : delete the container when it exists. Otherwise you can build up a collection of formerly-run images.
* `-p 8080:80` : Map port 8080 on the local computer to port 80 on the container.
* `-v ./wwwroot:/usr/local/apache2/htdocs/` : Map the `./wwwroot` directory on the local computer to the `/usr/local/apache2/htdocs/` on the container. That path is where the `httpd` container places the web site to be hosted.
* `http:latest` : Download and run the latest version of the `httpd` image from Docker Hub.

> You can give a name to your container using the `--name` argument. If don't, Docker will create one, (e.g. `fluffy_panda`). You can find the names of existing containers from the **Docker Desktop App** or by typing `docker ps -a`. If you omit the `--rm` argument then the container will remain on your system, taking up disk space, until you delete it. You can restart an existing container using `docker start <name>`
> You might experiment with creating several containers, starting and stopping them, and then cleaning them up with `docker container prune`

Here are some other commonly-used options:
*	`-d` : Detach the container from the terminal and run in the background.
* `-i` : *Interactive* - Allows you to send input to applications in the container.
* `-t` : TTY - Tells applications that there is a terminal attached to the output (whether the console is connected or not) so that it can send friendly output.

> Single-letter options can be combined. `-it` means the same thing as `-i -t`. These are frequently used together for interactive connections. Likewise, `-dit` is sometimes used to create a detached container to which you will later connect a terminal using `docker attach <container-name>`.

### Use Docker-Compose to serve the website

If you regularly start the same container, you could put the `docker run` command in a [shell script](https://www.w3schools.com/bash/bash_syntax.php) or create an [Bash alias](https://www.w3schools.com/bash/bash_alias.php). Creating a `docker-compose.yml` file is another option with certain advantages.

1. Create file called **docker-compose.yml** with the following contents:

<div style="font-weight: bold;">docker-compose.yml</div>
```yaml
services:
  web:
    image: httpd:latest
    ports:
      - 8080:80
    volumes:
      - ./wwwroot:/usr/local/apache2/htdocs
```

This file does the same thing as the `docker run` command from before. The `image` settings indicates which image to run, the `ports` setting maps port 8080 on the computer to port 80 in the container, and the `volumes` setting maps `./wwwroot` on the computer to `/usr/local/apache2/htdocs` on the container.

> The `ports` and `volumes` settings have a newline and a dash before the value. That is [YAML](https://yaml.com/resources/cheatsheet/) syntax for a *list* (also known as a *sequence* or *array*). In this case each list has just one entry but you could add more port and volume mappings. YAML has the same logical data model as JSON. That is, you can convert data directly between JSON and YAML. YAML 3 allows you to mix JSON and YAML syntax. So, the above YAML file could be written like this and it would work just as well.

<div style="font-weight: bold;">Alternative docker-compose.yml</div>
```json
{
  "services": {
    "web": {
      "image": "httpd:latest",
      "ports": ["8080:80"],
      "volumes": ["./wwwroot:/usr/local/apache2/htdocs"]
    }
  }
}
```

{: start="2"}
2. Start the container:
```sh
docker compose up
```
3. Browse to the website: `http://localhost:8080
4. Stop the container: Press `Ctrl-C` to exit the server. Then enter `docker compose down` to close and exit the container.

> The `-d` option on `docker compose up` will run the container in detached mode and return you to the command prompt, just as the `-d` option does with `docker run`. Adding `-v` to `docker compose down` will cause it to delete any volumes that were created for any of the containers thereby erasing your data.
> In addition to preserving a set of container configuration options, `docker compose` also has the advantage that it can launch a coordinated set of containers all at once. Those who took **CYBER 210** may recall a lab that used a `docker-compose.yml` file to launch a web server, a database server, and a database administration console.

### Examine the httpd container image

Suppose you intend to deploy the website on a cloud service that auto-scales by adding container instances. You would want to create a new container image that includes the web site contents. Conveniently, most container images are created by adding to or modifying an existing image; you don't have to start from scratch.

> Of course, for a static web site there are simpler options than creating a custom container image. But this can be quite valuable when you need to deploy a custom back end.

First, let's examine the **httpd** container for some details. From its [Docker Hub Page](https://hub.docker.com/_/httpd) we know that the web site is served from `/usr/local/apache2/htdocs/` and that the configuration file is located at `/usr/local/apache2/conf/httpd.conf`.

1. Use the following command to extract metadata from the container image.

```sh
docker image inspect httpd:latest > httpd.json
```

Among the data is `"cmd": ["httpd-foreground"]` which is the default command to be run when launching the container. That command causes the Apache web server to be run in foreground mode.

When you execute `docker run` you can include a command to be run. Doing so overrides the default command. In our case, we want to run the bash shell in interactive mode so that we can examine the contents of the container.

{: start="2"}
2. Enter the following commands:


```sh
docker run --rm -it httpd:latest bash
ls htdocs
cat htdocs/index.html
exit
```

This lists the contents of the web site, which consists exclusively of `index.html`. Then it writes out the contents of `index.html` which is just a very simple placeholder page. So far, we have substituted a local directory using Docker volume settings so you should never have seen that placeholder.

> Before entering the `exit` command, you could also view the Apache configuration file with this command: `cat conf/httpd.conf`. (It's a long file with lots of comments.)

### Create a new image using Dockerfile

With this information, we can create a new image. In this case, we will substitute the new website for the placeholder page that is in the **httpd** image.

1. Create a file called **my-site.Dockerfile** with these contents.

<div style="font-weight: bold;">my-site.Dockerfile</div>
```Dockerfile
FROM httpd:latest
COPY ./wwwroot/ /usr/local/apache2/htdocs/
```

* The `FROM` command indicates that the new image will be build from `httpd:latest`
* The `COPY` command overwrites the website directory in the source image with the new site.

{: start="2"}
2. Create a new image with the following command (*don't miss the dot at the end*):

```sh
docker build -t my-site -f my-site.Dockerfile .
```

* The `-t` option assigns the `my-site` tag to the image. Containers have names, container images have tags.
* The `-f` option indicate the *Dockerfile* that will build the image. The default is simply `Dockerfile` with no extension.

{: start="3"}
3. Launch the new image:

```sh
docker run --rm -p 8080:80 my-site
```

{: start="4"}

4. Browse to `http://localhost:8080` and view the site served from the new container.
5. Exit the container with `Ctrl-C`

### Export the container image

Your new container image, `my-site`, is managed by your local Docker environment. To transfer it to other systems you must export it to a [.tar](https://www.w3schools.com/bash/bash_tar.php) file;

1. Export the image:
```sh
docker save -o my-site.tar my-site
```

That's all we expect you to do for this homework. Here are the commands for related tasks:

* Load the image onto another Docker instance

```sh
docker load -i my-site.tar
```

* Post the image on Docker Hub<br/>(This command requires you to log into Docker Hub before it will function)

```sh
docker push <dockerhub-username>/<repository-name>: my-site
```

## Part 2: Container Exploration

[Docker Hub](https://hub.docker.com/) is huge, with more than 13 million images. It includes tools like `httpd`, basic images to serve as the foundation for custom images, database servers, games, AI models, and much more.

Search the containers available on Docker Hub and run at least three of them. If the built-in search is not helpful, you might query your favorite AI or do a general web search. As you do so, you may share your experiences and recommendations on Discord (though that is not required). You will have a better experience if you choose containers that stand alone -- not requiring configuration as did `httpd`.

As you explore, consider these collections which are listed on the [Docker Hub home page](https://hub.docker.com/), left column. However, the images you actually choose to run may come from anywhere.

* Docker Hardened Images
* Docker Official Images
* Verified Publisher
* Sponsored OSS

Also look at the variants that are published for certain images.

## Submission

To submit this homework assignment, open the Homework 2 assignment/exam in LearningSuite and **answer the questions there.**

For convenience here are what the questions will be:
* [5 Points] Did you launch a working website using `docker run` (yes/no)?
* [5 Points] Did you launch a working website using `docker compose` (yes/no)?
* [5 Points] Did you create a **custom container image** and launch it to create a working website (yes/no)?
* [5 Points] Paste in the token that appears at the bottom of the web page after you enter a name. This may be from any of the three launch methods.
* [10 Points] Name three Docker images that you launched from **Docker Hub**.
