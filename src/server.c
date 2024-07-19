/* IRC (Internet Relay Chat)
 * Copyright (C) 2024  Alexander (@alkuzin)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <irc/server.h>
#include <irc/utils.h>

/**
 * @brief Initialize IRC server.
 * 
 * @param [in] self - given server object pointer. 
 */
static void server_init(struct server_s *self);

/** 
 * @brief Free all memory allocated to IRC server. 
 * 
 * @param [in] self - given server object pointer. 
 */
static void server_destroy(struct server_s *self);

/**
 * @brief Receive message from client.
 * 
 * @param [in] sockfd - given client socket file descriptor.
 * @param [out] msg - given string to store message.
 * @param [in] size - given size of message in bytes.
 */
static void server_recv(int32_t sockfd, char *msg, size_t size);

/**
 * @brief Send message to client.
 * 
 * @param [in] sockfd - given client socket file descriptor.
 * @param [in] msg - given message to send.
 * @param [in] size - given size of message in bytes.
 */
static void server_send(int32_t sockfd, char *msg, size_t size);

/**
 * @brief Attach socket to port. 
 * 
 * @param [in] self - given server object pointer. 
 */
static void server_bind(struct server_s *self);

/**
 * @brief Prepare to accept connections on socket.
 * 
 * @param [in] self - given server object pointer. 
 */
static void server_listen(struct server_s *self);

/**
 * @brief Get IP address of given structure. 
 * 
 * @param [in] addr - given socket address.
 * @param [out] buffer - given buffer to store IP address string.
 */
static void server_get_ip(struct sockaddr_in *addr, char *buffer);

/**
 * @brief Show IP address & port of given socket address.
 * 
 * @param [in] addr - given socket address.
 */
static void server_show_info(struct sockaddr_in *addr);

/**
 * @brief Handle client.
 * 
 * @param [in] self - given server object pointer. 
 */
static void server_process(struct server_s *self);


void server_create(struct server_s *self)
{
    self->init    = server_init;
    self->destroy = server_destroy;
    self->recv    = server_recv;
    self->send    = server_send;
}

static void server_init(struct server_s *self)
{
    memset(&self->server_addr, 0, sizeof(self->server_addr));
    memset(&self->client_addr, 0, sizeof(self->client_addr));

    self->server_addr.sin_family      = AF_INET;      // address family (IPv4)
    self->server_addr.sin_port        = htons(12345); // port number in network byte order
    self->server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    self->sockfd                      = socket(AF_INET, SOCK_STREAM, 0);

    if (self->sockfd == -1)
        utils_error("server: socket initialization error");

    utils_log(SERVER_TITLE, "%s\n", "[ OK ] initialization successfull");

    server_bind(self);
    server_listen(self);

    putchar('\n');

    for (;;)
        server_process(self);
}

static void server_destroy(struct server_s *self)
{
    utils_log(SERVER_TITLE, "%s\n", "shutdown");
    close(self->sockfd);
}

static void server_bind(struct server_s *self)
{
    int32_t ret;

    ret = bind(self->sockfd, (struct sockaddr *)&(self->server_addr), sizeof(self->server_addr));

    if (ret == -1)
        utils_error("server: bind failed");

    utils_log(SERVER_TITLE, "%s\n", "[ OK ] binding successfull");
}

static void server_listen(struct server_s *self)
{
    int32_t ret;

    ret = listen(self->sockfd, MAX_CLIENTS);

    if (ret == -1)
        utils_error("server: listen failed");

    utils_log(SERVER_TITLE, "%s\n", "[ OK ] listening successfull");
}

static void server_recv(int32_t sockfd, char *msg, size_t size)
{
    ssize_t ret;

    ret = recv(sockfd, msg, size, 0);

    if (ret == -1)
        utils_error("server: recv error");
}

static void server_send(int32_t sockfd, char *msg, size_t size)
{
    ssize_t ret;

    ret = send(sockfd, msg, size, 0);

    if (ret == -1)
        utils_error("server: send error");
}

static void server_get_ip(struct sockaddr_in *addr, char *buffer)
{
	inet_ntop(AF_INET, &(addr->sin_addr.s_addr), buffer, INET_ADDRSTRLEN);
}

static void server_show_info(struct sockaddr_in *addr)
{
	char ip_addr[INET_ADDRSTRLEN];
	
	server_get_ip(addr, ip_addr);
	printf("(ip: %s port: %u)\n", ip_addr, addr->sin_port);
}

static void server_process(struct server_s *self)
{
    char    buffer[MSG_BUFSIZE];
    int32_t clientfd, addrlen;
    
    addrlen  = sizeof(self->client_addr);
    clientfd = accept(self->sockfd, (struct sockaddr *)&self->client_addr, (socklen_t *)&addrlen);
    
    if (clientfd == -1)
        utils_error("server: accept error");

    utils_log(SERVER_TITLE, "%s", "connected new client: ");
    server_show_info(&self->client_addr);

    utils_log(SERVER_TITLE, "waiting for client (%d)\n", clientfd);
    
    for (;;) {
        memset(buffer, 0, sizeof(buffer));
        self->recv(clientfd, buffer, sizeof(buffer));

        /* handeling incorrect/empty input */
        if (!buffer[0]) {
            utils_log(SERVER_TITLE, "client (%d) disconnected\n", clientfd);
            close(clientfd);
            return;
        }

        utils_log("client", "<%d>: \"%s\"\n", clientfd, buffer);
    }
}