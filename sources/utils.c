/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** utils.c
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

void free_tab(char **tab)
{
    int i = 0;

    while (tab[i] != NULL)
        free(tab[i++]);
    free(tab);
}

void print_and_exit(char *error, int exit_code)
{
    fprintf(stderr, "%s", error);
    exit(exit_code);
}

void add_client(ftp_command_channel_t *channel, int socket, char *in_addr, void
(*read_callback)(struct ftp_cmd_socket_s *this))
{
    socket_list_t *new_node = malloc(sizeof(socket_list_t));
    socket_list_t *head = channel->client_socket;
    ftp_cmd_socket_t *ftp_socket = malloc(sizeof(ftp_cmd_socket_t));

    memset(ftp_socket, 0, sizeof(ftp_cmd_socket_t));
    ftp_socket->data_channel = malloc(sizeof(ftp_data_socket_t));
    memset(ftp_socket->data_channel, 0, sizeof(ftp_data_socket_t));
    if (socket > channel->higher_fd)
        channel->higher_fd = socket;
    set_add_cli_value(ftp_socket, new_node, in_addr, socket);
    ftp_socket->master = channel;
    ftp_socket->read_callback = read_callback;
    if (head == NULL) {
        channel->client_socket = new_node;
        return;
    }
    while (head->next != NULL)
        head = head->next;
    head->next = new_node;
}

void delete_cli_two(socket_list_t *head)
{
    write(head->ftp_socket->socket, CLIENT_CLOSE, strlen(CLIENT_CLOSE));
    close(head->ftp_socket->socket);
    free(head->ftp_socket);
}

void delete_client(ftp_command_channel_t *channel, ftp_cmd_socket_t *to_delete)
{
    socket_list_t *head = NULL;

    if (to_delete == NULL || channel == NULL || channel->client_socket == NULL)
        return;
    head = channel->client_socket;
    if (head->ftp_socket == to_delete) {
        channel->client_socket = head->next;
        delete_cli_two(head);
        free(head);
        return;
    }
    for (socket_list_t *last = head; head != NULL; head = last->next) {
        if (head->ftp_socket == to_delete) {
            last->next = head->next;
            delete_cli_two(head);
            free(head);
            return;
        }
        last = head;
    }
}