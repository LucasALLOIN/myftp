/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** pasv.c
*/

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ftp_message.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include "ftp.h"

void do_port(ftp_cmd_socket_t *this, char **argv)
{
    this->data_channel->ip = my_str_to_array(argv[1], ',');
    this->data_channel->status = NOT_SET;
    if (tab_length(this->data_channel->ip) != 6) {
        this->data_channel->socket = 0;
        write(this->socket, ERROR_PORT, strlen(ERROR_PORT));
        free_tab(this->data_channel->ip);
    } else {
        this->data_channel->status = PORT;
        write(this->socket, PORT_OK, strlen(PORT_OK));
    }
}

void port(ftp_cmd_socket_t *this, char *command, char **argv)
{
    (void) command;
    if (this->logged == 0) {
        write(this->socket, PLEASE_LOGIN, strlen(PLEASE_LOGIN));
        return;
    } else if (tab_length(argv) == 1) {
        write(this->socket, ERROR_PORT, strlen(ERROR_PORT));
        return;
    } else if (this->data_channel->status == PASV ||
    this->data_channel->status == PORT) {
        close(this->data_channel->socket);
        this->data_channel->socket = 0;
    }
    do_port(this, argv);
}