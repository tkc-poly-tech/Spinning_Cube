#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float A, B, C;

float cube_Width = 50;
int width = 160, height = 44;
float z_buffer[160 * 44];
char buffer[160 * 44];
int background_ASCII_code = ' ';
int distance_from_cam = 500;
float K1 = 40;

float increment_speed = 0.6;

float x, y, z;
float ooz;
int xp, yp;
int index_n;

float	calculate_X(int i, int j, int k)
{
	return (j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + 
			j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C));
}

float	calculate_Y(int i, int j, int k)
{
	return (j * cos(A) * cos(C) + k * sin(A) * cos(C) - 
			j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
			i * cos(B) * sin(C));
}

float	calculate_Z(int i, int j, int k)
{
	return (k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B));
}

void	calculate_for_surface(float cube_X, float cube_Y, float cube_Z, int ch)
{
	x = calculate_X(cube_X, cube_Y, cube_Z);
	y = calculate_Y(cube_X, cube_Y, cube_Z);
	z = calculate_Z(cube_X, cube_Y, cube_Z) + distance_from_cam;

	ooz = 1 / z;

	xp = (int)(width / 2 + K1 * ooz * x * 2);
	yp = (int)(height / 2 * K1 * ooz * y);

	index_n = xp + yp * width;
	if (index_n >= 0 && index_n < width * height)
	{
		if (ooz > z_buffer[index_n])
		{
			z_buffer[index_n] = ooz;
			buffer[index_n] = ch;
		}
	}
}

int main()
{
	printf("\x1b[2J");
	while (1)
	{
		memset(buffer, background_ASCII_code, width * height);
		memset(z_buffer, 0, width * height * 4);
		for (float cube_X = - cube_Width; cube_X < cube_Width; cube_X += increment_speed)
		{
			for (float cube_Y = - cube_Width; cube_Y < cube_Width; cube_Y += increment_speed)
			{
				calculate_for_surface(cube_X, cube_Y, -cube_Width, '.');
				calculate_for_surface(cube_Width, cube_Y, cube_X, '$');
				calculate_for_surface(-cube_Width, cube_Y, -cube_X, '~');
				calculate_for_surface(-cube_X, cube_Y, cube_Width, '#');
				calculate_for_surface(cube_X, -cube_Width, -cube_Y, ';');
				calculate_for_surface(cube_X, cube_Width, cube_Y, '+');
			}
		}
		printf("\x1b[H");
		for (int k = 0; k < width * height; k++)
		{
			putchar(k % width ? buffer[k] : 10);
		}
		
		A += 0.005;
		B += 0.005;
		usleep(1000);
	}
	
	return (0);
}