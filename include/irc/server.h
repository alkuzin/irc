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
 * @file  server.h
 * @brief Declares IRC server related structure & functions.
 * 
 * @author Alexander Kuzin (<a href="https://github.com/alkuzin">alkuzin</a>)
 * @date   16.07.2024 
 */

#ifndef _IRC_SERVER_HPP_
#define _IRC_SERVER_HPP_

#include <netinet/in.h>
#include <sys/types.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_CLIENTS 3
#define MSG_BUFSIZE 256

struct server_s {
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int32_t sockfd;

    void (*init)    (struct server_s *self);
    void (*destroy) (struct server_s *self);
    void (*recv)    (int32_t, char *, size_t);
    void (*send)    (int32_t, char *, size_t);
};
typedef struct server_s server_t;

/**
 * @brief Create IRC server object.
 * 
 * @param [in] self - given server object pointer. 
 */
void server_create(struct server_s *self);

#endif /* _IRC_SERVER_HPP_ */