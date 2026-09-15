/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/21 15:08:38 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/01/21 17:07:05 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*t;
	unsigned const char	*f;

	if (!dst && !src)
		return (dst);
	t = dst;
	f = src;
	if (dst < src)
		ft_memcpy(dst, src, len);
	else
	{
		while (len--)
			*(t + len) = *(f + len);
	}
	return ((unsigned char *)dst);
}
