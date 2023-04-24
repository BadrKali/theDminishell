#include "../../minishell.h"

static int	count_delimiters(char *s, char c, int len)
{
	int	i;
	int	sum;
	int	is_delimiter;

	is_delimiter = -1;
	sum = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && i < len && is_delimiter < 0)
		{
			sum++;
			is_delimiter = 0;
		}
		else if (s[i] == c && is_delimiter >= 0)
			is_delimiter = -1;
		i++;
	}
	return (sum);
}

static char	*split_array(char *s, int start, int end)
{
	char	*ptr;
	int		i;

	i = 0;
	ptr = malloc((end - start + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	while (start < end)
	{
		ptr[i] = s[start++];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

static	void	*free_memory(char **res)
{
	int	j;

	j = 0;
	while (res[j] != NULL)
		j++;
	while (j--)
		free(res[j]);
	free(res);
	return (NULL);
}

static char	**split(char *s, char c, char **res, int len)
{
	int		i;
	int		j;
	int		start;
	char	*str;

	i = 0;
	j = 0;
	start = -1;
	while (i <= len)
	{
		if (s[i] != c && start < 0)
			start = i;
		else if ((s[i] == c || i == len) && start >= 0)
		{
			str = split_array(s, start, i);
			if (!str)
				return (free_memory(res));
			else
				res[j++] = str;
			start = -1;
		}
		i++;
	}
	res[j] = 0;
	return (res);
}

char	**ft_split(char *s, char c)
{
	char	**res;
	int		len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	res = malloc((count_delimiters(s, c, len) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	return (split(s, c, res, len));
}