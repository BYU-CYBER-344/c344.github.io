---
title: Final Project
---

The final project is worth **270 points total** (27% of your grade). It is designed to evaluate your ability to work as part of a team, apply concepts learned in class, and tackle a challenging operating-systems-related problem.

**Breakdown of points:**

- Ungraded: Team Formation
- Ungraded: Subject Selection
- 100 Points: A Working Project
- 75 Points: Team Presentation
- 75 Points: Team Writeup
- 20 Points: Peer Evaluation

## Team Formation

You will form teams of **4 to 6 students**. Five is recommended. For the *Team Formation* assignment (not graded), you must report who is on your team. If you do not have a team, notify us and you will be assigned to an instructor-assembled team.

- Only **one team member** needs to submit the working project and documentation.
- The **presentation** will be graded by the professor.
- **Peer evaluations** must be submitted by each student individually.

## Project Components

### A Working Project (100 Points)

By the end, your project should be **functional and demonstrable**. It should serve a real purpose, align with your design goals, and show meaningful effort from the team.

### Team Presentation (75 Points)

Toward the end of the semester, your team will present your project. You are limited to 10 minutes of presentation and 2 minutes of Q&A.

The presentation should include:

* The problem or purpose of your project
* An overview of your design and decisions
* A live or recorded demonstration of your project deliverables

All team members should participate in the presentation.

### Writeup (75 Points)

Your writeup should tell the story of your project. It must include:

* The steps you took to complete your project
* Design decisions and justifications
* Problems or errors you encountered and how you resolved them
* Setup and usage instructions
* External tools/resources you used (with proper references)

If someone else read your documentation, they should be able to **recreate your project**.

### Peer Evaluation (20 Points)

At the conclusion of your project, each student must evaluate their teammates’ contributions. The composite score (from self and team feedback) will determine your grade for this section.

## Project Options

Choose **one** of the following projects (or propose your own). These are intentionally challenging. You will likely need to research online, experiment with existing tools, and problem-solve creatively. While you may seek advice from experts, **all work must be completed by your team**.

### Suggested Projects

#### Create Your Own Linux Distro

Use [Linux From Scratch](https://www.linuxfromscratch.org/) or a similar resource to create a custom Linux distribution.
- Define a **specific cybersecurity-related use case** (e.g., forensic analysis, penetration testing, hardened server, feature demonstration).
- Justify your design decisions.
- Produce an ISO that others can install and test.

Modifying an existing ISO is insufficient. The operating system must be made from scratch.

#### SOC Dashboard for Data Center Infrastructure

Build a web-based monitoring dashboard that includes:
- **Real-time data visualization**
    - Network traffic (possibly including geo maps of traffic sources)
    - Server status (CPU, RAM, disk, temperature, etc.)
    - Global data from public sources (weather, airline activity, seaport activity, )
- **Alerts for critical events including threat indicators**
- Collect data via standard protocols such as SNMP, IPMI, DRAC/iDRAC, UPNP, or other protocols.

#### Custom CLI Shell

Write your own CLI shell with features such as:

- Parsing and executing commands (fork and exec)
- Pipelining and redirection of STDIN and STDOUT
- Innovative security features not seen in existing shells such as managing privilege escalation.
- May or may not be based on an open-source shell such as BASH.

#### Hardened Server

Choose a base Linux or BSD distribution and prepare a server to operate in a hostile environment.
- Go well-beyond the Lab 3 hardening task.
- Include firewall, auditing, SELinux/AppArmor, intrusion detection, and logging etc.
- Provide a **hardening guide** and a before/after security comparison.
- Test it using penetration testing tools.

#### Kubernetes Cluster

Create a Kubernetes cluster and deploy a set of containers on the cluster.
- Define a specific (simulated) purpose for the cluster. (E.g. Hosting a CTF competition.)
- Include a control plane and at least two nodes.
- Demonstrate high-availability features such as failover or auto-restart.

#### Load-Balanced Web Servers

Create a cluster of web servers with load balancing.
- Choose and deploy a load balancing solution.
- Determine how to handle session state when a browser may migrate to a different server.
- Demonstrate robustness with a simulated server failure.

### Other Projects

If you would like to propose a different project, it must be approved by the professor. Your idea should:
- Be directly related to **operating systems**
- Address security and/or privacy concerns.
- Be of similar **scope and difficulty** as the projects above