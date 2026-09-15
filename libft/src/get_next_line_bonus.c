/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:17:35 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/05/09 21:38:15 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_save(char *save)
{
	int		integer;
	char	*string;
	char	*temp;

	integer = 0;
	while (save[integer] && save[integer] != '\n')
		++integer;
	if (!save[integer])
	{
		free(save);
		return (NULL);
	}
	string = (char *)malloc(ft_strlen_gnl(save) - integer + 1);
	if (!string)
		return (NULL);
	++integer;
	temp = string;
	while (save[integer])
		*string++ = save[integer++];
	*string = '\0';
	free(save);
	return (temp);
}

static char	*ft_gnl(char *save)
{
	int		integer;
	char	*string;
	char	*temp;

	if (!*save)
		return (NULL);
	integer = 0;
	while (save[integer] && save[integer] != '\n')
		++integer;
	string = (char *)malloc(integer + 2);
	if (!string)
		return (NULL);
	temp = string;
	while (*save && *save != '\n')
		*string++ = *save++;
	if (*save == '\n')
		*string++ = *save++;
	*string = '\0';
	return (temp);
}

static char	*ft_targ_save(int fd, char *save)
{
	int		amount;
	char	*buff_s;

	buff_s = (char *)malloc(BUFFER_SIZE + 1);
	if (!buff_s)
		return (NULL);
	amount = 1;
	while (!ft_strchr_gnl(save, '\n') && amount != 0)
	{
		amount = read(fd, buff_s, BUFFER_SIZE);
		if (amount == -1)
		{
			free(buff_s);
			return (NULL);
		}
		buff_s[amount] = '\0';
		save = ft_strjoin_gnl(save, buff_s);
	}
	free(buff_s);
	return (save);
}

char	*get_next_line(int fd)
{
	char		*string;
	static char	*save[1000000];

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > 1000000)
		return (0);
	save[fd] = ft_targ_save(fd, save[fd]);
	if (!save[fd])
		return (NULL);
	string = ft_gnl(save[fd]);
	save[fd] = ft_save(save[fd]);
	return (string);
}
