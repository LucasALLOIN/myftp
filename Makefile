##
## EPITECH PROJECT, 2017
## Makefile
## File description:
## desc
##

CC	=	cc

SRC	=	sources/main.c							\
		sources/count_to_split.c				\
		sources/data_transfer.c					\
		sources/ftp_command.c					\
		sources/my_str_to_array.c				\
		sources/str_clear.c						\
		sources/commands/cdup.c					\
		sources/commands/cwd.c					\
		sources/commands/dele.c					\
		sources/commands/list.c					\
		sources/commands/noop.c					\
		sources/commands/pass.c					\
		sources/commands/pasv.c					\
		sources/commands/port.c					\
		sources/commands/pwd.c					\
		sources/commands/quit.c					\
		sources/commands/retr.c					\
		sources/commands/stor.c					\
		sources/commands/user.c					\
		sources/commands/syst.c

CFLAGS	=	-I./includes/ -Werror -Wextra -W -Wall -Wparentheses -Wsign-compare -Wpointer-sign -Wuninitialized -Wunused-but-set-variable

OBJ	=	$(SRC:.c=.o)

NAME	=	myftp


all:	$(NAME)

$(NAME):	$(OBJ)
	@$(CC) -o $(NAME)	$(OBJ)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

exe:
	gcc $(OBJ) -o $(NAME)

docker:
	sudo docker run -it -v `pwd`:/home/epitest -w /home/epitest epitechcontent/epitest-docker /bin/bash

re:	fclean all
