#include"../../minishell.h"


void close_fds(int index,int cmd_num,int ends[][2])
{
    int i;

    i = 0;
    while(i < cmd_num - 1)
    {
        if(i != index)
            close(ends[i][1]);
        if(i != index - 1)
            close(ends[i][0]);
        i++;
    }
}

void dup_fds(t_cmds *cmd,int ends[][2],int index,int cmd_num)
{
    close_fds(index,cmd_num,ends);
    if(cmd->prev == NULL)
        dup2(ends[index][1],STDOUT_FILENO);
    else if(cmd->next != NULL)
    {
        dup2(ends[index][1],STDOUT_FILENO);
        close(ends[index][1]);
        dup2(ends[index - 1][0],STDIN_FILENO);
        close(ends[index - 1][0]);
    }
    else
    {
        dup2(ends[index - 1][0],STDIN_FILENO);
        close(ends[index - 1][0]);
    }
        
}

void multi_command_exec(t_cmds *cmd,t_envp **env,int cmd_num)
{
    int ends[cmd_num - 1][2];
    int i;

    i = 0;
    while(i < cmd_num - 1)
    {
        pipe(ends[i]);
        i++;
    }
    i = 0;
    while(cmd != NULL)
    {
        cmd->pid = fork();
        if(cmd->pid < 0)
        {
            ft_putstr_fd("FORK FAILED\n",2);
            return;
        }   
        if(cmd->pid == 0)
        {
            dup_fds(cmd,ends,i,cmd_num);
            if(cmd->cmd[0] == '.' && cmd->cmd[1] == '/')
            {
                if(check_handler(cmd) == 1)
                    exit(126);
            }
            single_cmd_handler(cmd,env);
        }
        i++;
        cmd = cmd->next;
    }
    close(ends[0][1]);
    close_fds(0,cmd_num,ends);
}