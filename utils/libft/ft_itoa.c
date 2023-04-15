/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abahsine <abahsine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 17:18:45 by abahsine          #+#    #+#             */
/*   Updated: 2023/04/15 17:18:45 by abahsine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	count_length(int n)
{
	int	i;

	i = 0;
	while (n)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static char	*is_positive(int temp, int temp2, int len, char *result)
{
	int	i;

	i = 0;
	result = malloc((len + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (temp2)
	{
		temp = temp2 % 10;
		temp2 = temp2 / 10;
		result[--len] = temp + 48;
		i++;
	}
	result[i] = '\0';
	return (result);
}

static char	*is_negative(int temp, int temp2, int len, char *result)
{
	int	i;

	i = 0;
	temp2 *= -1;
	result = malloc((len + 2) * sizeof(char));
	if (!result)
		return (NULL);
	while (temp2)
	{
		if (i == 0)
			result[0] = '-';
		else
		{
			temp = temp2 % 10;
			temp2 = temp2 / 10;
			result[len--] = temp + 48;
		}
		i++;
	}
	result[i] = '\0';
	return (result);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		temp2;
	int		temp;
	int		len;

	temp = 0;
	temp2 = n;
	result = NULL;
	len = count_length(n);
	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n < 0)
		return (is_negative(temp, temp2, len, result));
	else
		return (is_positive(temp, temp2, len, result));
}