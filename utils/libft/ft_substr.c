/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 01:35:40 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/08 16:22:18 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*sub_str;
	size_t	i;
	size_t	length;

	if (!s)
		return (NULL);
	i = 0;
	length = ft_strlen(s);
	if (start >= length)
	{
		sub_str = malloc(1 * sizeof(char));
		if (!sub_str)
			return (NULL);
		return (sub_str);
	}
	if (len > length - start)
		sub_str = malloc((length - start + 1) * sizeof(char));
	else
		sub_str = malloc((len + 1) * sizeof(char));
	if (!sub_str)
		return (NULL);
	while (i < len && s[start])
		sub_str[i++] = s[start++];
	sub_str[i] = '\0';
	return (sub_str);
}
