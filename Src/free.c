
#include "../Inc/fdf.h"

void free_mesh(t_pixel ***mesh, t_map *map) {
  for (int i = 0; i < map->rows; i++) {
    free((*mesh)[i]);
  }
  free((*mesh));
}

void ft_free_map(t_map *map) {
  int i;

  if (!map)
    return;
  if (map->coors) {
    for (i = 0; i < map->rows; i++) {
      if (map->coors[i])
        free(map->coors[i]);
    }
    free(map->coors);
  }
  if (map->cols)
    free(map->cols);
}
