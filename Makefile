SRC_DIR  = src/

tcp:
	$(MAKE) -C $(SRC_DIR) tcp

all:
	$(MAKE) -C $(SRC_DIR) all

clean:
	$(MAKE) -C $(SRC_DIR) clean

fclean: clean
	rm -f server_irc client_irc

re: clean all