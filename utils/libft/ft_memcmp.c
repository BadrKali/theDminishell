#include"../../minishell.h"

int    ft_memcmp(const void *str1, const void *str2, size_t n)
{
        size_t                  i;
        unsigned char   *s1;
        unsigned char   *s2;

        s1 = (unsigned char *)str1;
        s2 = (unsigned char *)str2;
        i = 0;
        while (i <= n)
        {
                if (s1[i] != s2[i])
                    return(1);
                i ++;
        }
        return (0);
}