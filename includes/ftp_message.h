/*
** EPITECH PROJECT, 2019
** NWP_myftp_2018
** File description:
** ftp_message.h
*/

#ifndef NWP_MYFTP_2018_FTP_MESSAGE_H
#define NWP_MYFTP_2018_FTP_MESSAGE_H

#define HELLO "220 Service ready for new user.\n"
#define PERMISSION_DENIED "530 Permission denied.\n"
#define CANT_CHANGE_USER "530 Can't change user.\n"
#define SPECIFY_PASSWORD "331 User name okay, need password.\n"
#define USER_LOGGED_IN "230 User logged in, proceed.\n"
#define ALREADY_LOGGED_IN "230 Already logged in.\n"
#define USER_FIRST "503 Login with USER first.\n"
#define BAD_LOGIN "530 Login incorrect.\n"
#define CHANGE_DIR_OK "250 Directory successfully changed.\n"
#define CHANGE_DIR_FAILED "550 Failed to change directory.\n"
#define PLEASE_LOGIN "530 Please login with USER and PASS.\n"
#define UNKNOW_COMMAND "500 Unknown command.\n"
#define COMMAND_OK "200 Command okay.\n"
#define ERROR_PASV "520 PASV impossible to create data socket.\n"
#define ERROR_PORT "520 Illegal PORT command.\n"
#define PORT_OK "200 PORT command sucess.\n"
#define NEED_DATA_CHANNEL "425 Use PORT or PASV first.\n"
#define DATA_TRANSFER_FAILED "425 Failed to establish connection.\n"
#define SEND_LIST_DIR "150 Here comes the directory listing.\n"
#define SEND_LIST_OK "226 Directory send OK.\n"
#define FILE_NOT_FOUND "550 File not found.\n"
#define FILE_DELETED "250 File deleted.\n"
#define OPEN_FAILED "550 Failed to open file.\n"
#define START_DATA_TRANFERT "150 Opening BINARY mode data connection.\n"
#define DATA_TRANSFER_OK "226 Transfer complete.\n"
#define CLIENT_CLOSE "221 Service closing control connection.\n"

#endif //NWP_MYFTP_2018_FTP_MESSAGE_H
