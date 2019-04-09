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
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include "ftp.h"

int create_active_socket(ftp_cmd_socket_t *ftp_cmd_socket)
{
    char **ip = ftp_cmd_socket->data_channel->ip;
    char *sock_ip;
    struct sockaddr_in address;

    ftp_cmd_socket->data_channel->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (ftp_cmd_socket->data_channel->socket == -1)
        return (-1);
    asprintf(&sock_ip, "%s.%s.%s.%s", ip[0], ip[1], ip[2], ip[3]);
    address.sin_port = htons((uint16_t) (atoi(ip[4]) * 256 + atoi(ip[5])));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(sock_ip);
    if (connect(ftp_cmd_socket->data_channel->socket, (struct sockaddr *) &address, sizeof(address)) == -1)
        return (-1);
    return (ftp_cmd_socket->data_channel->socket);
}

void port(ftp_cmd_socket_t *this, char *command, char **argv)
{
    char **ip;

    (void) command;
    if (this->logged == 0) {
        write(this->socket, PLEASE_LOGIN, strlen(PLEASE_LOGIN));
        return;
    } else if (tab_length(argv) == 1) {
        write(this->socket, ERROR_PORT, strlen(ERROR_PORT));
        return;
    } else if (this->data_channel->status == PASV || this->data_channel->status == PORT) {
        close(this->data_channel->socket);
        this->data_channel->socket = 0;
    }
    ip = my_str_to_array(argv[1], ',');
    this->data_channel->status = NOT_SET;
    if (tab_length(ip) != 6) {
        this->data_channel->socket = 0;
        write(this->socket, ERROR_PORT, strlen(ERROR_PORT));
    } else {
        this->data_channel->ip = ip;
        this->data_channel->status = PORT;
        write(this->socket, PORT_OK, strlen(PORT_OK));
    }
}