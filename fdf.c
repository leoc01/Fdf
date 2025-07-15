#include "fdf.h"	

int	main(int argc, char **argv)
{
	t_fdf	fdf;

	if (argc != 2)
		return 1;
	start(&fdf, argv[1]);
	mlx_loop(fdf.mlx);
	return (0);
}
