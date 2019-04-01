/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** cdup.c
*/

#include <string.h>
#include <ftp_message.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <wait.h>
#include "ftp.h"

void list_data_transfert(ftp_cmd_socket_t *this, char *command, char **argv, int data_socket)
{
    char **arg;
    int pid;
    int status;

    (void) command;
    (void) argv;
    write(this->socket, SEND_LIST_DIR, strlen(SEND_LIST_DIR));
    if ((pid = fork()) == 0) {
        dup2(data_socket, 1);
        arg = malloc(sizeof(char *) * 3);
        arg[0] = "ls";
        arg[1] = "-l";
        arg[2] = NULL;
        execvp("ls", arg);
    } else {
        waitpid(pid, &status, 0);
        close(data_socket);
        close(this->data_channel->socket);
        write(this->socket, SEND_LIST_OK, strlen(SEND_LIST_OK));
    }
}

void list(ftp_cmd_socket_t *this, char *command, char **argv)
{
    exec_data_transfert(this, &list_data_transfert, command, argv);
}