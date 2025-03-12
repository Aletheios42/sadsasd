#include "../Inc/fdf.h"
#include <float.h>
#include <math.h>

int interpolColor(int height, int z_range[2]) {
  int min_height = z_range[0];
  int max_height = z_range[1];

  // Handle the case where min and max are the same to avoid division by zero
  if (max_height == min_height)
    return MIN_COLOR;

  // Calculate the normalized position of height within the range [0.0, 1.0]
  double factor = ((double)height - (double)min_height) /
                  ((double)max_height - (double)min_height);

  // Ensure factor is in range [0,1]
  if (factor < 0.0)
    factor = 0.0;
  if (factor > 1.0)
    factor = 1.0;

  // Calculate the interpolated color
  int r_min = (MIN_COLOR >> 16) & 0xFF;
  int g_min = (MIN_COLOR >> 8) & 0xFF;
  int b_min = MIN_COLOR & 0xFF;

  int r_max = (MAX_COLOR >> 16) & 0xFF;
  int g_max = (MAX_COLOR >> 8) & 0xFF;
  int b_max = MAX_COLOR & 0xFF;

  int r = r_min + (int)(factor * (r_max - r_min));
  int g = g_min + (int)(factor * (g_max - g_min));
  int b = b_min + (int)(factor * (b_max - b_min));

  return (r << 16) | (g << 8) | b;
}

t_pixel proj_iso(t_point point, int scale_z, int z_range[2]) {
  t_pixel pixel_to_draw;

  // Ángulo base para la proyección isométrica (normalmente 30°)
  const float alpha = M_PI / 6; // 30 grados en radianes

  // Aplicamos escala a la altura
  float z = point.height * scale_z;

  // Calculamos los ángulos necesarios
  float cos_a = cos(alpha);
  float sin_a = sin(alpha);
  float cos_a_plus_120 = cos(alpha + 2 * M_PI / 3);  // cos(α+120°)
  float sin_a_plus_120 = sin(alpha + 2 * M_PI / 3);  // sin(α+120°)
  float cos_a_minus_120 = cos(alpha - 2 * M_PI / 3); // cos(α-120°)
  float sin_a_minus_120 = sin(alpha - 2 * M_PI / 3); // sin(α-120°)

  // Aplicamos la fórmula de proyección isométrica
  float u = point.x * cos_a + point.y * cos_a_plus_120 + z * cos_a_minus_120;
  float v = point.x * sin_a + point.y * sin_a_plus_120 + z * sin_a_minus_120;

  // Redondeamos para mantener continuidad
  pixel_to_draw.x = round(u);
  pixel_to_draw.y = round(v);

  // Asignamos color
  if (point.native) {
    pixel_to_draw.color = point.color;
  } else {
    pixel_to_draw.color = interpolColor(point.height, z_range);
  }

  return pixel_to_draw;
} // Proyección circular
//
t_pixel test_proj(t_point point, int scale_z, int z_range[2]) {
  t_pixel pixel_to_draw;
  (void)scale_z;
  (void)z_range;

  pixel_to_draw.x = point.x * 40;
  pixel_to_draw.y = point.y * 40;

  if (point.native) {
    pixel_to_draw.color = point.color;
  } else {
    pixel_to_draw.color = interpolColor(point.height, z_range);
  }
  return pixel_to_draw;
} // Proyección circular

t_pixel proj_circular(t_point point, int scale_z, int z_range[2]) {
  t_pixel pixel_to_draw;

  // Cálculo de coordenadas polares
  float r = sqrt(point.x * point.x + point.y * point.y);
  float theta = atan2(point.y, point.x) +
                (point.height * scale_z * 0.01); // Factor reducido

  // Conversión a coordenadas cartesianas
  pixel_to_draw.x = (int)(r * cos(theta));
  pixel_to_draw.y = (int)(r * sin(theta));

  // Asignar color
  if (point.native) {
    pixel_to_draw.color = point.color;
  } else {
    pixel_to_draw.color = interpolColor(point.height, z_range);
  }

  return pixel_to_draw;
}

// Proyección ortogonal
t_pixel proj_orthogonal(t_point point, int scale_z, int z_range[2]) {
  t_pixel pixel_to_draw;

  // Mantener coordenada x y ajustar y según la altura
  pixel_to_draw.x = point.x;
  pixel_to_draw.y = point.y - (point.height * 0.2 * scale_z);

  // Asignar color
  if (point.native) {
    pixel_to_draw.color = point.color;
  } else {
    pixel_to_draw.color = interpolColor(point.height, z_range);
  }

  return pixel_to_draw;
}
