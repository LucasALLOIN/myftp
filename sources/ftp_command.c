/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** ftp_command.c
*/

#include <stdlib.h>
#include <string.h>
#include <ftp_message.h>
#include "commands.h"

const ftp_command_t ftp_command[] = {
    {"USER", &user},
    {"PASS", &pass},
    {"PWD", &pwd},
    {"CDUP", &cdup},
    {"CWD", &cwd},
    {"DELE", &dele},
    {"PASV", &pasv},
    {"LIST", &list},
    {"RETR", &retr},
    {"STOR", &stor},
    {"PORT", &port},
    {"SYST", &syst},
    {"HELP", &help},
    {"NOOP", &noop},
    {"QUIT", &quit},
    {0}
};

void launch_ftp_command(ftp_cmd_socket_t *socket, char *command, char **argv)
{
    ftp_command_t ftp_command1 = {0};

    if (command == NULL)
        return;
    for (int i = 0; memcmp(&ftp_command1, &(ftp_command[i]), sizeof(ftp_command_t)) != 0; i++) {
        if (strcasecmp(command, ftp_command[i].command) == 0) {
            ftp_command[i].callback(socket, command, argv);
            return;
        }
    }
    if (socket->logged == 1) {
        write(socket->socket, UNKNOW_COMMAND, strlen(UNKNOW_COMMAND));
    } else {
        write(socket->socket, PLEASE_LOGIN, strlen(PLEASE_LOGIN));
    }
}