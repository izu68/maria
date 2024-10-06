#include "input.h"
#include "raylib.h"

void handle_input ( void )
{
	if ( IsKeyDown ( KEY_W ) ) { pad_press_button ( 0, PAD_UP ); }
	else if ( !IsKeyDown ( KEY_W ) ) { pad_release_button ( 0, PAD_UP ); }	
	if ( IsKeyDown ( KEY_A ) ) { pad_press_button ( 0, PAD_LEFT ); }
	else if ( !IsKeyDown ( KEY_A ) ) { pad_release_button ( 0, PAD_LEFT ); }	
	if ( IsKeyDown ( KEY_S ) ) { pad_press_button ( 0, PAD_DOWN ); }
	else if ( !IsKeyDown ( KEY_S ) ) { pad_release_button ( 0, PAD_DOWN ); }	
	if ( IsKeyDown ( KEY_D ) ) { pad_press_button ( 0, PAD_RIGHT ); }
	else if ( !IsKeyDown ( KEY_W ) ) { pad_release_button ( 0, PAD_RIGHT ); }
	
	if ( IsKeyDown ( KEY_J ) ) { pad_press_button ( 0, PAD_A ); }
	else if ( !IsKeyDown ( KEY_J ) ) { pad_release_button ( 0, PAD_A ); }
	if ( IsKeyDown ( KEY_K ) ) { pad_press_button ( 0, PAD_B ); }
	else if ( !IsKeyDown ( KEY_K ) ) { pad_release_button ( 0, PAD_B ); }
	if ( IsKeyDown ( KEY_L ) ) { pad_press_button ( 0, PAD_C ); }
	else if ( !IsKeyDown ( KEY_L ) ) { pad_release_button ( 0, PAD_C ); }

	if ( IsKeyDown ( KEY_ENTER ) ) { pad_press_button ( 0, PAD_S ); }
	else if ( !IsKeyDown ( KEY_ENTER ) ) { pad_release_button ( 0, PAD_S ); }
}
