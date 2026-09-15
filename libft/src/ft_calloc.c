/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 14:55:34 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/01/28 17:26:12 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*temp;

	if (count >= SIZE_MAX || size >= SIZE_MAX || count * size >= __INT_MAX__)
		return (0);
	temp = malloc(count * size);
	if (!temp)
		return (0);
	temp = ft_memset(temp, 0, (count * size));
	return (temp);
}
