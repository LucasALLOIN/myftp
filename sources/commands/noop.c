/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** noop.c
*/


#include <ftp_message.h>
#include <string.h>
#include "ftp.h"

void noop(ftp_cmd_socket_t *this, char *command, char **argv)
{
    (void) command;
    (void) argv;

    if (this->logged == 0) {
        write(this->socket, PLEASE_LOGIN, strlen(PLEASE_LOGIN));
        return;
    }
    write(this->socket, COMMAND_OK, strlen(COMMAND_OK));
}