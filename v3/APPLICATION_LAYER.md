# Group Chat Application Layer Protocol (V3)

This document describes the custom application layer protocol and data management system for the Group Chat application.

## 1. Communication Protocol

The protocol is a **synchronous, server-driven, request-response** model built on top of **TCP**. The server manages the application state and renders the UI, while the client serves as a relay for display and user input.

### 1.1 Framing Layer
To handle TCP's stream-oriented nature, the protocol uses **length-prefixed framing**.

| Field | Size | Type | Description |
| :--- | :--- | :--- | :--- |
| **Header** | 4 Bytes | `uint32_t` | Message length in bytes (Network Byte Order). |
| **Payload** | Variable | `char[]` | The actual message content (ASCII/UTF-8 string). |

### 1.2 Control Flow
- **Server Transmission:** The server sends UI elements, prompts, or logs.
- **Input Sentinel:** When the server requires input, it sends `<<INPUT_READY>>`.
- **Client Response:** Upon receiving the sentinel, the client waits for user input and sends it back as a length-prefixed message.

## 2. Data Storage & Formats

The application uses flat text files for persistent storage. Data is generally formatted with a pipe (`|`) delimiter.

### 2.1 Standardized File Format (ID-Prepend)
Most files are managed via `file_io.c`, which automatically calculates a line-based ID and prepends it to the record.

| File | Format | Source | Destination |
| :--- | :--- | :--- | :--- |
| `users.txt` | `ID | Username | HashedPassword` | `registration.c` | `authentication.c` |
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
Passwords are never stored in plain text. They are hashed using the **djb2** algorithm (found in `registration.c`).

### 3.2 Authorization
- **Admins:** The user who creates a group is recorded as the admin in `groups.txt`.
- **Permissions:** Only admins can delete groups or view the full member list of a group.

### 3.3 File Interaction Logic
- **Writing:** `write_to_file` appends data and handles ID generation.
- **Reading:** `read_from_file` loads entire files for processing.
- **Searching:** `search_in_file` and `search_string` are used for verification (e.g., checking if a user exists).
- **Deletion:** `delete_all_lines_containing` handles scrubbing records (e.g., when a user leaves a group).
