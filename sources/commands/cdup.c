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

void cdup(ftp_cmd_socket_t *this, char *command, char **argv)
{
    char work_dir[256];

    (void) command;
    (void) argv;
    if (this->logged == 0) {
        write(this->socket, PLEASE_LOGIN, strlen(PLEASE_LOGIN));
        return;
    }
    getcwd(work_dir, 256);
    chdir(this->work_dir);
    if (chdir("..") == 0) {
        getcwd(this->work_dir, 256);
        write(this->socket, CHANGE_DIR_OK, strlen(CHANGE_DIR_OK));
    } else
        write(this->socket, CHANGE_DIR_FAILED, strlen(CHANGE_DIR_FAILED));
    chdir(work_dir);
}