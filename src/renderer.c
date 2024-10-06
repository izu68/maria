#include "renderer.h"
#include "backend.h"

float window_scale_x, window_scale_y;

unsigned char framebuffer[320 * 224 * 4];
unsigned char framebuffer_scaled[320 * 224 * 4 * SCALE_X * SCALE_Y];
const void* p_framebuffer = &framebuffer;

RenderTexture2D render_texture;

void init_vdp_render_interface ( void )
{
	vdp_set_buffer ( framebuffer );
	render_texture = LoadRenderTexture ( 320, 224 );	
}

void set_alpha ( unsigned char *fb, int index, int size )
{
	/* if index exceeds the size of the frame buffer, we're done */
	if ( index >= size )
	{
		return;
	}
	
	/* set alpha channel (every 4th byte) */
	fb[index + 3] = 255;
	/* recur for next pixel */
	set_alpha ( fb, index + 4, size ); /* +4 cause A channel is in every 4th byte */
}

void correct_color ( unsigned char *fb, int index, int size )
{
	/* if index exceeds the size of the frame buffer, we're done */
	if ( index >= size )
	{
		return;
	}

	/* write index longword of colors in RGBA32 */
	unsigned char old_color_longword[4];
	for ( int i = 0; i < 4; i++ )
	{
		old_color_longword[i] = fb[index + i];
	}
	
	/* convert to BGRA32 */
	fb[index] = old_color_longword[2]; /* B */
	fb[index + 1] = old_color_longword[1]; /* G */
	fb[index + 2] = old_color_longword[0]; /* R */
	fb[index + 3] = 255; /* alpha */ 

	/* recur for next pixel */
	correct_color ( fb, index + 4, size ); /* +4 cause A channel is in every 4th byte */	
}

void display_render_texture ( void )
{
	/* for ( int i = 0; i < ( 320 * 224 * 4 ); i++ )
	{
		framebuffer[i] = 0xFF;
	} */
	correct_color ( framebuffer, 0, 320 * 224 * 4 );

	UpdateTextureRec (
			render_texture.texture, 
			( Rectangle ) { 0, 0, 320, 224 },
			p_framebuffer
	);
	DrawTexturePro (
			render_texture.texture, 
			( Rectangle ) { 0, 0, 320, 224 }, 
			( Rectangle ) { 0, 0, 320 * window_scale_x, 224 * window_scale_y }, 
			( Vector2 ) { 0, 0 }, 
			0, 
			WHITE
	); 
}
