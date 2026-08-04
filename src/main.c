#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600
#define SCALE 8.0f
#define FPS 60

uint32_t framebuffer[WIDTH * HEIGHT];

void setPixel(uint32_t x, uint32_t y, uint32_t color) {
  if((x >= WIDTH) || (y >= HEIGHT)) {
    return;
  }

  framebuffer[x + y * WIDTH] = color;
}

int main(int argc, char** argv) {
  uint8_t isRunning = 1;
  SDL_Event event;

  float x = 0.01f;
  float y = 0.01f;
  float z = 0.01f;

  float sigma = 10;
  float ro = 28;
  float beta = 8.0/3.0;

  float target = 1.0f / FPS;

  printf("(x, y, z): (%f, %f, %f)\n", x, y,z);

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
