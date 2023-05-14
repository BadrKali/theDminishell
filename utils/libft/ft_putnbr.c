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