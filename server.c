#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 8081

struct data{
	int a;
	float f;
	char c;
};

struct data d1;

void func(int sockfd)
{
		char option;

		// Read the struct from client.
		read(sockfd, &d1, sizeof(struct data));

		// Print structure values received from the client.
		printf("Before : %d\t, %f\t %c \n\n", d1.a, d1.f, d1.c);
		printf("Press any key to continue and 'e' to exit ");

		if (option = getchar() == 'e')
			exit(0);

		d1.a = 2 * d1.a;
		d1.f = d1.f + 1;
		d1.c++;

		printf("After : %d\t, %f\t %c \n", d1.a, d1.f, d1.c);
		write(sockfd, &d1, sizeof(struct data));

		if (option == 'e') {
			printf("Exiting from server !\n");
			exit(0);
		}
}

int main()
{
	int sock_fd, conn_fd, len;
	struct sockaddr_in server_addr, cli;

	// creating and verifying socket
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
		printf("socket creation failed !!\n");
		exit(-1);
	}
	else
		printf("Socket created !\n");

	bzero(&server_addr, sizeof(server_addr));

	// assign IP_ADDR and PORT
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	// Bind newly created socket to IP.
	if ((	bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))) != 0) {
		printf("bind failed !\n");
		exit(0);
	}
	else
		printf("bind successfull !\n");

	if ((listen(sock_fd, 1)) != 0) {
		printf("Listen failed !\n");
		exit(-1);
	}
	else
		printf("Server listening..\n");

	len = sizeof(cli);

	// Accept the data from client.
	conn_fd = accept(sock_fd, (struct sockaddr *)&cli, &len);
	if (conn_fd < 0) {
		printf("Accept failed !\n");
		exit(0);
	}
	else
		printf("Connection accepted\n");

	// Function passing data between client and server
	func(conn_fd);

	// close the socket
	close(sock_fd);
}

