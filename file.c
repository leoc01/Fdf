/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 15:20:09 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/22 15:20:11 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

char	*store_content(char *file)
{
	int		fd;
	int		br;
	size_t	size;
	char	fs[BUFFER];
	char	*content;

	fd = open(file, O_RDONLY);
	br = read(fd, &fs, BUFFER);
	size = br;
	while (br > 0)
	{
		size += br;
		br = read(fd, fs, BUFFER);
	}
	fd = close(fd);
	content = (char *)malloc(sizeof(char) * (size + 1));
	fd = open(file, O_RDONLY);
	read(fd, content, size);
	fd = close(fd);
	content[size] = '\0';
	return (content);
}
