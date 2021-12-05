#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <netdb.h>


#define IP_ADDR "127.0.0.1"
#define PORT 8081
#define hostname "google.com"

struct data{
	int a;
	float f;
	char c;
};

struct data d2;
struct hostent *ip;

void func(int sock)
{
	   	char option;

		printf("Press any key to send data to server OR \n Press 'e' to exit : ");

		if ((option = getchar()) == 'e')
			exit(0);

 		d2.a = 11;
		d2.f = 2.3;
		d2.c = 'q';

		printf("Passing values to Server : %d\t %f\t %c\n", d2.a, d2.f, d2.c);


		write(sock, &d2, sizeof(struct data));

		
		//bzero(&d2, sizeof(struct data));
		//read(sock, &d2, sizeof(struct data));

		//printf("From Server : %d\t %f\t %c\n", d2.a, d2.f, d2.c);

		if (option == 'e') {
			printf("Exiting from client !\n");
			exit(0);
		}
}

int main()
{
	int sock_fd, conn_fd;
	struct sockaddr_in server_addr, cli;

	// socket create and varification
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
		printf(" failed !\n");
		exit(0);
	}
	else
		printf("Socket created !\n");

	bzero(&server_addr, sizeof(server_addr));
	// Use this code while running on the server
	ip = gethostbyname(hostname); 
	//use ip.h_addr instead of loopback

	// assign IP, PORT
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP_ADDR);
	server_addr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
		printf("server connection failed !\n");
		exit(0);
	}
	else
		printf("connected to server !\n");

	// function for chat
	func(sock_fd);

	// close the socket
	close(sock_fd);
}

