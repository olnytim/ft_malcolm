/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 19:57:09 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/02/18 15:25:48 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const*s1, char const*set)
{
	size_t	len;
	size_t	i;

	if (!s1 || !set)
		return (NULL);
	len = ft_strlen(s1) - 1;
	i = 0;
	while (s1[i] && ft_strchr(set, s1[i]))
		++i;
	while (ft_strchr(set, s1[len]) && len > i)
		--len;
	return (ft_substr(s1, i, len - i + 1));
}
