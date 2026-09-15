/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 00:39:45 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/01/21 17:52:44 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t len)
{
	unsigned char		*t;
	unsigned const char	*f;
	size_t				i;

	i = 0;
	if (!dst && !src)
		return ((unsigned char *)dst);
	t = dst;
	f = src;
	while (i < len)
	{
		t[i] = f[i];
		i++;
	}
	return ((unsigned char *)dst);
}
