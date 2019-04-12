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
#include <fcntl.h>
#include "ftp.h"

void stor_data_transfert(ftp_cmd_socket_t *this, char *command, char **argv,
int data_socket)
{
    int file = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
    char buffer[4096];
    ssize_t data_read;

    (void) command;
    if (file == -1) {
        write(this->socket, OPEN_FAILED, strlen(OPEN_FAILED));
        exit(0);
    }
    write(this->socket, START_DATA_TRANFERT, strlen(START_DATA_TRANFERT));
    while ((data_read = read(data_socket, buffer, 4096)) > 0)
        write(file, buffer, data_read);
    close(file);
    write(this->socket, DATA_TRANSFER_OK, strlen(DATA_TRANSFER_OK));
    close(data_socket);
    close(this->data_channel->socket);
    exit(0);
}

void stor(ftp_cmd_socket_t *this, char *command, char **argv)
{
    if (tab_length(argv) == 1) {
        write(this->socket, OPEN_FAILED, strlen(OPEN_FAILED));
        return;
    }
    exec_data_transfert(this, &stor_data_transfert, command, argv);
}