#include "../Inc/fdf.h"

int handle_key_events(int keycode, t_fdf *fdf) {
  if (keycode == KEY_ESC)
    close_window(fdf->mlx);

  // Camera rotation
  else if (keycode == KEY_UP)
    fdf->cam->angle[0] += 0.1;
  else if (keycode == KEY_DOWN)
    fdf->cam->angle[0] -= 0.1;
  else if (keycode == KEY_LEFT)
    fdf->cam->angle[1] -= 0.1;
  else if (keycode == KEY_RIGHT)
    fdf->cam->angle[1] += 0.1;
  else if (keycode == KEY_Z)
    fdf->cam->angle[2] += 0.1;
  else if (keycode == KEY_X)
    fdf->cam->angle[2] -= 0.1;

  // Movement
  else if (keycode == KEY_W)
    fdf->cam->offset[1] -= 10;
  else if (keycode == KEY_S)
    fdf->cam->offset[1] += 10;
  else if (keycode == KEY_A)
    fdf->cam->offset[0] -= 10;
  else if (keycode == KEY_D)
    fdf->cam->offset[0] += 10;

  // Zoom
  else if (keycode == KEY_SPACE)
    fdf->cam->scale_factor *= 1.1;
  else if (keycode == KEY_MINUS)
    fdf->cam->scale_factor *= 0.9;

  // Z-scale
  else if (keycode == KEY_Q)
    fdf->cam->scale_z += 0.3;
  else if (keycode == KEY_E)
    fdf->cam->scale_z -= 0.3;

  // Projection change
  else if (keycode == NUMKEY_1)
    fdf->cam->projection = 0;
  else if (keycode == NUMKEY_2)
    fdf->cam->projection = 1;
  else if (keycode == NUMKEY_3)
    fdf->cam->projection = 2;
  else if (keycode == NUMKEY_4)
    fdf->cam->projection = 3;

  render(fdf->map, fdf->cam, fdf->mlx);
  return (0);
}

int camera_key_hook(int keycode, t_cam *cam) {
  // Si presionas flecha izquierda (código 123), mueve la cámara a la izquierda
  if (keycode == 123)
    cam->offset[0] -= 10.0f;

  // Si presionas flecha derecha (código 124), mueve la cámara a la derecha
  else if (keycode == 124)
    cam->offset[0] += 10.0f;

  return (0);
}
