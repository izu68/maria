#ifndef H_RENDERER
#define H_RENDERER
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define SCALE_X 2
#define SCALE_Y 2

extern float window_scale_x, window_scale_y;

extern unsigned char framebuffer[320 * 224 * 4];

void init_vdp_render_interface ( void );
void display_render_texture ( void );

#endif
