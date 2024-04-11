/*
** EPITECH PROJECT, 2024
** B-PSU-200-LIL-2-1-minishell1-matisse.dufour
** File description:
** parsing.c
*/

#include "my.h"
#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static int process_separator(char sep, garbage_t *garbage, token_t **token_list)
{
    int i = 0;

    for (i = 0; f_tab[i] && sep != f_tab[i].sep; i++);
    if (f_tab[i].sep == 0)
        return EXIT_FAILURE;
    f_tab[i].(*feature)(garbage, token_list);
    return EXIT_SUCCESS;
}

static int parsing_function(garbage_t *garbage, token_t **token_list)
{
    token_t *token = *token_list;
    int index = 0;

    if (!token)
        return -1;
    for (; token; token = token->next) {
        if (token->sep && token->sep != ' ') {
            execute_command(garbage, *token_list, index);
            return process_separator(token->sep, garbage, &(token->next));
        }
        index = token->index;
    }
}
