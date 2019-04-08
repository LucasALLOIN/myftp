/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** commands.h
*/

#ifndef NWP_MYFTP_2018_COMMANDS_H
#define NWP_MYFTP_2018_COMMANDS_H

#include "ftp.h"

typedef struct ftp_command_s {
    char *command;
    void (*callback)(ftp_cmd_socket_t *ftp_socket, char *command, char **argv);
} ftp_command_t;

void user(ftp_cmd_socket_t *, char *, char **);
void pass(ftp_cmd_socket_t *this, char *command, char **argv);
void pwd(ftp_cmd_socket_t *this, char *command, char **argv);
void cwd(ftp_cmd_socket_t *this, char *command, char **argv);
void pasv(ftp_cmd_socket_t *this, char *command, char **argv);
void cdup(ftp_cmd_socket_t *this, char *command, char **argv);
void noop(ftp_cmd_socket_t *this, char *command, char **argv);
void quit(ftp_cmd_socket_t *socket, char *command, char **argv);
void list(ftp_cmd_socket_t *this, char *command, char **argv);
void port(ftp_cmd_socket_t *this, char *command, char **argv);
void dele(ftp_cmd_socket_t *this, char *command, char **argv);
void retr(ftp_cmd_socket_t *this, char *command, char **argv);
void stor(ftp_cmd_socket_t *this, char *command, char **argv);
void syst(ftp_cmd_socket_t *this, char *command, char **argv);
void help(ftp_cmd_socket_t *this, char *command, char **argv);
void launch_ftp_command(ftp_cmd_socket_t *socket, char *command, char **argv);

#endif //NWP_MYFTP_2018_COMMANDS_H
