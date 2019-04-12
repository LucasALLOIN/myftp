/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** pass.c
*/

#include <ftp_message.h>
#include <string.h>
#include <stdio.h>
#include "ftp.h"

extern const user_t user_tab[];

int pass_base(ftp_cmd_socket_t *this)
{
    if (this->user == NULL) {
        write(this->socket, USER_FIRST, strlen(USER_FIRST));
        return (1);
    }
    if (this->logged) {
        write(this->socket, ALREADY_LOGGED_IN, strlen(ALREADY_LOGGED_IN));
        return (1);
    }
    if (this->user->username == NULL) {
        this->user = NULL;
        write(this->socket, BAD_LOGIN, strlen(BAD_LOGIN));
        return (1);
    }
    return (0);
}

void pass_null_pass(ftp_cmd_socket_t *this, char **argv)
{
    if (tab_length(argv) != 1) {
        this->user = NULL;
        write(this->socket, BAD_LOGIN, strlen(BAD_LOGIN));
        return;
    }
    write(this->socket, USER_LOGGED_IN, strlen(USER_LOGGED_IN));
    this->logged = 1;
    if (strcasecmp(this->user->username, "anonymous") == 0) {
        chdir(this->master->def_path);
        getcwd(this->work_dir, 256);
        return;
    }
    if (chdir(this->user->root_dir) == 0) {
        getcwd(this->work_dir, 256);
    } else
        chdir(this->work_dir);
}

void pass_check(ftp_cmd_socket_t *this)
{
    write(this->socket, USER_LOGGED_IN, strlen(USER_LOGGED_IN));
    this->logged = 1;
    if (strcasecmp(this->user->username, "anonymous") == 0) {
        chdir(this->master->def_path);
        getcwd(this->work_dir, 256);
        return;
    }
    if (chdir(this->user->root_dir) == 0) {
        getcwd(this->work_dir, 256);
    } else
        chdir(this->work_dir);
}

void pass(ftp_cmd_socket_t *this, char *command, char **argv)
{
    (void) command;
    if (pass_base(this))
        return;
    if (this->user->password == NULL) {
        pass_null_pass(this, argv);
        return;
    }
    if (strcmp(this->user->password,
    (tab_length(argv) == 1) ? "" : argv[1]) == 0) {
        pass_check(this);
        return;
    }
    this->user = NULL;
    write(this->socket, BAD_LOGIN, strlen(BAD_LOGIN));
}