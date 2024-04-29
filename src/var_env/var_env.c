/*
** EPITECH PROJECT, 2024
** var env
** File description:
** var env
*/

#include "my.h"
#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static char *extract_var_name(const char *line)
{
    char *name = malloc(sizeof(char) * my_strlen(line));
    int i;

    if (name == NULL)
        return NULL;
    for (i = 0; line[i] && line[i] != '='; i++)
        name[i] = line[i];
    name[i] = '\0';
    return name;
}

static char *extract_var_value(const char *line)
{
    int i;

    for (i = 0; line[i] && line[i] != '='; i++);
    if (line[i] == '\0')
        return NULL;
    i++;
    return strdup(&line[i]);
}

static char *remove_quotes(const char *str)
{
    int len = my_strlen(str);
    char *new_str = malloc(sizeof(char) * (len + 1));
    int j = 0;
    int i = 0;

    for (i = 0; i < len; i++) {
        if (str[i] != '"' && str[i] != '\n') {
            new_str[j] = str[i];
            j++;
        }
    }
    new_str[j] = '\0';
    return new_str;
}

static char *check_var(char *line, token_t *token)
{
    char *arg_no_quotes = remove_quotes(token->arg);
    char *var_name = extract_var_name(line);
    char *var_value = extract_var_value(line);

    if (var_name == NULL || var_value == NULL) {
        free(var_name);
        free(var_value);
        free(arg_no_quotes);
        return NULL;
    }
    if (strcmp(arg_no_quotes + 1, var_name) == 0) {
        free(var_name);
        free(arg_no_quotes);
        return var_value;
    }
    free(var_name);
    free(var_value);
    free(arg_no_quotes);
    return NULL;
}

int check_varenv(token_t *token, garbage_t *garbage, pipeline_t *pipeline)
{
    char *value;

    if (token == NULL || token->arg == NULL || token->arg[0] != '$')
        return 0;
    for (int i = 0; garbage->env[0][i]; i++) {
        value = check_var(garbage->env[0][i], token);
        if (value != NULL) {
            insert_node(token, value, 1, pipeline);
            free(value);
            return 0;
        }
    }
    return 1;
}

token_t *manage_alias(token_t *token, garbage_t *garbage,
    pipeline_t *pipeline)
{
    if (token->arg) {
        if (check_alias_onpip(pipeline) == 0)
            token = check_alias(token, garbage, pipeline);
    }
    return token;
}

token_t *manage_variable(token_t *token, garbage_t *garbage,
    pipeline_t *pipeline)
{
    int ret;
    int env;

    if (token->arg) {
        ret = 0;
        env = 0;
        if (token->arg[0] == '$') {
            ret = check_local(token, garbage, pipeline);
            env = check_varenv(token, garbage, pipeline);
        }
        if (ret == 1 && env == 1) {
            fprintf(stderr, "%s: Undefined variable.\n", token->arg + 1);
            garbage->execute = 1;
        }
    }
    return token;
}
