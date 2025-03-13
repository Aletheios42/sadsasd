#ifndef FDF_H
#define FDF_H

#include "../Libft/libft.h"
#include "../minilibx_linux/mlx.h"

#include <stdbool.h>

#include "colors.h"
#include "keys.h"
#include "structs.h"

// DEFINES
#define WIN_HEIGHT 600
#define WIN_WIDTH 900

// parser.c
int parser(t_map *map, char *map_file);

// camera.c
void init_camera(t_cam *camera);

// mlx.c
void init_window(t_mlx *mlx);
void refresh_frame(t_mlx *mlx);
void my_mlx_pixel_put(t_mlx *mlx, int x, int y, int color);
int handle_keypress(int keycode, t_mlx *mlx);
int close_window(t_mlx *mlx);

// render.c
int render(t_map *map, t_cam *camera, t_mlx *mlx);
// draw.c
void draw_segment(t_mlx *mlx, t_pixel start, t_pixel end);
void test_line_drawing(t_mlx *mlx);

// proyec.c
int interpolColor(int height, int z_range[2]);
t_pixel proj_iso(t_point point, int z_scale, int z_range[2]);
t_pixel test_proj(t_point point, int z_scale, int z_range[2]);
t_pixel proj_circular(t_point point, int z_scale, int z_range[2]);
t_pixel proj_orthogonal(t_point point, int z_scale, int z_range[2]);

// camera.c
int camera_key_hook(int keycode, t_cam *cam);
int handle_key_events(int keycode, t_fdf *fdf);

// print.c
void print_int_array(int *arr, int size);
void print_projected_map(t_pixel **mesh, t_map map);
void print_original_map(t_map map);

// malloc.c
void init_fdf(t_fdf *fdf, t_map *map, t_cam *cam, t_mlx *mlx);
int realloc_cols(char *line, int **col, int rows);
int realloc_t_point(t_point ***coors, int rows, int ncols);
int malloc_mesh(t_pixel ***mesh, int rows, int *cols);

// rotate.c
// Add to fdf.h
void rotate_mesh(t_pixel ***mesh, t_cam *camera, t_map *map);
void rotate_x(t_pixel *pixel, double angle);
void rotate_y(t_pixel *pixel, double angle);
void rotate_z(t_pixel *pixel, double angle);

// free.c
void ft_free_map(t_map *map);
void free_mesh(t_pixel ***mesh, t_map *map);

// utils.c
int ft_atoi_base(char *str, char *base);
bool ft_ishex(char c);
int count_columns(char *line);
int ft_open(int *fd, char *map_file);

#endif
