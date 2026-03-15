# C Compilation Flow

```bash
gcc -Wall -Wextra -std=c11 \
	main.c file_io.c \
	v1/authentication.c v1/registration.c v1/group_interaction.c v1/messaging.c \
	-o group_chat

./group_chat
```
