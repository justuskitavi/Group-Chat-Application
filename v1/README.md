# C Group Chat Application

A modular, file-based group chat system implemented in C. This application allows users to register, authenticate, manage groups, and participate in group discussions.

## Features

### User Management

- **Registration:** Create a new account with a username and a hashed password (using the djb2 algorithm).
- **Authentication:** Secure login system that verifies credentials against stored data.

### Group Management

- **Create Group:** Users can create new groups, automatically becoming the group administrator.
- **Join Group:** Users can join existing groups to participate in conversations.
- **Leave Group:** Members can opt to leave a group at any time.
- **Delete Group:** Group administrators have the authority to delete a group, which also clears all associated memberships.
- **View Memberships:**
  - List all groups a specific user has joined.
  - List all members within a specific group.

### Messaging

- **Interactive Chat:** Enter a group's chat session to read and send messages in real-time.
- **Message Persistence:** All messages are saved with sender information and group context.

## File Structure

- `main.c`: The entry point of the application, containing the main menu and dashboard logic.
- `authentication.c/h`: Handles user login and credential verification.
- `registration.c/h`: Manages user signup and group creation/deletion.
- `group_interaction.c/h`: Provides functionality for joining, leaving, and viewing groups/members.
- `messaging.c/h`: Implements the core chat logic and message handling.
- **Data Stores (.txt):**
  - `users.txt`: Stores user credentials (hashed passwords).
  - `groups.txt`: Stores group names and their administrators.
  - `memberships.txt`: Maps users to the groups they belong to.
  - `messages.txt`: Stores the history of group messages.

## Getting Started

### Prerequisites

- A C compiler (e.g., GCC, Clang, or MSVC).

### Compilation

You can compile the project using GCC with the following command:

```bash
gcc main.c authentication.c group_interaction.c messaging.c registration.c -o group_chat
```

### Running the Application

After compilation, run the executable:

```bash
./group_chat
```

## Usage

1. **Register:** Start by creating a new user account.
2. **Login:** Access the group dashboard by logging in.
3. **Manage Groups:** Create a group or join an existing one.
4. **Chat:** Select "Enter Chat" and provide a group name to start messaging.
