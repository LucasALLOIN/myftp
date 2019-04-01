/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** quit.c
*/

#include <ftp.h>

void quit(ftp_cmd_socket_t *this, char *command, char **argv)
{
    (void) command;
    (void) argv;
    delete_client(this->master, this);
}