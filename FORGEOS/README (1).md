# Linux Process Creation, Execution and Termination System

> **Operating Systems and Systems Programming (25CS2104E)** · 2026–27, Term-I
> Section 08 · Team 12

A C-based Linux systems programming project that demonstrates the complete lifecycle of a process — **creation → identification → execution → synchronization → signal control → termination → status reporting** — by calling real Linux/POSIX system calls directly. There is no GUI, web layer, or database: everything runs in the terminal and talks straight to the Linux kernel.

---

## Table of Contents

- [Overview](#overview)
- [Objectives](#objectives)
- [Features](#features)
- [OS Concepts and System Calls Used](#os-concepts-and-system-calls-used)
- [Methodology](#methodology)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Build and Run](#build-and-run)
- [Sample Output](#sample-output)
- [Testing](#testing)
- [Tools and Technologies](#tools-and-technologies)
- [Team and Contributions](#team-and-contributions)
- [Faculty](#faculty)

---

## Overview

Process management is a core topic in Operating Systems, but creation, execution, synchronization and termination are hard to grasp from theory alone. This project makes them observable. It creates parent and child processes, runs other programs inside children, synchronizes parent and child, sends signals, and reports every process ID, parent process ID, and termination status on the terminal.

It also includes dedicated demonstrations of **zombie** and **orphan** processes, and shows how concurrent processes behave.

## Objectives

1. Implement process creation and parent-child relationships using `fork()`, `getpid()` and `getppid()`.
2. Demonstrate program execution and synchronization using the `exec()` family with `wait()` and `waitpid()`.
3. Demonstrate termination and control mechanisms: `exit()`, signals, zombie processes and orphan processes.
4. Display and analyse process behaviour through terminal output and test cases.

## Features

- **Process creation** with `fork()` and display of parent-child relationships and hierarchy
- **Process identification** using `getpid()` and `getppid()`
- **Program execution** in child processes via the `exec()` family
- **Synchronization** with `wait()` and `waitpid()`, including collection of child exit status
- **Signal-based control** using `kill()` and signal handlers
- **Zombie process demonstration** (terminated child not yet reaped by its parent)
- **Orphan process demonstration** (parent terminates before the child)
- **Concurrent execution** of multiple processes
- **Status reporting** of PIDs, PPIDs, process states, and termination status
- **Makefile-based build** for one-command compilation

## OS Concepts and System Calls Used

| Concept / System Call | Purpose in the Project |
| --- | --- |
| `fork()` | Creates a new child process from the parent |
| `exec()` family | Replaces the child's process image and runs another program |
| `wait()` | Parent waits for a child to terminate |
| `waitpid()` | Parent waits for, and monitors, a specific child |
| `exit()` | Terminates a process and provides an exit status |
| `getpid()` | Returns the current process ID |
| `getppid()` | Returns the parent process ID |
| `kill()` | Sends a signal to a process |
| Signals | Process control and termination |
| Parent-child relationship | Process hierarchy created with `fork()` |
| Process synchronization | Coordination between parent and child |
| Zombie process | Terminated child whose status has not been collected |
| Orphan process | Child whose parent terminated first |
| Concurrent execution | Multiple processes running independently |

## Methodology

```
Process Creation → Process Identification → Program Execution → Synchronization
        → Signal / Process Control → Process Termination → Status Reporting
```

1. The parent process creates one or more children with `fork()`.
2. PIDs and PPIDs are printed using `getpid()` and `getppid()` to show the hierarchy.
3. Each child uses an `exec()` call to replace its image with a chosen program, while the parent continues or waits.
4. The parent synchronizes with `wait()` / `waitpid()` and reads the child's termination status.
5. Termination is demonstrated with `exit()`, and control with `kill()` and signal handlers.
6. Extra scenarios demonstrate zombie and orphan processes.
7. Every step is run in the Linux terminal and its output recorded as evidence.

## Project Structure

> Adjust file names below to match the actual contents of the repository.

```
CherryPie_OSSP/
├── src/
│   ├── process_creation.c     # fork(), getpid(), getppid(), parent-child hierarchy
│   ├── process_exec.c         # exec() family, wait(), waitpid()
│   ├── process_termination.c  # exit(), kill(), signal handling
│   ├── zombie_orphan.c        # zombie and orphan process demonstrations
│   └── main.c                 # entry point / menu (if applicable)
├── tests/                     # test cases and expected behaviour
├── screenshots/               # terminal output evidence
├── Makefile
└── README.md
```

## Prerequisites

- Linux (developed and tested on Ubuntu)
- GCC (`gcc`)
- GNU Make (`make`)
- Git

Install on Ubuntu/Debian:

```bash
sudo apt update
sudo apt install build-essential git
```

## Build and Run

**Clone the repository**

```bash
git clone https://github.com/fathimaqhibtiyakhader/CherryPie_OSSP.git
cd CherryPie_OSSP
```

**Build with Make**

```bash
make
```

**Or compile manually with GCC**

```bash
gcc -Wall -Wextra -o process_demo src/*.c
```

**Run**

```bash
./process_demo
```

**Clean build artifacts**

```bash
make clean
```

> Replace `process_demo` with the actual executable name defined in your Makefile.

## Sample Output

Illustrative example of the kind of output the program produces (actual PIDs and text will differ):

```
[Parent] PID = 4021, PPID = 3890
[Parent] Creating child process using fork()...
[Child]  PID = 4022, PPID = 4021
[Child]  Executing program using exec()...
[Parent] Waiting for child (PID 4022) using waitpid()...
[Parent] Child terminated normally with exit status 0
```

To observe a zombie process while the demo is running, open a second terminal and run:

```bash
ps -eo pid,ppid,stat,cmd | grep defunct
```

A process with state `Z` (defunct) is a zombie.

## Testing

Test cases verify each process operation:

| Test Scenario | Expected Behaviour |
| --- | --- |
| Process creation | Child is created; parent and child print distinct PIDs |
| Parent-child relationship | Child's PPID equals the parent's PID |
| `exec()` in child | Child image is replaced and the target program runs |
| `wait()` / `waitpid()` | Parent blocks until the child ends and reports its status |
| `exit()` with status code | Parent receives the correct exit status |
| `kill()` / signal handling | Target process receives the signal and responds as expected |
| Zombie process | Terminated child appears as `Z` until reaped |
| Orphan process | Orphaned child is re-parented (typically to `init`/`systemd`) |

Terminal outputs and screenshots for each test are stored in the `screenshots/` folder.

## Tools and Technologies

| Tool | Purpose |
| --- | --- |
| Linux / Ubuntu | Execution environment |
| C | Core programming language |
| GCC | Compiler |
| POSIX APIs | Process creation, execution, synchronization, termination |
| GNU Make | Automated builds via Makefile |
| Git and GitHub | Version control and collaboration |
| VS Code | Code editing |

## Team and Contributions

**Section 08 · Team 12**

| Roll Number | Name | Responsibility |
| --- | --- | --- |
| 2520030155 | Fathima Qhibtiya Khader | Process creation using `fork()`, process identification, parent-child process management |
| 2520030039 | TSSS Ananya | Program execution with `exec()`, synchronization with `wait()` / `waitpid()` |
| 2520030171 | P. Anuradha Nandini | Process termination, signal handling, zombie and orphan process demonstrations |
| 2520030388 | B. Pemesh | Testing, debugging, documentation, output analysis, screenshots, GitHub management |

## Faculty

**V. Muniraju Naidu**

---

*Developed as part of the Operating Systems and Systems Programming course project (25CS2104E), 2026–27, Term-I.*
