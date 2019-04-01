/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** pasv.c
*/

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ftp_message.h>
#include "ftp.h"

int create_passive_socket(ftp_cmd_socket_t *ftp_cmd_socket)
{
    ftp_data_socket_t *data = ftp_cmd_socket->data_channel;
    struct sockaddr_in address;
    socklen_t adress_len;

    if ((data->socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return (1);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = 0;
    if (bind(data->socket, (struct sockaddr*)&address, sizeof(address)) == -1)
        return (1);
    if (listen(data->socket, 1) == -1)
        return (1);
    adress_len = sizeof(address);
    if (getsockname(data->socket, (struct sockaddr*)&address, &adress_len) == -1)
        return (1);
    data->port = ntohs(address.sin_port);
    data->status = PASV;
    return (0);
}

void pasv(ftp_cmd_socket_t *this, char *command, char **argv)
{
    char **ip = my_str_to_array(this->ip, '.');
    char *res;

    (void) command;
    (void) argv;
    if (this->logged == 0) {
        write(this->socket, PLEASE_LOGIN, strlen(PLEASE_LOGIN));
        free_tab(ip);
        return;
    } else if (this->data_channel->status == PASV || this->data_channel->status == PORT) {
        close(this->data_channel->socket);
        this->data_channel->socket = 0;
    }
    this->data_channel->status = NOT_SET;
    if (create_passive_socket(this) == 1) {
        write(this->socket, ERROR_PASV, strlen(ERROR_PASV));
    } else {
        asprintf(&res, "227 Entering Passive Mode (%s,%s,%s,%s,%d,%d).\n",
        ip[0], ip[1], ip[2], ip[3], this->data_channel->port / 256,
        this->data_channel->port % 256);
        write(this->socket, res, strlen(res));
    }
    free_tab(ip);
}