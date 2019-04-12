/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** cdup.c
*/

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <ftp.h>
#include <commands.h>
#include <ftp_message.h>
#include <netdb.h>
#include <arpa/inet.h>

void init_select_list(ftp_command_channel_t *channel)
{
    FD_ZERO(channel->readfds);
    FD_SET(channel->master_socket->socket, channel->readfds);
    for (socket_list_t *client = channel->client_socket; client != NULL;
    client = client->next) {
        if (client->ftp_socket->socket != 0)
            FD_SET(client->ftp_socket->socket, channel->readfds);
    }
}

void client_selector(ftp_command_channel_t *channel)
{
    int activity = 0;
    socket_list_t *next = NULL;

    init_select_list(channel);
    activity = select(channel->higher_fd + 1, channel->readfds , NULL, NULL,
    NULL);
    if ((activity < 0) && (errno != EINTR)) {
        printf("select error");
        return;
    }
    if (FD_ISSET(channel->master_socket->socket, channel->readfds))
        channel->master_socket->read_callback(channel->master_socket);
    for (socket_list_t *h = channel->client_socket; h != NULL; h = next) {
        next = h->next;
        if (FD_ISSET(h->ftp_socket->socket, channel->readfds))
            h->ftp_socket->read_callback(h->ftp_socket);
    }
}

void print_help(void)
{
    printf("%s\n", "USAGE: ./myftp port path");
    printf("%s\n", "       port is the port number on which the server socket "
    "listens");
    printf("%s\n", "       path is the path to the home directory for the "
    "Anonymous user");
}

int test_default_path(char *path)
{
    char buf[256];

    getcwd(buf, 256);
    if (chdir(path) == -1) {
        chdir(buf);
        fprintf(stderr, "%s\n", "Bad path");
        exit(84);
    }
    chdir(buf);
    return (0);
}

int main(int argc, char **argv)
{
    ftp_command_channel_t *command_channel;
    unsigned short port;

    if (argc < 3) {
        print_help();
        return (84);
    }
    port = (unsigned short) atoi(argv[1]);
    if (port == 0 || test_default_path(argv[2])) {
        fprintf(stderr, "%s\n", "Bad port");
        return (84);
    }
    command_channel = init_command_socket(port);
    command_channel->def_path = argv[2];
    while (1) {
        client_selector(command_channel);
    }
    return (0);
}