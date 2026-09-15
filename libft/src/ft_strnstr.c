/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 22:59:49 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/01/28 00:24:39 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;

	if (!*needle)
		return ((char *)haystack);
	i = ft_strlen(needle);
	while (len)
	{
		if (!*haystack || i > len--)
			return (0);
		if (!ft_strncmp(haystack, needle, i))
			return ((char *)haystack);
		haystack++;
	}
	return (0);
}
