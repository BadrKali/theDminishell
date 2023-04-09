/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expansion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 17:47:52 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/09 17:50:10 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../minishell.h"

// void	ft_expand_vars(t_tokens **token)
// {
// 	while (*token)
// 	{
// 		if ((*token)->type == VAR)
// 			(*token)->value = ft_strdup(getenv((*token)->value));
// 		*token = (*token)->next;
// 	}
// }