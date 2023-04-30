/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 19:34:05 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/30 16:24:10 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_strjoin(char *s1, char *s2)
{
	char	*str_join;
	size_t	len;
	size_t	i;
	size_t	j;

	if (!s2)
		return (NULL);
	if (!s1)
		ft_strdup("");
	j = 0;
	i = 0;
	len = ft_strlen(s1) + ft_strlen(s2);
	str_join = malloc((len + 1) * sizeof(char));
	if (!str_join)
		return (NULL);
	while (s1[i])
	{
		str_join[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str_join[i++] = s2[j];
		j++;
	}
	str_join[i] = '\0';
	return (free(s1), str_join);
}
