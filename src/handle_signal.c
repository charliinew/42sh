/*
** EPITECH PROJECT, 2024
** B-PSU-200-LIL-2-1-42sh-nicolas.dumetz
** File description:
** handle_signal.c
*/

#include <unistd.h>
#include "../include/minishell.h"

void sigint_handler(void)
{
    my_printf("\n");
    ttycheck();
}
