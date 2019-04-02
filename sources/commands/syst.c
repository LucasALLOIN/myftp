/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** cdup.c
*/

#include <string.h>
#include <ftp_message.h>
#include <stdlib.h>
#include "ftp.h"

void syst(ftp_cmd_socket_t *this, char *command, char **argv)
{
    (void) command;
    (void) argv;
    write(this->socket, SYST_TYPE, strlen(SYST_TYPE));
}