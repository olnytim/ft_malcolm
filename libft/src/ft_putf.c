/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putf.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 20:58:37 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/05/05 20:04:48 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putnbr(int n)
{
	int	counter;

	counter = 0;
	if (n == -2147483648)
	{
		counter += ft_putchar('-');
		counter += ft_putchar('2');
		n = 147483648;
	}
	if (n < 0)
	{
		counter += ft_putchar('-');
		n = -n;
	}
	if (n > 9)
	{
		counter += ft_putnbr(n / 10);
		counter += ft_putnbr(n % 10);
	}
	else
		counter += ft_putchar(n + 48);
	return (counter);
}

int	ft_putnbr_uns(unsigned int n)
{
	int	counter;

	counter = 0;
	if (n > 9)
	{
		counter += ft_putnbr(n / 10);
		counter += ft_putnbr(n % 10);
	}
	else
		counter += ft_putchar(n + 48);
	return (counter);
}

int	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (!s)
	{
		i += (write(1, "(null)", 6));
		return (i);
	}
	while (s[i])
		write(1, &s[i++], 1);
	return (i);
}
