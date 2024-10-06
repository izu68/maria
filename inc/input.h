#ifndef H_INPUT
#define H_INPUT
#include <stdio.h>

enum pad_button
{
    PAD_UP,
    PAD_DOWN,
    PAD_LEFT,
    PAD_RIGHT,
    PAD_B,
    PAD_C,
    PAD_A,
    PAD_S
};

void pad_press_button(int pad, int button);
void pad_release_button(int pad, int button);

void handle_input ( void );

#endif
