
#include "../Inc/fdf.h"

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
