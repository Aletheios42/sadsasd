#include "../Inc/fdf.h"
#include <math.h>

// Interpola un color entre start y end basado en la posición actual
int interpolate_color(t_pixel start, t_pixel end, int current_x,
                      int current_y) {
  // Extraer componentes RGB de los colores
  int r1 = (start.color >> 16) & 0xFF;
  int g1 = (start.color >> 8) & 0xFF;
  int b1 = start.color & 0xFF;

  int r2 = (end.color >> 16) & 0xFF;
  int g2 = (end.color >> 8) & 0xFF;
  int b2 = end.color & 0xFF;

  // Calcular la distancia total
  double dx = end.x - start.x;
  double dy = end.y - start.y;
  double distance = sqrt(dx * dx + dy * dy);

  // Calcular la distancia actual
  double cx = current_x - start.x;
  double cy = current_y - start.y;
  double current_distance = sqrt(cx * cx + cy * cy);

  // Calcular ratio de interpolación (0.0 a 1.0)
  double ratio = (distance == 0) ? 0 : current_distance / distance;

  // Interpolar componentes RGB
  int r = r1 + ratio * (r2 - r1);
  int g = g1 + ratio * (g2 - g1);
  int b = b1 + ratio * (b2 - b1);

  // Devolver el color interpolado
  return (r << 16) | (g << 8) | b;
}

void test_line_drawing(t_mlx *mlx) {
  t_pixel start = {0, 0, 0xFF0000};   // Rojo
  t_pixel end = {500, 400, 0x00FF00}; // Verde

  draw_segment(mlx, start, end);
  mlx_put_image_to_window(mlx->mlx, mlx->win, mlx->img, 0, 0);
}

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
  int color;
  // Inicializa el algoritmo de Bresenham
  bresenham(&b, start, end);

  while (1) {
    // Calcular color interpolado para la posición actual
    color = interpolate_color(start, end, b.x0, b.y0);
    // Dibuja el pixel actual si está dentro de los límites de la ventana
    if (b.x0 >= 0 && b.x0 < WIN_WIDTH && b.y0 >= 0 && b.y0 < WIN_HEIGHT) {
      my_mlx_pixel_put(mlx, b.x0, b.y0, color);
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
