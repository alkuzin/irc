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

#include <irc/client.h>
#include <irc/utils.h>

/**
 * @brief Initialize IRC client.
 * 
 * @param [in] self - given client object pointer. 
 */
static void client_init(struct client_s *self);

/** 
 * @brief Free all memory allocated to IRC client. 
 * 
 * @param [in] self - given client object pointer. 
 */
static void client_destroy(struct client_s *self);

/**
 * @brief Receive message from server.
 * 
 * @param [in] self - given client object pointer. 
 * @param [out] msg - given string to store message.
 * @param [in] size - given size of message in bytes.
 */
static void client_recv(struct client_s *self, char *msg, size_t size);

/**
 * @brief Send message to server.
 * 
 * @param [in] self - given client object pointer. 
 * @param [in] msg - given message to send.
 * @param [in] size - given size of message in bytes.
 */
static void client_send(struct client_s *self, char *msg, size_t size);

/**
 * @brief Connect to server.
 * 
 * @param [in] size - given size of message in bytes.
 */
static void client_connect(struct client_s *self);

/**
 * @brief Handle server.
 * 
 * @param [in] self - given client object pointer. 
 */
static void client_process(struct client_s *self);


void client_create(struct client_s *self)
{
    self->init    = client_init;
    self->destroy = client_destroy;
    self->recv    = client_recv;
    self->send    = client_send;
}

static void client_init(struct client_s *self)
{
    memset(&self->server_addr, 0, sizeof(self->server_addr));

    self->server_addr.sin_family      = AF_INET;      // address family (IPv4)
    self->server_addr.sin_port        = htons(12345); // port number in network byte order
    self->server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    self->sockfd                      = socket(AF_INET, SOCK_STREAM, 0);

    if (self->sockfd == -1)
        utils_error("client: socket initialization error");

    client_connect(self);
    client_process(self);
}

static void client_destroy(struct client_s *self)
{
    utils_log(CLIENT_TITLE, "%s\n", "shutdown");
    close(self->sockfd);
}

static void client_connect(struct client_s *self)
{
    int32_t ret;

    ret = connect(self->sockfd, (struct sockaddr *)&self->server_addr, sizeof(self->server_addr));

    if (ret == -1)
        utils_error("server: accept error");
}

static void client_recv(struct client_s *self, char *msg, size_t size)
{
    ssize_t ret;

    ret = recv(self->sockfd, msg, size, 0);

    if (ret == -1)
        utils_error("client: recv error");
}

static void client_send(struct client_s *self, char *msg, size_t size)
{
    ssize_t ret;

    ret = send(self->sockfd, msg, size, 0);

    if (ret == -1)
        utils_error("client: send error");
}

static void client_process(struct client_s *self)
{
    char input[MSG_BUFSIZE];

    for (;;) {
        memset(input, 0, sizeof(input));

        printf("%s", ">>> ");
        utils_getinput(input, MSG_BUFSIZE);

        if (!input[0])
            continue;

        client_send(self, input, sizeof(input));
    }
}