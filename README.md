# Modular Group Chat System in C

This project is an evolving implementation of a multi-user group chat system, designed to demonstrate various architectural patterns in C—from standalone modular applications to networked client-server models using both connection-oriented (TCP) and connectionless (UDP) protocols.

## Project Overview

The system allows users to register, authenticate, manage groups, and participate in real-time chat sessions. It uses a flat-file database approach for persistent storage, ensuring that users, groups, and message history are preserved across sessions.

### Key Features
- **User Management:** Secure registration and authentication with hashed passwords (djb2).
- **Group Dashboard:** Create, join, leave, or delete groups.
- **Messaging:** Real-time interactive chat with message persistence and a reply-by-ID feature.
- **Modular Design:** Clear separation of concerns between authentication, registration, group logic, and data persistence.

---

## Versions Evolution

### V1: Standalone Application
The foundational version where all modules reside within a single executable. It provides the core logic for user and group management using local file I/O without any networking components.

### V2: Client-Server Architecture (Local)
Separates the application into a **Client** (UI/Terminal) and a **Server** (Logic/Storage). Both components run on the same machine, establishing the framework for networked communication.

### V3: Iterative TCP Server
An iterative, connection-oriented version using **TCP sockets**. The server handles one client request at a time in a sequential loop, ensuring reliable delivery of messages.

### V4: Iterative UDP Server
The connectionless counterpart to V3, utilizing **UDP datagrams**. It follows an iterative model where the server processes individual datagrams, demonstrating a lightweight but less reliable communication alternative.

---

## Future Roadmap

The project will continue to expand into advanced concurrency models and networking patterns. Future versions will include:

- **Concurrent Architectures:**
  - **Thread-based:** Utilizing `pthreads` to handle multiple simultaneous client connections.
  - **Process-based:** Using `fork()` and IPC for isolated client handling.
  - **AsyncIO/Event-driven:** Implementing non-blocking I/O and multiplexing (e.g., `select`, `poll`, or `epoll`).
- **Protocol Variants:**
  - Each concurrent implementation will be provided in both **Connectionless (UDP)** and **Connection-oriented (TCP)** versions.

---

## Technical Stack
- **Language:** C
- **Networking:** BSD Sockets (TCP/UDP)
- **Storage:** Pipe-delimited flat-file database (.txt)
- **Security:** djb2 Hashing Algorithm
- **Platform:** Linux / POSIX-compliant environments

## How to Run
Each version contains its own `makefile` or compilation instructions. Navigate to a specific version directory (e.g., `cd v4`) and refer to the local `README.md` for specific build and execution commands.
