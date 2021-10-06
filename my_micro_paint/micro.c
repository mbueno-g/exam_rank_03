#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_zone
{
	int width;
	int height;
	char background;
}								t_zone;

typedef struct s_shape
{
	char		r;
	float		x;
	float		y;
	float		width;
	float		height;
	char		charac;
}							t_shape;

char *draw_zone(FILE *file, t_zone *z)
{
	char *str;
	int		i;
	int		input;

	z->width = 0;
	z->height = 0;
	z->background = 0;
	if ((input = fscanf(file, "%d %d %c\n", &z->width, &z->height, &z->background)) != 3)
		return (NULL);
	else if (z->width <= 0 || z->width > 300)
		return (NULL);
	else if (z->height <= 0 || z->height > 300)
		return (NULL);
	else if (input == -1)
		return (NULL);
	str = (char *)malloc(sizeof(char *)*(z->width * z->height));
	if (!str)
		return (NULL);
	i = 0;
	while (i < z->width * z->height)
		str[i++] = z->background;
	return (str);
}

int	in_rectangle(float x, float y, t_shape *s)
{
	if ((x < s->x) || (s->x + s->width < x) || (y < s->y) || (s->y + s->height < y))
		return (0);
	if ((x - s->x < 1) || (s->x + s->width - x < 1) || (y - s->y < 1) || (s->y + s->height - y < 1)) //borde
		return (2);
	return (1);
}

int draw_rectangle(FILE *file, t_zone *z, char *str)
{
	int			input;
	t_shape s;
	int			x;
	int			y;
	int			in;

	while ((input = fscanf(file, "%c %f %f %f %f %c\n", &s.r, &s.x, &s.y, &s.width, &s.height, &s.charac)) == 6)
	{
		if (s.width <= 0 || s.height <= 0 || (s.r != 'r' && s.r != 'R'))
			return (1);
		y = 0;
		while (y < z->height)
		{
			x = 0;
			while (x < z->width)
			{
				in = in_rectangle((float)x, (float)y, &s);
				if ((s.r == 'r' && in == 2) || (s.r == 'R' && in))
					str[(y*z->width + x)] = s.charac;
				x++;
			}
			y++;
		}
	}
	if (input != -1)
		return (1);
	return (0);
}

void	draw_output(t_zone *z, char *str)
{
	int i;

	i = 0;
	while (i < z->height)
	{
		write(1, str + (i*z->width), z->width);
		write(1, "\n", 1);
		i++;
	}
}

int main(int argc, char **argv)
{
	FILE *file;
	t_zone z;
	char *draw_z;

	draw_z = NULL;
	if (argc != 2)
	{
		write(1, "Error: argument\n", 16);
		return (1);
	}
	if (!(file = fopen(argv[1], "r")))
	{
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	if (!(draw_z = draw_zone(file, &z)))
	{
		fclose(file);
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	if (draw_rectangle(file, &z, draw_z))
	{
		fclose(file);
		write(1, "Error: Operation file corrupted\n", 32);
		return (1);
	}
	draw_output(&z, draw_z);
	fclose(file);
	free(draw_z);
	return(0);
}

