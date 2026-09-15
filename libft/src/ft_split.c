/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 20:30:35 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/02/17 22:25:37 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	colnum(const char *str, char c)
{
	int	counter;
	int	i;

	counter = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] == c)
			i++;
		if (str[i])
			counter++;
		while (str[i] != c && str[i])
			i++;
	}
	return (counter);
}

static char	**fill_matrix(const char *str, char c, char **matrix, int word)
{
	int	i;
	int	j;
	int	start;
	int	end;

	i = 0;
	j = 0;
	while (j < word)
	{
		while (str[i] == c)
			i++;
		start = i;
		while (str[i] != c)
		{
			if (!str[i])
				break ;
			i++;
		}
		end = i;
		if (end > start)
			matrix[j] = ft_substr(str, start, end - start);
		j++;
	}
	matrix[j] = NULL;
	return (matrix);
}

char	**ft_split(char const *s, char c)
{
	char	**matrix;
	int		i;

	i = colnum(s, c);
	matrix = (char **)malloc(sizeof(char *) * (i + 1));
	if (!matrix)
		return (NULL);
	return (fill_matrix(s, c, matrix, i));
}
