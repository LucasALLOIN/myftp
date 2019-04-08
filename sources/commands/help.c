/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** help.c
*/

#include <ftp.h>
#include <ftp_message.h>
#include <string.h>

void help(ftp_cmd_socket_t *this, char *command, char **argv)
{
    (void) command;
    (void) argv;
    if (this->logged == 0) {
        write(this->socket, PLEASE_LOGIN, strlen(PLEASE_LOGIN));
        return;
    }
    write(this->socket, "214\n", strlen("214\n"));
}