/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 17:34:33 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/06/02 21:37:05 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substring;

	if (!s)
		return (NULL);
	if (start > ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s))
		len = ft_strlen(s);
	substring = (char *)malloc((len + 1) * sizeof(char));
// 	if (!substring)
// 		malloc_err(!substring, "|$*malloc*$|. Thanks for attention");
	ft_strlcpy(substring, s + start, len + 1);
	return (substring);
}
