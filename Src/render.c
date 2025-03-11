#include "../Inc/fdf.h"
#include <float.h>

void scale_and_center_mesh(t_pixel ***mesh, t_map *map) {
  int i, j;
  float x_min = FLT_MAX, y_min = FLT_MAX;
  float x_max = FLT_MIN, y_max = FLT_MIN;

  // Find the boundaries of the mesh
  for (i = 0; i < map->rows; i++) {
    for (j = 0; j < map->cols[i]; j++) {
      if ((*mesh)[i][j].x < x_min)
        x_min = (*mesh)[i][j].x;
      if ((*mesh)[i][j].y < y_min)
        y_min = (*mesh)[i][j].y;
      if ((*mesh)[i][j].x > x_max)
        x_max = (*mesh)[i][j].x;
      if ((*mesh)[i][j].y > y_max)
        y_max = (*mesh)[i][j].y;
    }
  }

  // Calculate map dimensions
  float map_width = x_max - x_min;
  float map_height = y_max - y_min;

  // Calculate margins (20% of screen dimensions)
  float margin_x = WIN_WIDTH * 0.2;
  float margin_y = WIN_HEIGHT * 0.2;

  // Calculate available space
  float available_width = WIN_WIDTH - 2 * margin_x;
  float available_height = WIN_HEIGHT - 2 * margin_y;

  // Calculate scale factors
  float scale_x = available_width / map_width;
  float scale_y = available_height / map_height;

  // Use the smaller scale factor to maintain proportions
  float scale_factor = (scale_x < scale_y) ? scale_x : scale_y;

  // Calculate centered position
  float center_offset_x =
      margin_x + (available_width - map_width * scale_factor) / 2;
  float center_offset_y =
      margin_y + (available_height - map_height * scale_factor) / 2;

  // Apply transformation to each point
  for (i = 0; i < map->rows; i++) {
    for (j = 0; j < map->cols[i]; j++) {
      // Scale and translate
      (*mesh)[i][j].x =
          ((*mesh)[i][j].x - x_min) * scale_factor + center_offset_x;
      (*mesh)[i][j].y =
          ((*mesh)[i][j].y - y_min) * scale_factor + center_offset_y;
    }
  }
}

void apply_perspective(t_pixel ***mesh, t_cam *camera, t_map *map) {
  int i;
  int j;

  t_pixel (*pespectives_array[])(t_point point, int z_scale, int z_range[2]) = {
      proj_iso, proj_circular, proj_orthogonal};

  i = -1;
  while (++i < map->rows) {
    j = -1;
    while (++j < map->cols[i]) {
      (*mesh)[i][j] = pespectives_array[camera->projection](
          map->coors[i][j], camera->scale_z, map->z_range);
    }
  }
}

int render(t_map *map, t_cam *camera, t_mlx *mlx) {
  int i, j;
  t_pixel **mesh;

  refresh_frame(mlx);

  print_original_map(*map);

  malloc_mesh(&mesh, map->rows, map->cols);
  apply_perspective(&mesh, camera, map);
  scale_and_center_mesh(&mesh, map);

  print_projected_map(mesh, *map);

  // Dibujar líneas horizontales
  for (i = 0; i < map->rows; i++) {
    for (j = 0; j < map->cols[i] - 1; j++) {
      draw_segment(mlx, mesh[i][j], mesh[i][j + 1]);
    }
  }

  // Dibujar líneas verticales
  for (j = 0; j < map->cols[0]; j++) {
    for (i = 0; i < map->rows - 1; i++) {
      if (j < map->cols[i] && j < map->cols[i + 1]) {
        draw_segment(mlx, mesh[i][j], mesh[i + 1][j]);
      }
    }
  }
  mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);
  return 0;
}
