#include "../Inc/fdf.h"

void init_fdf(t_fdf *fdf, t_map *map, t_cam *cam, t_mlx *mlx) {
  (*fdf).mlx = mlx;
  (*fdf).cam = cam;
  (*fdf).map = map;
}

void init_camera(t_cam *camera) {
  ft_memset(camera, 0, sizeof(t_cam));
  camera->scale_factor = 1.0;
  camera->scale_z = 1.0;
  camera->projection = 0;
}

int malloc_mesh(t_pixel ***mesh, int rows, int *cols) {
  int i;

  *mesh = (t_pixel **)malloc(sizeof(t_point *) * rows);
  if (!*mesh)
    return 0;

  i = -1;
  while (++i < rows) {
    (*mesh)[i] = (t_pixel *)malloc(sizeof(t_pixel) * cols[i]);
    if (!(*mesh)[i])
      return 0;
  }
  return 1;
}

int realloc_cols(char *line, int **col, int rows) {
  int i;
  int *col_helper;
  int row;
  i = count_columns(line);

  col_helper = *col;
  *col = (int *)malloc(sizeof(int) * (1 + rows));

  row = -1;
  while (++row < rows)
    (*col)[row] = col_helper[row];
  (*col)[rows] = i;
  free(col_helper);
  return 1;
}

int realloc_t_point(t_point ***coors, int rows, int ncols) {
  t_point **new_coors;
  int i;

  new_coors = malloc(sizeof(t_point *) * (rows + 1));
  if (!new_coors)
    return 0;

  i = 0;
  while (i < rows) {
    new_coors[i] = (*coors)[i];
    i++;
  }

  new_coors[rows] = calloc(ncols, sizeof(t_point));
  if (!new_coors[rows]) {
    free(new_coors);
    return 0;
  }

  free(*coors);
  *coors = new_coors;
  return 1;
}
