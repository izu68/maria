#include "eva.h"

e_byte EVA_RAM[ADDRSPACE 0xFF][ADDRSPACE 0xFFFF];
eva_t eva;

void eva_update_address_bank ( void )
{
	eva.addr_bank = EVA_RAM[0x00][0x0001];
	//printf ( "(EVA) ADDR BANK SET: %x\n", eva.addr_bank );
}

void eva_update_address_port ( void )
{
	eva_update_address_bank ();
	eva.addr = EVA_RAM[0x00][0x0002] << 8 | EVA_RAM[0x00][0x0003];
	//printf ( "(EVA) ADDR PORT SET: %x\n", eva.addr );
}

void eva_update_data_port ( void )
{
	if ( eva.addr_bank < 2 )
	{
		printf ( "(EVA ERROR) UNAUTHORIZED MEM WRITE: BANK %02x ADDR %04x\n", eva.addr_bank, eva.addr );
		return;
	}
	eva.data = EVA_RAM[0x00][0x0004] << 24 | EVA_RAM[0x00][0x0005] << 16 |
			 EVA_RAM[0x00][0x0006] << 8 | EVA_RAM[0x00][0x0007];
	/* transfer from port to actual memory location */
	EVA_RAM[eva.addr_bank][eva.addr] = EVA_RAM[0x00][0x0004];
	EVA_RAM[eva.addr_bank][eva.addr+1] = EVA_RAM[0x00][0x0005];
	EVA_RAM[eva.addr_bank][eva.addr+2] = EVA_RAM[0x00][0x0006];
	EVA_RAM[eva.addr_bank][eva.addr+3] = EVA_RAM[0x00][0x0007];
	printf ( "(EVA) MEM WRITE: BANK %02x ADDR %04x: %08x\n", eva.addr_bank, eva.addr, eva.data );
}
