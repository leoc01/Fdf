#include "fdf.h"

t_color	hex_to_color(char *n)
{
	char	*base = "0123456789ABCDEF";
	int		len;
	t_color	color;
	int		i;

	i = 2;
	len = ft_strlen(&n[i]);
	color.rgb = 0;
	if (n[0] != '0' || ft_toupper(n[1]) != 'X')
		color.rgb = 0X00FFFFFF;
	while (n[i] && n[i] != '\n')
	{
		color.rgb += (ft_strchr(base, ft_toupper(n[i])) - &base[0]) * pow(16, (len - 1));
		len--;
		i++;
	}
	if (len == 2)
		color.rgb = (color.rgb >> 16) & 0XFF;
	if (len == 4)
		color.rgb = (color.rgb >> 8) & 0XFFFF;
	color.r = rgb_from('r', color.rgb);
	color.g = rgb_from('g', color.rgb);
	color.b = rgb_from('b', color.rgb);
	return (color);
}

int	get_points(t_map *map, int fd)
{
	char	*row_line;
	char 	**row;
	int		i;
	int		j;
	char	**color;

	row_line = get_next_line(fd);
	j = 0;
	while (row_line)
	{
		row = ft_split(row_line, ' ');
		free(row_line);
		i = 0;
		while (i < map->size_x)
		{
			map->point[i + (j * map->size_x)].ax = i;
			map->point[i + (j * map->size_x)].ay = j;
			map->point[i + (j * map->size_x)].az = ft_atoi(row[i]);
			map->point[i + (j * map->size_x)].color = hex_to_color("0xFFFFFF");
			color = ft_split(row[i], ',');
			free(color[0]);
			if (color[1])
			{
				map->point[i + (j * map->size_x)].color = hex_to_color(color[1]);
				free(color[1]);
			}
			free(color);
			free(row[i]);
			i++;
		}
		free(row);
		j++;
		row_line = get_next_line(fd);
	}
	return (1);
}

int	create_map(t_map *map, int fd)
{
	char	*row;
	char 	**column;

	map->point = NULL;
	map->size_x = 0;
	map->size_y = 0;
	row = get_next_line(fd);
	column = ft_split(row, ' ');
	while (column[map->size_x])
		free(column[map->size_x++]);
	free(column);
	map->size_y++;
	while (row)
	{
		free(row);
		row = get_next_line(fd);
		map->size_y++;
	}
	map->area = map->size_x * map->size_y;
	map->point = ft_calloc(map->size_x * map->size_y, sizeof(t_point));
	return (1);
}
