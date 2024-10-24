#include <stdio.h>
#include "raylib.h"
#include "backend.h"
#include "renderer.h"
#include "input.h"

void print_help ( void )
{
	printf ( "usage: maria [rom path]\n" );
}

int main ( int argc, char *argv[] ) 
{
	printf ( "maria emulator\n" );

	if ( argc < 2 )
	{
		print_help ();
		return 1;
	}

	window_scale_x = window_scale_y = 2;
	SetTraceLogLevel ( LOG_ERROR );

	InitWindow ( 320 * window_scale_x, 240 * window_scale_y, "maria" );
	SetWindowState ( FLAG_WINDOW_RESIZABLE );
	InitAudioDevice ();
	SetTargetFPS ( 60 );

	init_vdp_render_interface ();
	open_rom ( argv[1] );
	dump_rom_header ();
	attach_rom ();

	m68k_pulse_reset ();
	eva_pulse_reset ();

	while ( !WindowShouldClose () )
	{
		BeginDrawing ();
		ClearBackground ( BLACK );

		handle_input ();
		frame ();
		render_vdp_output ();

		EndDrawing ();

		if ( IsKeyDown ( KEY_LEFT_ALT ) && IsKeyPressed ( KEY_TAB ) )
		{
			close_rom ();
			printf ( "ROM reload\n" );
			open_rom ( argv[1] );
			dump_rom_header ();
			attach_rom ();
			m68k_pulse_reset ();
		}
		else if ( IsKeyPressed ( KEY_TAB ) ) 
		{
			m68k_pulse_reset ();
		}
	}	

	close_rom ();
	destroy_vdp_render_interface ();
	CloseWindow ();
	return 0;
}
