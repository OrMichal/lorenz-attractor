#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "./commands/help/help.h"

#define WIDTH 800
#define HEIGHT 600
#define SCALE 8.0f

uint32_t framebuffer[WIDTH * HEIGHT];

void setPixel(uint32_t x, uint32_t y, uint32_t color) {
  if((x >= WIDTH) || (y >= HEIGHT)) {
    return;
  }

  framebuffer[x + y * WIDTH] = color;
}

void checkArg(int argIndex, int argc, char** argv) {
  if(argIndex > argc) {
    fprintf(stderr, "Invalid use of argument: '%s'!", argv[argIndex]);
  }
}

int main(int argc, char** argv) {
  uint8_t isRunning = 1;
  SDL_Event event;

  int fps = 60;

  float x = 0.01f;
  float y = 0.01f;
  float z = 0.01f;

  float sigma = 10;
  float ro = 28;
  float beta = 8.0/3.0;

  for(int i = 0; i < argc; i++) {
    if(strcmp(argv[i], "--rho") == 0 || strcmp(argv[i], "-r") == 0) {
      ro = atof(argv[i + 1]);
      checkArg(i, argc, argv);
    } else if(strcmp(argv[i], "--beta") == 0 || strcmp(argv[i], "-b") == 0) {
      beta = atof(argv[i + 1]);
      checkArg(i, argc, argv);
    } else if(strcmp(argv[i], "--sigma") == 0 || strcmp(argv[i], "-s") == 0) {
      sigma = atof(argv[i + 1]);
      checkArg(i, argc, argv);
    } else if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
      return execHelpCommand();
    } else if(strcmp(argv[i], "--fps") == 0 || strcmp(argv[i], "-f") == 0) {
      fps = atoi(argv[i + 1]);
      checkArg(i, argc, argv);
    }
  }

  printf("argc: %d\n", argc);
  printf("beta: %f, sigma: %f, rho: %f\n", beta, sigma, ro);

  float target = 1.0f / fps;

  if(!SDL_Init(SDL_INIT_VIDEO)) {
    fprintf(stderr, "Failed to initialize application: %s \n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Window *window = SDL_CreateWindow(
    "Lorenzo attractors", 
    WIDTH, 
    HEIGHT, 
    0
  );

  if(window == NULL) {
    fprintf(stderr, "Failed to initialize application windowf: %s \n", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

  if(renderer == NULL) {
    fprintf(stderr, "Failed to initialize application renderer: %s \n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Texture *texture = SDL_CreateTexture(
    renderer, 
    SDL_PIXELFORMAT_XRGB8888, 
    SDL_TEXTUREACCESS_STREAMING, 
    WIDTH, 
    HEIGHT
  );

  if(texture == NULL) {
    fprintf(stderr, "Failed to initialize application texture: %s \n", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  while(isRunning) {
    uint64_t start = SDL_GetPerformanceCounter();
    
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_EVENT_QUIT) {
        isRunning = 0;
      }
    }

    float dt = 0.01;

    float dx = sigma * (y - x) * dt;
    float dy = (x * (ro - z) - y) * dt;
    float dz = (x * y - beta * z) * dt;

    z += dz;
    y += dy;
    x += dx;

    setPixel(x * SCALE + WIDTH/2.0, y * SCALE + HEIGHT/2.0, 0x00ff00);

    SDL_UpdateTexture(
      texture, 
      NULL, 
      framebuffer, 
      WIDTH * sizeof(uint32_t)
    );

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    uint64_t end = SDL_GetPerformanceCounter();
    double elapsed = (double)(end - start) / (double)SDL_GetPerformanceFrequency();

    if(elapsed < target) {
      SDL_Delay((target - elapsed) * 1000.0);
    }
  }

  return EXIT_SUCCESS;
}
