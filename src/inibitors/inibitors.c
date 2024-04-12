/*
** EPITECH PROJECT, 2024
** B-PSU-200-LIL-2-1-42sh-nicolas.dumetz
** File description:
** inibitors.c
*/

#include "minishell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void delete_node_string(token_t **head)
{
    token_t *next = 0;
    token_t *current = (*head)->next;

    for (; current->sep != '\"'; current = next) {
        if (current->arg)
            free(current->arg);
        next = current->next;
        free(current);
    }
    (*head)->next = next;
    free(current);
    if (next != NULL)
        next->prev = *head;
}

static void fill_string(token_t **head)
{
    token_t *fill = *head;

    for (token_t *current = fill->next; current->sep != '\"';
    current = current->next) {
        if (current->sep)
            strncat(fill->arg, &current->sep, 1);
        else
            strcat(fill->arg, current->arg);
    }
}

int get_string(token_t **head)
{
    token_t *current = (*head)->next;
    int len = 0;

    for (; current && current->sep != '\"'; current = current->next);
    if (current == NULL) {
        write(2, "Unmatched '\"'.\n", 15);
        return 1;
    }
    for (current = (*head)->next; current->sep != '\"';
    current = current->next)
        len += (current->sep == 0) ? strlen(current->arg) : 1;
    (*head)->sep = 0;
    free((*head)->arg);
    (*head)->arg = malloc(len + 1);
    (*head)->arg[0] = '\0';
    fill_string(head);
    delete_node_string(head);
    for (token_t *current = (*head)->next; current; current = current->next)
        current->index = current->prev->index;
    return 0;
}
