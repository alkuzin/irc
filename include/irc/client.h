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

/**
 * @file  client.h
 * @brief Declares IRC client related structure & functions.
 * 
 * @author Alexander Kuzin (<a href="https://github.com/alkuzin">alkuzin</a>)
 * @date   16.07.2024 
 */

#ifndef _IRC_CLIENT_HPP_
#define _IRC_CLIENT_HPP_

#include <netinet/in.h>
#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>

#define CLIENT_TITLE "client"
#define MSG_BUFSIZE  256

struct client_s {
    struct sockaddr_in server_addr;
    int32_t sockfd;

    void (*init)    (struct client_s *self);
    void (*destroy) (struct client_s *self);
    void (*recv)    (struct client_s *self, char *, size_t);
    void (*send)    (struct client_s *self, char *, size_t);
};
typedef struct client_s client_t;

/**
 * @brief Create IRC client object.
 * 
 * @param [in] self - given client object pointer. 
 */
void client_create(struct client_s *self);

#endif /* _IRC_CLIENT_HPP_ */