# Group Chat Application Layer Protocol (V4)

This document describes the custom application layer protocol and data management system for the connectionless Group Chat application.

## 1. Communication Protocol

The protocol is a **stateless, request-response** model built on top of **UDP**. The application keeps the same logical features as V3, but the transport is connectionless, so each request is carried in its own datagram and processed completely before the server moves to the next one.

### 1.1 Datagram Layer
Because UDP preserves message boundaries, length-prefixed framing has been removed. Data is sent directly as ASCII/UTF-8 strings via `sendto` and `recvfrom`.

### 1.2 Control Flow
- **Client Transmission:** The client manages the UI locally and sends formatted command strings to the server.
- **Iterative Server Algorithm:** The server follows the connectionless iterative model from the lecture pattern:
  `Create socket -> Bind socket -> Repeat: Read request -> Process request -> Formulate response -> Send reply`.
- **Server Statelessness:** The server does not keep an open session per client. Every datagram contains the context needed for that operation, such as username, group name, or message content.
- **Response Model:** Each request receives a single ACK, ERR, or data reply datagram.

### 1.3 Operation Format
Requests use a pipe-delimited format:

`OpCode|Field1|Field2|...`

Current operation set:
- `1|Username|Password` -> Register user
- `2|Username|Password` -> Login user
- `3|Username|GroupName` -> Create group
- `4|Username|GroupName` -> Join group
- `5|Username` -> Show joined groups
- `6|Username|GroupName` -> Show group members
- `7|Username|GroupName` -> Delete group
- `8|Username|GroupName` -> Leave group
- `9|Username|GroupName` -> Fetch chat history
- `10|Username|GroupName|ParentId|MessageContent` -> Send or reply to a message
- `11|Username` -> Logout

## 2. Data Storage & Formats

The application uses flat text files for persistent storage. Data is generally formatted with a pipe (`|`) delimiter.

### 2.1 Standardized File Format (ID-Prepend)
Most files are managed via `file_io.c`, which automatically calculates a line-based ID and prepends it to the record.

| File | Format | Source | Destination |
| :--- | :--- | :--- | :--- |
| `users.txt` | `ID | Username | HashedPassword (Decimal)` | `registration.c` | `authentication.c` |
| `groups.txt` | `ID | AdminUsername | GroupName` | `registration.c` | `group_interaction.c` |
| `memberships.txt` | `ID | Username | GroupName` | `group_interaction.c` | `group_interaction.c` |

### 2.2 Messaging Schema
The `messages.txt` file uses a custom schema managed directly in `messaging.c`.

**Format:** `MessageID|GroupID|Sender|Content|Timestamp|ParentMsgID`

- **MessageID:** A random numeric ID for the message.
- **GroupID:** The name of the group where the message was sent.
- **Sender:** The username of the author.
- **Content:** The text message.
- **Timestamp:** Unix epoch time.
- **ParentMsgID:** The ID of the message being replied to (0 for new threads).

## 3. Security & Logic

### 3.1 Authentication
Passwords are never stored in plain text. They are hashed using the **djb2** algorithm (found in `registration.c`) and stored in **decimal** format (`%lu`).

### 3.2 Authorization
- **Admins:** The user who creates a group is recorded as the admin in `groups.txt`.
- **Permissions:** Only admins can delete groups or view the full member list of a group.

### 3.3 File Interaction Logic
- **Writing:** `write_to_file` appends data and handles ID generation.
- **Reading:** `read_from_file` loads entire files for processing.
- **Searching:** `search_in_file` and `search_string` are used for verification (e.g., checking if a user exists).
- **Deletion:** `delete_all_lines_containing` handles scrubbing records (e.g., when a user leaves a group).
