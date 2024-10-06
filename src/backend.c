#include "backend.h"
#include "raylib.h"

FILE *rom_file;
unsigned char *rom_buffer;
size_t rom_size;

int open_rom ( const char *filename )
{
	rom_file = fopen ( filename, "rb" );
	if ( rom_file == NULL )
	{
		perror ( "E1.000 File unaccessible" );
		return 1;
	}

	/* determine file size */
	fseek ( rom_file, 0, SEEK_END );
	long filesize = ftell ( rom_file );
	/* reset file pointer to the start */
	fseek ( rom_file, 0, SEEK_SET );

	/* allocate memory for rom buff */
	rom_buffer = ( unsigned char * ) malloc ( filesize );
	if ( rom_buffer == NULL )
	{
		perror ( "E1.001 Error allocating rom buffer" );
		fclose ( rom_file );
		return 1;
	}

	/* copy file to buffer */
	size_t bytes_read = fread ( rom_buffer, 1, filesize, rom_file );
	if ( bytes_read != filesize )
	{
		perror ( "E1.002 Error copying rom to buffer? Size does not match" );
		free ( rom_buffer );
		fclose ( rom_file );
		return 1;
	}
	
	rom_size = bytes_read;
	printf ( "ROM read, size: %zx\n", rom_size );

	return 0;
}

int dump_rom_header ( void )
{
	/*
	 *	ROM header info (tipically?) resides at ROM offset 0x000100-0x00018D
	 */
	if ( rom_buffer == NULL )
	{
		perror ( "E8.000 ROM buffer uninitialized. Cannot dump header" );
		return 1;
	}

	printf ( "ROM header dump:\n" );
	for ( int i = 0x000100; i < 0x00018E; i++ )
	{
		printf ( "%c", rom_buffer[i] );
	}
	printf ( "\n" );

	return 0;
}

void attach_rom ( void )
{
	set_rom ( rom_buffer, rom_size );
}

void close_rom ( void )
{
	free ( rom_buffer );
	fclose ( rom_file );
}
