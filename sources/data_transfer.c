/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** data_transfer.c
*/

#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "ftp_message.h"
#include "ftp.h"

int create_active_socket(ftp_cmd_socket_t *ftp_cmd_socket, char **ip)
{
    char *sock_ip;
    struct sockaddr_in address;

    ftp_cmd_socket->data_channel->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (ftp_cmd_socket->data_channel->socket == -1)
        return (1);
    asprintf(&sock_ip, "%s.%s.%s.%s", ip[0], ip[1], ip[2], ip[3]);
    address.sin_port = htons((uint16_t) (atoi(ip[4]) * 256 + atoi(ip[5])));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(sock_ip);
    if (connect(ftp_cmd_socket->data_channel->socket,
    (struct sockaddr *) &address, sizeof(address)) == -1)
        return (1);
    return (0);
}

void exec_data_child(ftp_cmd_socket_t *this, void
(*data_callback) (ftp_cmd_socket_t *this, char *command, char **argv, int
send_socket), char *command, char **argv)
{
    struct sockaddr_in address;
    socklen_t size = sizeof(address);
    int send_socket = -1;

    if (this->data_channel->status == PORT &&
    create_active_socket(this, this->data_channel->ip)) {
        write(this->socket, DATA_TRANSFER_FAILED, strlen(DATA_TRANSFER_FAILED));
        exit(0);
    } else if (this->data_channel->status == PORT)
        send_socket = this->data_channel->socket;
    (void) this;
    if (this->data_channel->status == PASV)
        send_socket = accept(this->data_channel->socket,
        (struct sockaddr *) &address, &size);
    if (send_socket == -1) {
        write(this->socket, DATA_TRANSFER_FAILED, strlen(DATA_TRANSFER_FAILED));
        exit(0);
    }
    data_callback(this, command, argv, send_socket);
}

void exec_data_transfert(ftp_cmd_socket_t *this, void
(*data_callback)(ftp_cmd_socket_t *this, char *command, char **argv, int
send_socket), char *command, char **argv)
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
        close(this->data_channel->socket);
        this->data_channel->socket = 0;
        this->data_channel->status = NOT_SET;
    }
}