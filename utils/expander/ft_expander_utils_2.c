/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expander_utils_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 21:28:04 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/12 21:32:03 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_is_only_space(char *value)
{
	int	i;

	i = 0;
	while (value[i] && (value[i] == ' ' || value[i] == '\t'))
		i++;
	if (value[i] == '\0')
		return (1);
	return (0);
}

void	ft_initialize_vars(t_utils *utils)
{
	utils->i = 0;
	utils->k = 0;
	utils->j = 0;
	utils->start = 0;
}