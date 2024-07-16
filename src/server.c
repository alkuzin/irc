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
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <irc/server.h>

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
 * @param [in] self - given server object pointer. 
 * @param [out] msg - given string to store message.
 * @param [in] size - given size of message in bytes.
 */
static void server_recv(struct server_s *self, char *msg, size_t size);

/**
 * @brief Send message to client.
 * 
 * @param [in] self - given server object pointer.
 * @param [in] msg - given message to send.
 * @param [in] size - given size of message in bytes.
 */
static void server_send(struct server_s *self, char *msg, size_t size);

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
    puts("[SERVER] initializing server");
    memset(&self->server_addr, 0, sizeof(self->server_addr));
    memset(&self->client_addr, 0, sizeof(self->client_addr));

    self->server_addr.sin_family      = AF_INET;      // address family (IPv4)
    self->server_addr.sin_port        = htons(12345); // port number in network byte order
    self->server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    self->sockfd                      = socket(AF_INET, SOCK_STREAM, 0);

    if (self->sockfd == -1) {
        perror("server_init: socket initialization error");
        exit(EXIT_FAILURE);
    }

    server_bind(self);
    server_listen(self);
    server_process(self);
}

static void server_destroy(struct server_s *self)
{
    puts("[SERVER] shutdown");
    close(self->sockfd);
}

static void server_bind(struct server_s *self)
{
    int32_t ret;

    puts("[SERVER] binding server");
    ret = bind(self->sockfd, (struct sockaddr *)&(self->server_addr), sizeof(self->server_addr));

    if (ret == -1) {
        perror("server_bind: bind failed");
        exit(EXIT_FAILURE);
    }
}

static void server_listen(struct server_s *self)
{
    int32_t ret;

    puts("[SERVER] listening for clients");
    ret = listen(self->sockfd, MAX_CLIENTS);

    if (ret == -1) {
        perror("server_listen: listen failed");
        exit(EXIT_FAILURE);
    }
}

static void server_recv(struct server_s *self, char *msg, size_t size)
{
    ssize_t ret;

    ret = recv(self->sockfd, msg, size, 0);

    if (ret == -1) {
        perror("server_process: recv error");
        exit(EXIT_FAILURE);
    }
}

static void server_send(struct server_s *self, char *msg, size_t size)
{
    ssize_t ret;

    ret = send(self->sockfd, msg, size, 0);

    if (ret == -1) {
        perror("server_process: recv error");
        exit(EXIT_FAILURE);
    }
}

static void server_process(struct server_s *self)
{
    int32_t clientfd, addrlen;
    char    buffer[MSG_BUFSIZE];
    
    addrlen = sizeof(self->server_addr);

    for (;;) {
        clientfd = accept(self->sockfd, (struct sockaddr *)&self->server_addr, (socklen_t *)&addrlen);

        if (clientfd == -1) {
            perror("server_process: accept error");
            exit(EXIT_FAILURE);
        }

        server_recv(self, buffer, MSG_BUFSIZE);
        printf("[SERVER] received \"%s\"\n", buffer);
    }
}