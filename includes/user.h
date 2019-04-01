/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** user.h
*/

#ifndef NWP_MYFTP_2018_USER_H
#define NWP_MYFTP_2018_USER_H

typedef struct user_s {
    char *username;
    char *password;
    char *root_dir;
} user_t;

#endif //NWP_MYFTP_2018_USER_H
