#include "../Inc/fdf.h"

void bresenham(t_bresenham *b, t_pixel start, t_pixel end) {
  b->x0 = start.x;
  b->y0 = start.y;
  b->x1 = end.x;
  b->y1 = end.y;
  b->dx = abs(b->x1 - b->x0);
  b->dy = abs(b->y1 - b->y0);
  b->sx = (b->x0 < b->x1) ? 1 : -1;
  b->sy = (b->y0 < b->y1) ? 1 : -1;
  b->err = b->dx - b->dy;
}

void draw_segment(t_mlx *mlx, t_pixel start, t_pixel end) {
  t_bresenham b;
  int e2;

  // Inicializa el algoritmo de Bresenham
  bresenham(&b, start, end);

  while (1) {
    // Dibuja el pixel actual si está dentro de los límites de la ventana
    if (b.x0 >= 0 && b.x0 < WIN_WIDTH && b.y0 >= 0 && b.y0 < WIN_HEIGHT) {
      my_mlx_pixel_put(mlx, b.x0, b.y0, start.color);
    }

    // Condición de salida: llegamos al punto final
    if (b.x0 == b.x1 && b.y0 == b.y1)
      break;

    // Algoritmo de Bresenham
    e2 = 2 * b.err;
    if (e2 > -b.dy) {
      b.err -= b.dy;
      b.x0 += b.sx;
    }
    if (e2 < b.dx) {
      b.err += b.dx;
      b.y0 += b.sy;
    }
  }
}
