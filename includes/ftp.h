/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** ftp.h
*/

#ifndef NWP_MYFTP_2018_FTP_H
#define NWP_MYFTP_2018_FTP_H

#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include "user.h"

struct ftp_command_channel_s;

enum data_channel_status {
    NOT_SET,
    PASV,
    PORT
};

typedef struct ftp_data_socket_s {
    int socket;
    uint16_t port;
    enum data_channel_status status;
    char **ip;
} ftp_data_socket_t;

typedef struct ftp_cmd_socket_s {
    int socket;
    char logged;
    user_t *user;
    char work_dir[256];
    char ip[16];
    void (*read_callback)(struct ftp_cmd_socket_s *this);
    struct ftp_data_socket_s *data_channel;
    struct ftp_command_channel_s *master;
} ftp_cmd_socket_t;

typedef struct socket_list_s {
    ftp_cmd_socket_t *ftp_socket;
    struct socket_list_s *next;
} socket_list_t;

typedef struct ftp_command_channel_s {
    ftp_cmd_socket_t *master_socket;
    socket_list_t *client_socket;
    int higher_fd;
    fd_set *readfds;
    uint16_t port;
    char *def_path;
} ftp_command_channel_t;

void add_client(ftp_command_channel_t *channel, int socket, char *in_addr, void
(*read_callback)(struct ftp_cmd_socket_s *this));
void delete_client(ftp_command_channel_t *channel, ftp_cmd_socket_t *to_delete);
int count_to_split(char *cmd, char to_split);
char *my_strclear(char *str);
char **my_str_to_array(char *cmd, char to_s);
unsigned int tab_length(char **argv);
int asprintf(char **strp, const char *fmt, ...);
void free_tab(char **tab);
void exec_data_transfert(ftp_cmd_socket_t *this, void
(*data_callback)(ftp_cmd_socket_t *this, char *command, char **argv, int
send_socket), char *command, char **argv);
void *my_malloc(size_t size);
void print_and_exit(char *error, int exit_code);
void client_command_callback(ftp_cmd_socket_t *this);
void master_command_callback(ftp_cmd_socket_t *this);
ftp_command_channel_t *init_command_socket(uint16_t port);
void set_add_cli_value(ftp_cmd_socket_t *ftp_socket, socket_list_t *new_node,
char *in_addr, int socket);

#endif //NWP_MYFTP_2018_FTP_H
