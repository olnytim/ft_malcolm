/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 18:50:19 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/02/17 16:27:51 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = ft_strlen(src);
	if (!dstsize)
		return (i);
	if (i >= dstsize)
	{
		ft_memcpy(dst, src, (dstsize - 1));
		dst[dstsize - 1] = 0;
	}
	else
		ft_memcpy(dst, src, (i + 1));
	return (i);
}
