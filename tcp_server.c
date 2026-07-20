/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tcp_server.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 13:30:41 by daflynn           #+#    #+#             */
/*   Updated: 2026/07/20 15:16:10 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
	int server_fd, client_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	char buffer[1024];

	// 1, Create a socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0)
	{
		perror("socket");
		exit(1);
	}

	//allow quick restart
	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));

	//bind
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(6667); //common irc port 

	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) <0){
	perror("bind");
	exit(1);
	}

	//listen
	if(listen(server_fd, 5) < 0)
	{
		perror("listen");
		exit(1);
	}
	printf("server listening\n");
	printf("waiting for client\n");

	//accept one client
	client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
	if(client_fd < 0)
	{
		perror("accept");
		exit(1);
	}

	printf("Client connected from %s:%d\n",
			inet_ntoa(client_addr.sin_addr),
			ntohs(client_addr.sin_port));

	//simple receive
	int n = recv(client_fd, buffer, sizeof(buffer)-1, 0);
	if(n > 0){
		buffer[n] = '\0';
		printf("Received: %s\n", buffer);
	
	}
	close(client_fd);
	close(server_fd);
}
