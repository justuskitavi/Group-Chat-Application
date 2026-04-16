# Commands made for linux or WSL terminal

# Compiles the server executable
# make server
server:
	gcc server.c server_stub.c data_transfer.c authentication.c group_dashboard.c messaging.c registration.c file_io.c group_interaction.c -o server

# Compiles the client executable
# make client
client:
	gcc client_stub.c client.c data_transfer.c -o client

# Deletes the client and server executables. Use when recompiling to get fresh executables
# make clean
clean:
	rm -f client server

# Performs the whole process of cleaning and compilation of files and clears the terminal
# make whole
whole:
	make clean ; make server ; make client ; clear