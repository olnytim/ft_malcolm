/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 16:13:26 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/02/18 18:56:18 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;

	if ((!dst || !src) && !dstsize)
		return (0);
	i = ft_strlen(dst);
	j = ft_strlen(src);
	if (i >= dstsize)
		return (dstsize + j);
	if ((dstsize - i) <= j)
	{
		ft_memcpy((dst + i), src, dstsize - i - 1);
		dst[dstsize - 1] = 0;
	}
	else
		ft_memcpy((dst + i), src, (j + 1));
	return (i + j);
}
