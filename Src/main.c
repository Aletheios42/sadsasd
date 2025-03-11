#include "../Inc/fdf.h"

int main(int ac, char **av) {
  t_fdf fdf;
  t_map map;
  t_mlx mlx;
  t_cam camera;

  if (ac != 2)
    return (ft_putstr_fd("Usage Instrctions: ./fdf <map_file.fdf>\n", 2), 1);

  ft_memset(&map, 0, sizeof(t_map));
  if (parser(&map, av[1]) == -1)
    return (1);

  /*print_original_map(map);*/

  ft_memset(&mlx, 0, sizeof(t_mlx));
  init_window(&mlx);

  init_camera(&camera);

  init_fdf(&fdf, &map, &camera, &mlx);
  render(&map, &camera, &mlx);

  // Hook para user-input
  mlx_key_hook(mlx.win, handle_key_events, &fdf);
  // Hook para cerrar la ventana
  mlx_key_hook(mlx.win, handle_keypress, &camera);
  mlx_hook(mlx.win, 17, 1, close_window, mlx.mlx);
  // Hook para manejar
  /*mlx_hook(mlx.win, 2, 1, handle_keypress, mlx.mlx);*/
  mlx_loop(mlx.mlx);
  return (0);
}
