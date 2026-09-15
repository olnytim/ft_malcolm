/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/31 18:47:59 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/02/18 19:03:27 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	numlen(int num)
{
	int		i;
	long	numl;

	i = 0;
	numl = num;
	if (numl < 0)
		numl *= -1;
	while (numl > 9)
	{
		numl /= 10;
		++i;
	}
	++i;
	return (i);
}

static char	ito(int n)
{
	char	s;

	s = 0;
	s = (n % 10) + 48;
	return (s);
}

static char	*nbr(int num, char *temp, int len)
{
	temp[len] = '\0';
	if (num == 0)
		temp[0] = '0';
	if (num == -2147483648)
	{
		temp[0] = '-';
		temp[1] = '2';
		num = 147483648;
	}
	if (num < 0)
	{
		temp[0] = '-';
		num *= -1;
	}
	--len;
	while (num > 0)
	{
		temp[len] = ito(num);
		num /= 10;
		len--;
	}
	return (temp);
}

char	*ft_itoa(int n)
{
	char	*s;
	int		i;
	long	numl;

	numl = n;
	if (n < 0)
		i = numlen(numl) + 1;
	else
		i = numlen(numl);
	s = malloc(sizeof(char) * (i + 1));
	if (!s)
		return (NULL);
	s = nbr(numl, s, i);
	return (s);
}
