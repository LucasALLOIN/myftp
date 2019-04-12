/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** server.c
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

void client_command_callback(ftp_cmd_socket_t *this)
{
    char *line = NULL;
    size_t len = 0;
    char **argv;
    FILE *stream = fdopen(this->socket, "r");

    if (stream == NULL)
        return;
    ssize_t read_res = getline(&line, &len, stream);
    if (read_res == -1) {
        delete_client(this->master, this);
        return;
    }
    printf("%ld %ld", len, read_res);
    line[read_res - 1] = 0;
    if (read_res >= 2 && line[read_res - 2] == '\r')
        line[read_res - 2] = 0;
    argv = my_str_to_array(line, ' ');
    free(line);
    launch_ftp_command(this, argv[0], argv);
}

void master_command_callback(ftp_cmd_socket_t *this)
{
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct in_addr var = {0};

    address.sin_family = AF_INET;
    address.sin_addr = var;
    address.sin_port = htons(this->master->port);
    if ((new_socket = accept(this->socket, (struct sockaddr *) &address,
    (socklen_t *) &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    add_client(this->master, new_socket, inet_ntoa(address.sin_addr),
    &client_command_callback);
    write(new_socket, HELLO, strlen(HELLO));
}

void set_value_init(ftp_command_channel_t *res, ftp_cmd_socket_t *master,
                    uint16_t port, struct sockaddr_in *address)
{
    struct in_addr var = {0};

    address->sin_family = AF_INET;
    address->sin_addr = var;
    address->sin_port = htons(port);
    master->master = res;
    master->logged = 1;
    res->port = htons(port);
    res->master_socket = master;
    res->client_socket = NULL;
}

ftp_command_channel_t *init_command_socket(uint16_t port)
{
    struct sockaddr_in address;
    ftp_command_channel_t *res = my_malloc(sizeof(ftp_command_channel_t));
    ftp_cmd_socket_t *master = my_malloc(sizeof(ftp_cmd_socket_t));
    int option = 1;

    set_value_init(res, master, port, &address);
    if ((res->master_socket->socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    print_and_exit("socket failed", 84);
    setsockopt(res->master_socket->socket, SOL_SOCKET, SO_REUSEADDR, &option,
    sizeof(option));
    if (bind(res->master_socket->socket, (struct sockaddr *) &address,
    sizeof(address)) < 0)
        print_and_exit("bind failed", 84);
    if (listen(res->master_socket->socket, 0) < 0)
        print_and_exit("listen", 84);
    res->higher_fd = res->master_socket->socket;
    res->master_socket->read_callback = &master_command_callback;
    res->readfds = malloc(sizeof(fd_set));
    return (res);
}

void set_add_cli_value(ftp_cmd_socket_t *ftp_socket, socket_list_t *new_node,
char *in_addr, int socket)
{
    ftp_socket->data_channel->socket = 0;
    ftp_socket->data_channel->status = NOT_SET;
    new_node->next = NULL;
    ftp_socket->socket = socket;
    ftp_socket->logged = 0;
    ftp_socket->user = NULL;
    strcpy(ftp_socket->ip, in_addr);
    getcwd(ftp_socket->work_dir, 256);
    new_node->ftp_socket = ftp_socket;
}
