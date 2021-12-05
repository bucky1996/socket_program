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

void func(int sockfd1, int sock_fd2)
{
		char option;

		// Read the struct from client.
		read(sockfd1, &d1, sizeof(struct data));

		// Print structure values received from the client.
 		printf("Received data from client1\n");
		printf("Before : %d,\t%f,\t%c \n", d1.a, d1.f, d1.c);

		//Modifying received data from the client1
		d1.a = 2 * d1.a;
		d1.f = d1.f + 1;
  		if(d1.c == 'z'){
			d1.c = 'a';
		}
		else{
			d1.c++;
		}

		//Printing new data	
		printf("After : %d,\t%f,\t%c \n", d1.a, d1.f, d1.c);

		printf("Press any key to send data to client2 OR 'e' to exit: ");

		if ((option = getchar() ) == 'e') {
			printf("Exiting from server !\n");
			exit(0);
		}else{
			write(sock_fd2, &d1, sizeof(struct data));
		}


}

int main()
{
	int sock_fd, conn_fd1, conn_fd2, len;
	struct sockaddr_in server_addr, cli1, cli2;

	// creating and verifying socket
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
		printf("Socket creation failed !!\n");
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
		printf("Bind successfull !\n");

	if ((listen(sock_fd, 1)) != 0) {
		printf("Listen failed !\n");
		exit(-1);
	}
	else
		printf("Server listening..\n");

	len = sizeof(cli1);

	// Accept the connection req from clients.
	conn_fd1 = accept(sock_fd, (struct sockaddr *)&cli1, &len);
	if (conn_fd1 < 0) {
		printf("Accept failed !\n");
		exit(0);
	}
	else
		printf("Connection accepted from client1 \n");

	conn_fd2 = accept(sock_fd, (struct sockaddr *)&cli2, &len);
	if (conn_fd2 < 0) {
		printf("Accept failed !\n");
		exit(0);
	}
	else
		printf("Connection accepted from client2 \n");

	// Function passing data between client and server
	func(conn_fd1, conn_fd2);

	// close the socket
	close(sock_fd);
}

