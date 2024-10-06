#ifndef H_BACKEND
#define H_BACKEND
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"

int open_rom ( const char *filename );
int dump_rom_header ( void );
void attach_rom ( void );
void close_rom ( void );

/* forwarded from libmegadrive.a */
void vdp_set_buffer ( unsigned char *screen_buffer);
void set_rom ( unsigned char *buffer, size_t size );
void m68k_pulse_reset ( void );
void m68k_pulse_halt ( void );
void m68k_execute ( int num_cycles );
void frame ( void );

#endif
