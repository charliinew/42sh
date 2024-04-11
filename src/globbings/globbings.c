/*
** EPITECH PROJECT, 2024
** B-PSU-200-LIL-2-1-42-matisse.dufour
** File description:
** fork.c
*/

#include "minishell.h"

char **make_globbings(char *str)
{
    glob_t glob_result;
    int error = glob(str, GLOB_NOCHECK, NULL, &glob_result);
    int len = 0;
    char **arr = 0;

    for (; glob_result.gl_pathv[len] != NULL; len++);
    arr = malloc(sizeof(char *) * (len + 1));
    for (int i = 0; i < len; i++)
        arr[i] = strdup(glob_result.gl_pathv[i]);
    arr[len] = NULL;
    globfree(&glob_result);
    return arr;
}

arg_t *create_new_arg(arg_t *head, char **new_arg, arg_t **actual)
{
    arg_t *new_list = NULL;
    arg_t *current = NULL;
    arg_t *new_node = NULL;

    for (int i = 0; new_arg[i] != NULL; i++) {
        new_node = malloc(sizeof(arg_t));
        new_node->str = new_arg[i];
        new_node->next = NULL;
        if (new_list == NULL) {
            new_list = new_node;
            current = new_node;
        } else {
            current->next = new_node;
            current = current->next;
        }
    }
    current->next = head->next;
    *actual = current;
    free(head);
    return new_list;
}

int globbings(arg_t **arg)
{
    arg_t *head = *arg;
    arg_t *prev = NULL;
    arg_t *current = NULL;
    char **new_arg = make_globbings(head->str);

    if (new_arg) {
        *arg = create_new_arg(head, new_arg, &current);
        free(new_arg);
    }
    while (current && current->next) {
        new_arg = make_globbings(current->next->str);
        current->next = create_new_arg(current->next, new_arg, &current);
        free(new_arg);
    }
    return 0;
}
