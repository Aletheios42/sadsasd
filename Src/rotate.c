#include "../Inc/fdf.h"

// Rotation functions for the FDF renderer
// Add these to your existing rendering code

#include <math.h>

void rotate_x(t_pixel *pixel, double angle) {
  int original_y = pixel->y;
  int original_z = 0; // Since we're in 2D projection, we need to derive z

  if (pixel->y != 0) {
    original_z = -pixel->y / 2; // Approximate value, may need adjustment
  }

  pixel->y = original_y * cos(angle) - original_z * sin(angle);
}

void rotate_y(t_pixel *pixel, double angle) {
  int original_x = pixel->x;
  int original_z = 0;
  if (pixel->x != 0) {
    original_z = -pixel->y / 2; // Approximate value
  }

  pixel->x = original_x * cos(angle) + original_z * sin(angle);
  // Z component updated for future rotations
}

void rotate_z(t_pixel *pixel, double angle) {
  int original_x = pixel->x;
  int original_y = pixel->y;

  pixel->x = original_x * cos(angle) - original_y * sin(angle);
  pixel->y = original_x * sin(angle) + original_y * cos(angle);
}

void rotate_mesh(t_pixel ***mesh, t_cam *camera, t_map *map) {
  int i, j;

  // Rotating around all three axes in sequence
  for (i = 0; i < map->rows; i++) {
    for (j = 0; j < map->cols[i]; j++) {
      // Apply rotations in order: x, y, then z
      rotate_x(&(*mesh)[i][j], camera->angle[0]);
      rotate_y(&(*mesh)[i][j], camera->angle[1]);
      rotate_z(&(*mesh)[i][j], camera->angle[2]);
    }
  }
}
