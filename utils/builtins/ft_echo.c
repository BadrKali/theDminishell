/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/11 18:02:34 by bel-kala          #+#    #+#             */
/*   Updated: 2023/05/11 18:02:35 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include"../../minishell.h"

void	ft_putchar(int fd, char c)
{
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int n, int fd)
{
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
	}
	else if (n < 0)
	{
		n = n * (-1);
		ft_putchar(fd, '-');
		ft_putnbr_fd(n, fd);
	}
	else if (n < 10)
	{
		ft_putchar(fd, n + 48);
	}
	else if (n > 9)
	{
		ft_putnbr_fd((n / 10), fd);
		ft_putnbr_fd((n % 10), fd);
	}
}


void print_string(char *str)
{
    int i;

    i = 0;
    while(str[i] != '\0')
    {
        write(1,&str[i],1);
        i++; 
    }
}





int echo_flag(char *str)
{
    int i;

    i = 1;
    if(str[0] == '-')
    {
        if(str[i] == '\0')
            return(0);
        while(str[i] != '\0')
        {
            if(str[i] != 'n')
                return(0);
            i++;
        }   
    }
    else if(str[0] != '-')
        return(0);
    return(1);
}


int check_option(char **str)
{
    int i;
    int j;
    int option;

    i = 1;
    j = 0;
    option = 0;
    while(str[i] != NULL)
    {
        if(echo_flag(str[i]) == 0)
            break;
        option = option + echo_flag(str[i]);
        i++;
    }
    return(option);
}

int ft_echo(t_cmds *cmd)
{
    int option;
    int i;

    i = 1;
    option = 0;
    if(cmd->args[1] == NULL)
    {
        write(1,"\n",1);
        return(EXIT_SUCCESS);
    }
    option = check_option(cmd->args);
    i = option + 1;
    while(cmd->args[i] != NULL)
    {
        print_string(cmd->args[i]);
        if(cmd->args[i + 1] != NULL)
            write(1," ",1);
        i++;
    }
    if(option == 0)
        write(1,"\n",1);
    return(EXIT_SUCCESS);
}
