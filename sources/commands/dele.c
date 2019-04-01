/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** cdup.c
*/

#include <string.h>
#include <ftp_message.h>
#include <stdlib.h>
#include <stdio.h>
#include "ftp.h"

void dele(ftp_cmd_socket_t *this, char *command, char **argv)
{
    (void) command;
    if (this->logged == 0) {
        write(this->socket, PLEASE_LOGIN, strlen(PLEASE_LOGIN));
        return;
    }
    if (tab_length(argv) < 2) {
        write(this->socket, FILE_NOT_FOUND, strlen(FILE_NOT_FOUND));
        return;
    }
    if (remove(argv[1]) != -1)
        write(this->socket, FILE_DELETED, strlen(FILE_DELETED));
    else
        write(this->socket, FILE_NOT_FOUND, strlen(FILE_NOT_FOUND));
}