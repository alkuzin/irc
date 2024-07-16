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
#include <stdio.h>

#include <irc/client.h>

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


void client_create(struct client_s *self)
{
    self->init    = client_init;
    self->destroy = client_destroy;
    self->recv    = client_recv;
    self->send    = client_send;
}

static void client_init(struct client_s *self)
{
    puts("[CLIENT] initializing client");
    memset(&self->server_addr, 0, sizeof(self->server_addr));

    self->server_addr.sin_family      = AF_INET;      // address family (IPv4)
    self->server_addr.sin_port        = htons(12345); // port number in network byte order
    self->server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    self->sockfd                      = 0;
}

static void client_destroy(struct client_s *self)
{
    puts("[client] shutdown");
    close(self->sockfd);
}

static void client_recv(struct client_s *self, char *msg, size_t size)
{
    (void) self;
    (void) msg;
    (void) size;
}

static void client_send(struct client_s *self, char *msg, size_t size)
{
    (void) self;
    (void) msg;
    (void) size;
}