/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 19:42:29 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/05/05 20:05:19 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	check_conversions(va_list ars, const char *str, int *counter)
{
	if (*str == 'c')
		*counter += ft_putchar(va_arg(ars, int));
	else if (*str == 's')
		*counter += ft_putstr(va_arg(ars, char *));
	else if (*str == 'p')
		*counter += pointer(va_arg(ars, unsigned long long));
	else if (*str == 'd' || *str == 'i')
		*counter += ft_putnbr(va_arg(ars, int));
	else if (*str == 'u')
		*counter += ft_putnbr_uns(va_arg(ars, unsigned int));
	else if (*str == 'x')
		*counter += hex_lower(va_arg(ars, unsigned int));
	else if (*str == 'X')
		*counter += hex_upper(va_arg(ars, unsigned int));
	else if (*str == '%')
		*counter += ft_putchar('%');
	else
		*counter += write(1, &str, 1);
}

int	ft_printf(const char *str, ...)
{
	va_list	ars;
	int		counter;
	int		iterator;

	va_start(ars, str);
	counter = 0;
	iterator = 0;
	while (str[iterator])
	{
		if (str[iterator] == '%' && str[iterator + 1])
			check_conversions(ars, &str[iterator++ + 1], &counter);
		else
			counter += write(1, &str[iterator], 1);
		++iterator;
	}
	va_end(ars);
	return (counter);
}
