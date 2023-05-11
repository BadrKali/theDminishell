#include"../../minishell.h"

void node_delete(t_envp **env,t_envp *node)
{
    t_envp *prev;
    t_envp *next;

    if(node->next == NULL)
    {
        prev = node->prev;
        prev->next = NULL;
        free(node);
        return;
    }
    if(node->prev == NULL)
    {
        (*env) = (*env)->next;
        free(node);
        return;
    }
    prev = node->prev;
    next = node->next;
    free(node);
    prev->next = next;
    next->prev = prev;
}

int check_syntax_unset(char *str)
{
    int i;

    i = 0;
    while(str[i] != '\0')
    {
        if(ft_isalpha(str[i]) == 0 && ft_isdigit(str[i]) == 0 && str[i] != '_')
            return(1);
        i++;
    }
    return(0);
}

int ft_unset(t_envp **env,t_cmds *cmd)
{
    int i;
    t_envp *tmp;

    i = 1;
    if(cmd->args[1] == NULL)
        return(EXIT_SUCCESS);
    while(cmd->args[i] != NULL)
    {
        if(check_syntax_unset(cmd->args[i]) == 0)
        {
            tmp = check_env_exist(cmd->args[i],*env);
            if(tmp != NULL)
                node_delete(env,tmp);
            i++;
        }
        else
        {
            ft_putstr_fd("Minishell: unset: '",1);
            ft_putstr_fd(cmd->args[i],1);
            ft_putstr_fd("': not a valid identifier\n",1);
            return(258);//check this error code 
        }
    }
    return(EXIT_SUCCESS);
}

//need to check the syntax before unset EX : unset shds**99KK 