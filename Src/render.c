#include "../Inc/fdf.h"
#include <float.h>

// Aplicar la perspectiva a la malla
void apply_perspective(t_pixel ***mesh, t_cam *camera, t_map *map) {
  int i, j;

  t_pixel (*perspectives_array[])(t_point point, int z_scale,
                                  int z_range[2]) = {proj_iso, proj_orthogonal,
                                                     test_proj, proj_circular};

  for (i = 0; i < map->rows; i++) {
    for (j = 0; j < map->cols[i]; j++) {
      (*mesh)[i][j] = perspectives_array[camera->projection](
          map->coors[i][j], camera->scale_z, map->z_range);
    }
  }
}

// Obtener el rango del mapa
void get_map_range(t_pixel ***mesh, t_map *map, float min[2], float max[2]) {
  int i, j;
  min[0] = FLT_MAX;
  min[1] = FLT_MAX;
  max[0] = -FLT_MAX;
  max[1] = -FLT_MAX;

  for (i = 0; i < map->rows; i++) {
    for (j = 0; j < map->cols[i]; j++) {
      if ((*mesh)[i][j].x < min[0])
        min[0] = (*mesh)[i][j].x;
      if ((*mesh)[i][j].y < min[1])
        min[1] = (*mesh)[i][j].y;
      if ((*mesh)[i][j].x > max[0])
        max[0] = (*mesh)[i][j].x;
      if ((*mesh)[i][j].y > max[1])
        max[1] = (*mesh)[i][j].y;
    }
  }
}

// Obtener factores de escala
void get_scale(float min[2], float max[2], float *scale_factor,
               float offset[2]) {
  float map_width = max[0] - min[0];
  float map_height = max[1] - min[1];

  if (map_width <= 0)
    map_width = 1;
  if (map_height <= 0)
    map_height = 1;

  float margin_x = WIN_WIDTH * 0.2;
  float margin_y = WIN_HEIGHT * 0.2;

  float available_width = WIN_WIDTH - 2 * margin_x;
  float available_height = WIN_HEIGHT - 2 * margin_y;

  float scale_x = available_width / map_width;
  float scale_y = available_height / map_height;

  *scale_factor = (scale_x < scale_y) ? scale_x : scale_y;

  if (*scale_factor > 1000)
    *scale_factor = 1000;
  if (*scale_factor < 0.001)
    *scale_factor = 0.001;

  offset[0] = margin_x + (available_width - map_width * (*scale_factor)) / 2;
  offset[1] = margin_y + (available_height - map_height * (*scale_factor)) / 2;
}

// Función principal que escala y centra la malla
void place_objects_in_window(t_pixel ***mesh, t_map *map) {
  int i, j;
  float min[2], max[2], offset[2];
  float scale_factor;

  get_map_range(mesh, map, min, max);
  get_scale(min, max, &scale_factor, offset);

  for (i = 0; i < map->rows; i++) {
    for (j = 0; j < map->cols[i]; j++) {
      (*mesh)[i][j].x = (((*mesh)[i][j].x - min[0]) * scale_factor + offset[0]);
      (*mesh)[i][j].y = (((*mesh)[i][j].y - min[1]) * scale_factor + offset[1]);
    }
  }
}

// Aplicar transformaciones de cámara a la malla
void apply_cam_to_mesh(t_pixel ***mesh, t_cam *camera, t_map *map) {
  int i, j;

  // Solo aplicar si los valores de la cámara no son predeterminados
  if (camera->scale_factor == 1.0 && camera->offset[0] == 0 &&
      camera->offset[1] == 0) {
    return;
  }

  // Aplicar transformaciones a cada punto
  for (i = 0; i < map->rows; i++) {
    for (j = 0; j < map->cols[i]; j++) {
      // Aplicar escalado
      (*mesh)[i][j].x = (*mesh)[i][j].x * camera->scale_factor;
      (*mesh)[i][j].y = (*mesh)[i][j].y * camera->scale_factor;

      // Aplicar traslación
      (*mesh)[i][j].x += camera->offset[0];
      (*mesh)[i][j].y += camera->offset[1];
    }
  }
}

int render(t_map *map, t_cam *camera, t_mlx *mlx) {
  int x, y;
  t_pixel **mesh;

  refresh_frame(mlx);
  print_original_map(*map);
  malloc_mesh(&mesh, map->rows, map->cols);
  apply_perspective(&mesh, camera, map);
  printf("apply_perspective Result:\n");
  print_projected_map(mesh, *map);
  place_objects_in_window(&mesh, map);
  printf("scale_and_center_mesh Result:\n");
  print_projected_map(mesh, *map);
  apply_cam_to_mesh(&mesh, camera, map);

  for (y = 0; y < map->rows; y++) {
    for (x = 0; x < map->cols[y]; x++) {
      if (y < map->rows - 1)
        draw_segment(mlx, mesh[y][x], mesh[y + 1][x]);
      if (x < map->cols[y] - 1)
        draw_segment(mlx, mesh[y][x], mesh[y][x + 1]);
    }
  }
  mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);
  free_mesh(&mesh, map);
  return 0;
}
