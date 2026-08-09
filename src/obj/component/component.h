#include <cstdint>
#ifndef COMPONENT_H

typedef struct {
  uint32_t X;
  uint32_t Y;
  uint32_t Width;
  uint32_t Height;
  uint32_t Buffer;
} Component;

Component* newComponent(uint32_t width, uint32_t height);

#endif
