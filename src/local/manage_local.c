/*
** EPITECH PROJECT, 2024
** manage
** File description:
** manage
*/

#include "my.h"
#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static int del_var(char *name, garbage_t *garbage)
{
    var_t *current = garbage->local;
    var_t *prev = NULL;

    for (; current != NULL; current = current->next) {
        if (strcmp(name, current->var) == 0) {
            delete_var(current, prev, garbage);
            return 1;
        }
        prev = current;
    }
    return 1;
}

int var_len(garbage_t *garbage)
{
    var_t *current = garbage->local;
    int compt = 0;
    var_t *prev = NULL;
    var_t *next;

    if (garbage == NULL || current == NULL)
        return 0;
    for (; current; current = current->next) {
        compt++;
        if (compt > 1000)
            del_var(current->var, garbage);
        prev = current;
    }
    return compt;
}

token_t *insert_node(token_t *token, char *com,
    garbage_t *garbage, pipeline_t *pipeline)
{
    char temp[my_strlen(com) + 2];
    token_t **insert;
    token_t *current;

    my_strcpy(temp, com);
    temp[my_strlen(com)] = '\n';
    temp[my_strlen(com) + 1] = '\0';
    insert = init_token_list(temp);
    if (token->prev != NULL)
        token->prev->next = *insert;
    else
        pipeline->token_list = insert;
    current = *insert;
    for (; current->next->next; current = current->next);
    current->next = token->next;
    token->next->prev = current;
    token = *insert;
    reset_index(pipeline);
    return token;
}

static pipeline_t *format_token(garbage_t *garbage, pipeline_t *pipeline)
{
    if (pipeline->token_list == NULL)
        return pipeline;
    for (token_t *current = *pipeline->token_list;
        current && current->index <=
            get_token_list_size(*pipeline->token_list);
            current = current->next) {
            manage_variable(current, garbage, pipeline);
    }
    return pipeline;
}

void format_variable(garbage_t *garbage, pipeline_t **pip)
{
    pipeline_t *pipeline = *pip;

    if (pipeline == NULL)
        return;
    for (; pipeline; pipeline = pipeline->next)
        pipeline = format_token(garbage, pipeline);
}
