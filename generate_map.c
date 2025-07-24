/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbuscaro <lbuscaro@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:43:36 by lbuscaro          #+#    #+#             */
/*   Updated: 2025/07/09 17:53:45 by lbuscaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static char	*store_content(char *file);
static int	create_map(t_map *map, char *map_file);
static int	get_points(t_map *map, char *map_file);
static int	save_point(t_map *map, char *map_file, int i, int j);

void	load_file_data(t_fdf *fdf, char *file)
{
	fdf->file_content = store_content(file);
	if (!fdf->file_content)
		close_fdf(fdf, "Invalid file");
	if (!create_map(&fdf->map, fdf->file_content))
	{
		free(fdf->file_content);
		close_fdf(fdf, "Fail to generate map");
	}
	if (!get_points(&fdf->map, fdf->file_content))
	{
		free(fdf->file_content);
		close_fdf(fdf, "Fail to save the point");
	}
	free(fdf->file_content);
}

static char	*store_content(char *file)
{
	int		fd;
	int		br;
	ssize_t	size;
	char	fs[BUFFER];
	char	*content;

	fd = open(file, O_RDONLY);
	size = read(fd, fs, 1);
	if (fd < 0 || size != 1)
		return (NULL);
	br = 1;
	while (br > 0)
	{
		br = read(fd, fs, BUFFER);
		size += br;
	}
	fd = close(fd);
	content = (char *)malloc(sizeof(char) * (size + 1));
	fd = open(file, O_RDONLY);
	if (fd < 3 || read(fd, content, size) < size)
		return (free(content), NULL);
	fd = close(fd);
	content[size] = '\0';
	return (content);
}

static int	create_map(t_map *map, char *content)
{
	int	x;

	x = 0;
	while (content[0])
	{
		while (content[0] == ' ')
			content++;
		while (content[0] != ' ' && content[0] != '\n')
			content++;
		map->size_x++;
		if (content[0] == '\n')
		{
			if (x == 0)
				x = map->size_x;
			if (x != map->size_x)
				return (0);
			map->size_x = 0;
			map->size_y++;
		}
		content++;
	}
	map->size_x = x;
	map->area = map->size_x * map->size_y;
	map->point = ft_calloc(map->area, sizeof(t_point));
	return (1);
}

static int	get_points(t_map *map, char *content)
{
	int		i;
	int		j;
	int		c;

	c = 0;
	i = 0;
	while (i < map->size_y)
	{
		j = 0;
		while (j < map->size_x)
		{
			c = save_point(map, content, i, j);
			content += c;
			j++;
		}
		i++;
	}
	return (1);
}

static int	save_point(t_map *map, char *content, int i, int j)
{
	int	c;

	c = 0;
	while (content[c] == ' ' || content[c] == '\n')
		c++;
	map->point[j + (i * map->size_x)].ax = j;
	map->point[j + (i * map->size_x)].ay = i;
	map->point[j + (i * map->size_x)].az = ft_atoi(&content[c]);
	while (content[c] >= '0' && content[c] <= '9')
		c++;
	if (content[c] == ',')
		c++;
	map->point[j + (i * map->size_x)].color = set_color(&content[c]);
	while (content[c] != ' ' && content[c] != '\n')
		c++;
	return (c);
}
