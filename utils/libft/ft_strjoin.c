/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bel-kala <bel-kala@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 19:34:05 by abahsine          #+#    #+#             */
/*   Updated: 2023/05/14 12:08:53 by bel-kala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"


// char	*ft_strjoin(char *s1, char *s2)
// {
// 	char	*str_join;
// 	size_t	len;
// 	size_t	i;
// 	size_t	j;
	
// 	if (!s2)
// 		return (NULL);
// 	if (!s1)
// 		ft_strdup("");
// 	j = 0;
// 	i = 0;
// 	len = ft_strlen(s1) + ft_strlen(s2);
// 	str_join = malloc(len + 1 * sizeof(char));
// 	if (!str_join)
// 		return (NULL);
// 	while (s1[i] != '\0')
// 	{
// 		printf("hello\n");
// 		str_join[i] = s1[i];
// 		i++;
// 	}
// 	while (s2[j] != '\0')
// 		str_join[i++] = s2[j++];
// 	str_join[i] = '\0';
// 	return (free(s1), str_join);
// }

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		k;
	char	*new;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	i = 0;
	k = 0;
	new = malloc(sizeof (char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (new == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		new[i] = s1[i];
		i ++;
	}
	while (s2[k] != '\0')
	{
		new[i] = s2[k];
		k ++;
		i ++;
	}
	new[i] = '\0';
	return (free(s1),new);
}