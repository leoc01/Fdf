#include "fdf.h"

void	get_points(t_point *point, t_map *map, char *file)
{
	char	*line;
	char 	**row;
	int		i;
	int		j;
	int		fd;
	
	fd = open(file, O_RDONLY);
	line = get_next_line(fd);
	j = 0;
	while (line)
	{
		row = ft_split(line, ' ');
		free(line);
		i = 0;
		while (i < map->size_x)
		{
			point[i + (j * map->size_x)].ax = i;
			point[i + (j * map->size_x)].ay = j;
			point[i + (j * map->size_x)].az = ft_atoi(row[i]);
			i++;
		}
		j++;
		line = get_next_line(fd);
	}
	close(fd);
}

t_map	create_map(char *file)
{
	t_map	map;
	char	*line;
	char 	**row;
	int		fd;
	
	fd = open(file, O_RDONLY);
	map.size_x = 0;
	map.size_y = 0;
	line = get_next_line(fd);
	row = ft_split(line, ' ');
	while (row[map.size_x])
		map.size_x++;
	while (line)
	{
		line = get_next_line(fd);
		free(line);
		map.size_y++;
	}
	close(fd);
	map.area = map.size_x * map.size_y;
	map.point = ft_calloc(map.size_x * map.size_y, sizeof(t_point));
	get_points(map.point, &map, file);
	return (map);
}
