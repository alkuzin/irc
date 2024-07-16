INCLUDE_DIR = ../include/
CC      	= gcc
CFLAGS 		= -Wall -Wextra -Werror -g -I $(INCLUDE_DIR)

SRC  = server client
SRCS = $(addsuffix .c, $(SRC))
OBJS = $(addsuffix .o,   $(SRC))

OBJS_DIR 	  = objs/
OBJS_PREFIXED = $(addprefix $(OBJS_DIR), $(OBJS))

$(OBJS_PREFIXED): $(SRCS)
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $(SRCS)
	mv *.o $(OBJS_DIR)

tcpserver: $(OBJS_PREFIXED)
	$(CC) $(CFLAGS) server_main.c $(OBJS_PREFIXED) -o server_irc

tcpclient: $(OBJS_PREFIXED)
	$(CC) $(CFLAGS) client_main.c $(OBJS_PREFIXED) -o client_irc

tcp: tcpserver tcpclient

all: tcp
	mv server_irc ../
	mv client_irc ../

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf server_irc client_irc

re: clean all