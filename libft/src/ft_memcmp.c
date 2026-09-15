/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 21:11:44 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/01/27 21:43:47 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*scr;
	unsigned char	*dst;

	scr = (unsigned char *)s1;
	dst = (unsigned char *)s2;
	while (n--)
	{
		if (*scr != *dst)
			return (*scr - *dst);
		++scr;
		++dst;
	}
	return (0);
}
