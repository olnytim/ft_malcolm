/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_conversions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 22:42:47 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/05/05 21:04:57 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	hex_lower(unsigned int num)
{
	int		counter;

	counter = 0;
	if (num > 15)
		counter = hex_lower(num / 16);
	counter++;
	ft_putchar("0123456789abcdef"[num % 16]);
	return (counter);
}

static int	hex_lower_pointer(unsigned long long num)
{
	int					counter;
	unsigned long long	val;	

	val = (unsigned long long)num;
	counter = 0;
	if (val > 15)
		counter = hex_lower_pointer(val / 16);
	counter++;
	ft_putchar("0123456789abcdef"[val % 16]);
	return (counter);
}

int	hex_upper(unsigned int num)
{
	int		counter;

	counter = 0;
	if (num > 15)
		counter = hex_upper(num / 16);
	counter++;
	ft_putchar("0123456789ABCDEF"[num % 16]);
	return (counter);
}

int	pointer(unsigned long long num)
{
	int	counter;

	counter = 0;
	counter += ft_putstr("0x");
	counter += hex_lower_pointer(num);
	return (counter);
}
