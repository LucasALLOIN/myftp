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

void retr_data_transfert(ftp_cmd_socket_t *this, char *command, char **argv,
int data_socket)
{
    int file = open(argv[1], O_RDONLY);
    char buffer[4096];
    ssize_t data_read;

    (void) command;
    if (file == -1) {
        write(this->socket, OPEN_FAILED, strlen(OPEN_FAILED));
        exit(0);
    }
    write(this->socket, START_DATA_TRANFERT, strlen(START_DATA_TRANFERT));
    while ((data_read = read(file, buffer, 4096)) > 0)
        write(data_socket, buffer, data_read);
    close(file);
    write(this->socket, DATA_TRANSFER_OK, strlen(DATA_TRANSFER_OK));
    close(data_socket);
    close(this->data_channel->socket);
    exit(0);
}

void retr(ftp_cmd_socket_t *this, char *command, char **argv)
{
    int file;
    char buf[256];

    getcwd(buf, 256);
    if (tab_length(argv) == 1) {
        write(this->socket, OPEN_FAILED, strlen(OPEN_FAILED));
        return;
    }
    chdir(this->work_dir);
    file = open(argv[1], O_RDONLY);
    if (file == -1) {
        write(this->socket, OPEN_FAILED, strlen(OPEN_FAILED));
        chdir(buf);
        return;
    }
    chdir(buf);
    close(file);
    exec_data_transfert(this, &retr_data_transfert, command, argv);
}