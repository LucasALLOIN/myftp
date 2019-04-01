/*
** EPITECH PROJECT, 2018
** my_str_to_array
** File description:
** my_str_to_array
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <ftp.h>

char **my_str_to_array(char *cmd, char to_s)
{
	int z = 0;
	int y = 0;
	char **res = malloc(sizeof(char *) * (count_to_split(cmd, to_s) + 1));
	char *st = malloc(sizeof(char) * (strlen(cmd)) + 1);

	memset(st, 0, strlen(cmd) + 1);
	for (int i = 0; cmd[i]; i = i + 1) {
		if (cmd[i] != to_s) {
			st[z] = cmd[i];
			z = z + 1;
		}
		if (cmd[i] == to_s || !cmd[i + 1]) {
			z = 0;
			res[y] = st;
			st = malloc(sizeof(char *) * (strlen(cmd)) + 1);
			memset(st, 0, strlen(cmd) + 1);
			y = y + 1;
		}
	}
	res[y] = 0;
	free(st);
	return (res);
}

unsigned int tab_length(char **argv)
{
	unsigned int i = 0;

	while (argv[i] != NULL)
		i++;
	return (i);
}