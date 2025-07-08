#include "fdf.h"

void	get_points(t_map *map, char *file, t_fdf *fdf)
{
	char	*line;
	char 	**row;
	int		i;
	int		j;
	int		fd;
	char	**color;

	fd = open(file, O_RDONLY);
	if (fd < 3)
		close_fdf(fdf, 1);
	line = get_next_line(fd);
	j = 0;
	while (line)
	{
		row = ft_split(line, ' ');
		free(line);
		i = 0;
		while (i < map->size_x)
		{
			map->point[i + (j * map->size_x)].ax = i;
			map->point[i + (j * map->size_x)].ay = j;
			map->point[i + (j * map->size_x)].az = ft_atoi(row[i]);
			map->point[i + (j * map->size_x)].color = 0x00FFFFFF;
			color = ft_split(row[i], ',');
			free(color[0]);
			if (color[1])
			{
				printf("%s\n", color[1]);
				free(color[1]);
			}
			free(color);
			free(row[i]);
			i++;
		}
		free(row);
		j++;
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
}

void	create_map(t_fdf *fdf, char *file)
{
	char	*line;
	char 	**row;
	int		fd;

	fdf->map.point = NULL;
	fd = open(file, O_RDONLY);
	if (fd < 3)
		close_fdf(fdf, 0);
	fdf->map.size_x = 0;
	fdf->map.size_y = 0;
	line = get_next_line(fd);
	row = ft_split(line, ' ');
	while (row[fdf->map.size_x])
		free(row[fdf->map.size_x++]);
	free(row);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
		fdf->map.size_y++;
	}
	close(fd);
	fdf->map.area = fdf->map.size_x * fdf->map.size_y;
	fdf->map.point = ft_calloc(fdf->map.size_x * fdf->map.size_y, sizeof(t_point));
	get_points(&fdf->map, file, fdf);
}
