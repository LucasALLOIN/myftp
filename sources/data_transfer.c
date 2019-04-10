/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** data_transfer.c
*/

#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "ftp_message.h"
#include "ftp.h"


void exec_data_child(ftp_cmd_socket_t *this, void (*data_callback)(ftp_cmd_socket_t *this, char *command, char **argv, int send_socket), char *command, char **argv)
{
    struct sockaddr_in address;
    socklen_t size = sizeof(address);
    int send_socket = (this->data_channel->status == PORT)
                      ? this->data_channel->socket : accept(
            this->data_channel->socket, (struct sockaddr *) &address, &size);

    if (send_socket == -1) {
        write(this->socket, DATA_TRANSFER_FAILED, strlen(DATA_TRANSFER_FAILED));
        exit(0);
    }
    data_callback(this, command, argv, send_socket);
}

void exec_data_transfert(ftp_cmd_socket_t *this, void (*data_callback)(ftp_cmd_socket_t *this, char *command, char **argv, int send_socket), char *command, char **argv)
{
    int pid;

    if (this->logged == 0) {
        write(this->socket, PLEASE_LOGIN, strlen(PLEASE_LOGIN));
        return;
    }
    if (this->data_channel->status == NOT_SET) {
        write(this->socket, NEED_DATA_CHANNEL, strlen(NEED_DATA_CHANNEL));
        return;
    }
    pid = fork();
    if (pid == 0) {
        chdir(this->work_dir);
        exec_data_child(this, data_callback, command, argv);
        exit(0);
    } else {
        this->data_channel->socket = 0;
        this->data_channel->status = NOT_SET;
    }
}