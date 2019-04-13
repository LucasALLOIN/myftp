/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** user.c
*/

#include <stdlib.h>
#include <ftp.h>
#include <ftp_message.h>
#include <string.h>
#include "user.h"

const int user_number = 2;

const user_t user_tab[] = {
    {"Anonymous", NULL, "."},
    {"lucas", "lucas", "/home/lucas"},
    {"test", "test", "/home"},
    {NULL, NULL, NULL}
};

void user(ftp_cmd_socket_t *this, char *command, char **argv)
{
    (void) command;
    if (tab_length(argv) == 1) {
        write(this->socket, PERMISSION_DENIED, strlen(PERMISSION_DENIED));
        return;
    } else if (this->logged) {
        write(this->socket, CANT_CHANGE_USER, strlen(CANT_CHANGE_USER));
        return;
    }
    this->user = NULL;
    for (int i = 0; i < user_number; i++)
        if (strcasecmp(user_tab[i].username, argv[1]) == 0)
            this->user = (user_t *) &user_tab[i];
    if (this->user == NULL)
        this->user = (user_t *) &user_tab[user_number];
    write(this->socket, SPECIFY_PASSWORD, strlen(SPECIFY_PASSWORD));
}