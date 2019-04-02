

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

void client_command_callback(ftp_cmd_socket_t *this)
{
    char buffer[1024] = {0};
    char *clear;
    char **argv;

    ssize_t read_res = read(this->socket, buffer, 1024);
    if (read_res == 0) {
        delete_client(this->master, this);
        return;
    }
    buffer[read_res - 1] = 0;
    if (read_res >= 2 && buffer[read_res - 2] == '\r')
        buffer[read_res - 2] = 0;
    clear = my_strclear(buffer);
    argv = my_str_to_array(buffer, ' ');
    free(clear);
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
    if ((new_socket = accept(this->socket, (struct sockaddr *)&address, (socklen_t*) &addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    add_client(this->master, new_socket, inet_ntoa(address.sin_addr), &client_command_callback);
    write(new_socket, HELLO, strlen(HELLO));
}

ftp_command_channel_t *init_command_socket(uint16_t port)
{
    struct sockaddr_in address;
    ftp_command_channel_t *res = malloc(sizeof(ftp_command_channel_t));
    ftp_cmd_socket_t *master = malloc(sizeof(ftp_cmd_socket_t));
    struct in_addr var = {0};

    address.sin_family = AF_INET;
    address.sin_addr = var;
    address.sin_port = htons(port);
    master->master = res;
    master->logged = 1;
    res->port = htons(port);
    res->master_socket = master;
    res->client_socket = NULL;
    if ((res->master_socket->socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        print_and_exit("socket failed", 84);
    if (bind(res->master_socket->socket, (struct sockaddr *) &address, sizeof(address)) < 0)
        print_and_exit("bind failed", 84);
    if (listen(res->master_socket->socket, 0) < 0)
        print_and_exit("listen", 84);
    res->higher_fd = res->master_socket->socket;
    res->master_socket->read_callback = &master_command_callback;
    res->readfds = malloc(sizeof(fd_set));
    return (res);
}

void add_client(ftp_command_channel_t *channel, int socket, char *in_addr, void (*read_callback)(struct ftp_cmd_socket_s *this))
{
    socket_list_t *new_node = malloc(sizeof(socket_list_t));
    socket_list_t *head = channel->client_socket;
    ftp_cmd_socket_t *ftp_socket = malloc(sizeof(ftp_cmd_socket_t));

    memset(ftp_socket, 0, sizeof(ftp_cmd_socket_t));
    ftp_socket->data_channel = malloc(sizeof(ftp_data_socket_t));
    memset(ftp_socket->data_channel, 0, sizeof(ftp_data_socket_t));
    if (socket > channel->higher_fd)
        channel->higher_fd = socket;
    ftp_socket->data_channel->socket = 0;
    ftp_socket->data_channel->status = NOT_SET;
    new_node->next = NULL;
    ftp_socket->socket = socket;
    ftp_socket->master = channel;
    ftp_socket->logged = 0;
    ftp_socket->user = NULL;
    strcpy(ftp_socket->ip, in_addr);
    getcwd(ftp_socket->work_dir, 256);
    ftp_socket->read_callback = read_callback;
    new_node->ftp_socket = ftp_socket;
    if (head == NULL) {
        channel->client_socket = new_node;
        return;
    }
    while (head->next != NULL)
        head = head->next;
    head->next = new_node;
}

void delete_client(ftp_command_channel_t *channel, ftp_cmd_socket_t *to_delete)
{
    socket_list_t *head = NULL;

    if (to_delete == NULL || channel == NULL || channel->client_socket == NULL)
        return;
    head = channel->client_socket;
    if (head->ftp_socket == to_delete) {
        channel->client_socket = head->next;
        write(head->ftp_socket->socket, CLIENT_CLOSE, strlen(CLIENT_CLOSE));
        close(head->ftp_socket->socket);
        free(head->ftp_socket);
        free(head);
        return;
    }
    for (socket_list_t *last = head; head != NULL; head = last->next) {
        if (head->ftp_socket == to_delete) {
            last->next = head->next;
            write(head->ftp_socket->socket, CLIENT_CLOSE, strlen(CLIENT_CLOSE));
            close(head->ftp_socket->socket);
            free(head->ftp_socket);
            free(head);
            return;
        }
        last = head;
    }
}

unsigned int client_number(ftp_command_channel_t *channel)
{
    unsigned int i = 0;

    for (socket_list_t *client = channel->client_socket; client != NULL;
    client = client->next, i++);
    return (i);
}

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
    activity = select(channel->higher_fd + 1, channel->readfds , NULL, NULL, NULL);
    if ((activity < 0) && (errno != EINTR)) {
        printf("select error");
        return;
    }
    if (FD_ISSET(channel->master_socket->socket, channel->readfds))
        channel->master_socket->read_callback(channel->master_socket);
    for (socket_list_t *head = channel->client_socket; head != NULL; head = next) {
        next = head->next;
        if (FD_ISSET(head->ftp_socket->socket, channel->readfds))
            head->ftp_socket->read_callback(head->ftp_socket);
    }
}

void print_help(void)
{
    printf("%s\n", "USAGE: ./myftp port path");
    printf("%s\n", "       port is the port number on which the server socket listens");
    printf("%s\n", "       path is the path to the home directory for the Anonymous user");
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