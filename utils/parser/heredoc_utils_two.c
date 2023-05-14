/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_two.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 13:17:39 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/14 15:10:41 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	clear_heredocs_utils(char *buffer, char *input, int fd)
{
	ft_putstr_fd(buffer, fd);
	free(buffer);
	close(fd);
}

char	*read_heredoc_input_two(t_tokens **token, t_envp *envp, char *input,
		char *buffer)
{
	char	*delimiter;

	delimiter = get_delimiter(*token);
	while (1 && globale.heredoc != 1)
	{
		input = readline("> ");
		if (!input)
		{
			if (globale.heredoc == 1)
				if (dup2(globale.heredoc_tmp, 0) == -1)
					return (ft_putstr_fd("dup2 failed\n", 2), NULL);
			break ;
		}
		if (ft_strcmp(input, delimiter))
			break ;
		if (!check_delimiter_type(*token) && check_is_joined(*token))
			input = handle_heredoc_variables(input, envp);
		buffer = ft_strjoin(buffer, input);
		buffer = ft_strjoin(buffer, "\n");
		free(input);
	}
	free(input);
	return (buffer);
}

void	read_heredoc_input(t_tokens **token, t_envp *envp, int fd)
{
	char	*input;
	char	*buffer;

	input = NULL;
	signal(SIGINT, heredoc_signal_handler);
	buffer = read_heredoc_input_two(token, envp, input, ft_strdup(""));
	clear_heredocs_utils(buffer, input, fd);
}
