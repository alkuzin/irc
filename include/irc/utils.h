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
 * @file  utils.h
 * @brief Declares auxilar functions.
 * 
 * @author Alexander Kuzin (<a href="https://github.com/alkuzin">alkuzin</a>)
 * @date   19.07.2024 
 */

#ifndef _IRC_UTILS_HPP_
#define _IRC_UTILS_HPP_

#include <stdint.h>
#include <stdarg.h>

/**
 * @brief Get secure user input.
 * 
 * @param [out] input - given buffer to store input.
 * @param [in] size - given max size of @a input.
 */
void utils_getinput(char *input, uint32_t size);

/**
 * @brief Display log message.
 * 
 * @param [in] title - given log title.
 * @param [in] fmt - given format string.
 * @param [in] ... - given variable number of arguments to print.
 */
void utils_log(const char *title, const char *fmt, ...);

#endif /* _IRC_UTILS_HPP_ */