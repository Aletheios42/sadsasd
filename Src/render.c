#include "../Inc/fdf.h"
#include <float.h>

// Aplicar la perspectiva a la malla
void apply_perspective(t_pixel ***mesh, t_cam *camera, t_map *map) {
  int i, j;

  t_pixel (*perspectives_array[])(t_point point, int z_scale,
                                  int z_range[2]) = {
      test_proj, proj_iso, proj_circular, proj_orthogonal};

  for (i = 0; i < map->rows; i++) {
    for (j = 0; j < map->cols[i]; j++) {
      (*mesh)[i][j] = perspectives_array[camera->projection](
          map->coors[i][j], camera->scale_z, map->z_range);
    }
  }
}

// Escalar y centrar la malla en la ventana
void scale_and_center_mesh(t_pixel ***mesh, t_map *map) {
  int i, j;
  float x_min = FLT_MAX, y_min = FLT_MAX;
  float x_max = -FLT_MAX, y_max = -FLT_MAX;

  // Encontrar los límites de la malla
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

  // Calcular dimensiones del mapa
  float map_width = x_max - x_min;
  float map_height = y_max - y_min;

  // Evitar división por cero
  if (map_width <= 0)
    map_width = 1;
  if (map_height <= 0)
    map_height = 1;

  // Calcular márgenes (20% de las dimensiones de la pantalla)
  float margin_x = WIN_WIDTH * 0.2;
  float margin_y = WIN_HEIGHT * 0.2;

  // Calcular espacio disponible
  float available_width = WIN_WIDTH - 2 * margin_x;
  float available_height = WIN_HEIGHT - 2 * margin_y;

  // Calcular factores de escala
  float scale_x = available_width / map_width;
  float scale_y = available_height / map_height;

  // Usar el factor de escala menor para mantener proporciones
  float scale_factor = (scale_x < scale_y) ? scale_x : scale_y;

  // Prevenir valores extremadamente grandes o pequeños
  if (scale_factor > 1000)
    scale_factor = 1000;
  if (scale_factor < 0.001)
    scale_factor = 0.001;

  // Calcular posición centrada
  float center_offset_x =
      margin_x + (available_width - map_width * scale_factor) / 2;
  float center_offset_y =
      margin_y + (available_height - map_height * scale_factor) / 2;

  // Aplicar transformación a cada punto
  for (i = 0; i < map->rows; i++) {
    for (j = 0; j < map->cols[i]; j++) {
      // Escalar y trasladar
      (*mesh)[i][j].x =
          (((*mesh)[i][j].x - x_min) * scale_factor + center_offset_x);
      (*mesh)[i][j].y =
          ((*mesh)[i][j].y - y_min) * scale_factor + center_offset_y;
    }
  }
}

// Aplicar transformaciones de cámara a la malla
void apply_cam_to_mesh(t_pixel ***mesh, t_cam *camera, t_map *map) {
  int i, j;

  // Solo aplicar si los valores de la cámara no son predeterminados
  if (camera->scale_factor == 1.0 && camera->offset[0] == 0 &&
      camera->offset[1] == 0) {
    return; // Evitar transformaciones innecesarias
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
  scale_and_center_mesh(&mesh, map);
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
