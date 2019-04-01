/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** pwd.c
*/

#include <string.h>
#include <stdlib.h>
#include <ftp_message.h>
#include "ftp.h"

void pwd(ftp_cmd_socket_t *this, char *command, char **argv)
{
    char *res;

    (void) command;
    (void) argv;
    if (this->logged == 0) {
        write(this->socket, PLEASE_LOGIN, strlen(PLEASE_LOGIN));
        return;
    }
    asprintf(&res, "257 \"%s\"\n", this->work_dir);
    write(this->socket, res, strlen(res));
    free(res);
}