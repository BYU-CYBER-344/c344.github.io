---
title: "Lab 5: File Sharing and Race Conditions"
---

**Scenario:** You are creating an application that needs to write event records to a log file. Each log entry is one line of text. There may be multiple instances of your application running at the same time so you need to ensure that multiple accesses to the file from different processes do not interfere with each other.