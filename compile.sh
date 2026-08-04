gcc src/main.c `pkg-config --libs sdl3` `pkg-config --cflags sdl3` -o lorenzo
./lorenzo
