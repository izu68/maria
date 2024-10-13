#ifndef H_INSPECTOR
#define H_INSPECTOR

int init_inspector_window ( void );
void update_inspector_window ( void );
void destroy_inspector_window ( void );
void inspector_transfer_eva_registers ( int r0, int r1, int r2, int r3, int flags, int pc, int addr_bank, int addr, int data );

bool inspector_sent_reset ( void );
void emu_done_reset ( void );

#endif
