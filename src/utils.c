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

#include <stdint.h>
#include <stdio.h>

#include <irc/utils.h>


void getinput(char *input, uint32_t size) 
{
	uint32_t i;

    i = 0;
	fgets(input, size, stdin);

	if (input) {
        
		while (input[i] != '\n' && input[i] != '\0')
			i++;
        
		if (input[i] == '\n')
            input[i] = '\0';
		else {
            while (getchar() != '\n')
                continue;
        }
    }
}