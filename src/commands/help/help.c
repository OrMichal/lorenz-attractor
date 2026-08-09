#include "./help.h"
#include <stdio.h>
#include <stdlib.h>

int execHelpCommand() {
  printf(
    "Usage: Lorenzo [OPTIONS]\n\n"
    "Interactive visualization of the Lorenz attractor rendering point-by-point.\n\n"
    "Options:\n"
    "  -r, --rho <float>     Rho parameter (Rayleigh number)      [default: 28.0]\n"
    "  -b, --beta <float>    Beta parameter                       [default: 2.6667 (8/3)]\n"
    "  -s, --sigma <float>   Sigma parameter (Prandtl number)     [default: 10.0]\n"
    "  -f, --fps <int>       Rendering speed (frames per second)  [default: 60]\n"
    "  -h, --help            Display this help message and exit\n\n"
    "Examples:\n"
    "  ./lorenzo\n"
    "  ./lorenzo --rho 28.0 --beta 2.6667 --sigma 10.0\n"
    "  ./lorenzo -r 14.0 -f 120\n"
  );

  return EXIT_SUCCESS;
}
