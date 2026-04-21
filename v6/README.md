# Group Chat System (V6)

A concurrent, multi-user group chat application built in C using TCP sockets and multi-processing. This version implements a process-per-client model for enhanced isolation.

## 1. Project Overview

The Group Chat System allows users to register, log in, create groups, join existing groups, and participate in real-time chat sessions. It features persistent storage for users, groups, memberships, and messages.

### Core Architecture
- **Protocol:** Custom application-layer protocol with length-prefixed framing.
- **Concurrency:** Multi-process server using `fork()` to handle each client connection in a separate child process.
- **Persistence:** Flat-file database system using `.txt` files with pipe-delimited records.
- **Security:** Password protection via `djb2` hashing.

## 2. Project Structure

| Component | Responsibility | Files |
| :--- | :--- | :--- |
| **Server** | Main entry, listener, and connection handling. | `server.c`, `server.h`, `server_stub.c` |
| **Client** | User interface relay and terminal handling. | `client.c`, `client.h`, `client_stub.c` |
| **Protocol** | Framing and data transmission logic. | `data_transfer.c`, `data_transfer.h` |
| **Auth/Reg** | User signup, login, and password hashing. | `authentication.c`, `registration.c` |
| **Groups** | Group creation, joining, and membership. | `group_dashboard.c`, `group_interaction.c` |
| **Messaging** | Chat room logic, message storage, and history. | `messaging.c`, `messaging.h` |
| **Storage** | Low-level file I/O and ID management. | `file_io.c`, `file_io.h` |

## 3. Features

- **User Management:** Secure registration and authentication.
- **Group Dashboards:** Navigate through personal groups and public groups.
- **Admin Control:** The group creator (admin) has exclusive rights to delete the group or view member lists.
- **Real-time Chat:** Send messages, view history, and use the **Reply** feature to respond to specific messages by ID.
- **Graceful Shutdown:** Handles client disconnections and cleans up resources.

## 4. Getting Started

### Prerequisites
- GCC Compiler
- Linux/WSL Environment (uses `netinet`)

### Compilation
Use the provided `makefile` to build the project:

```bash
# Build both server and client
make whole

# Or build individually
make server
make client
```

### Running the Application
1. **Start the Server:**
   ```bash
   ./server
   ```
2. **Start the Client (in a separate terminal):**
   ```bash
   ./client
   ```

## 5. Protocol Details
For a deep dive into how the client and server communicate, refer to [APPLICATION_LAYER.md](./APPLICATION_LAYER.md).
