#include "libretro.h"

#include "../src/cpc/cpc.h"

static int KeySymToCPCKey[512];
extern unsigned char MXjoy[2]; // joy
extern int SHOWKEY;
extern int SHIFTON;

int oldjoy0=0;

void	retro_InitialiseKeyboardMapping()
{
	int	 i;

	for (i=0; i<512; i++)
	{
		KeySymToCPCKey[i] = CPC_KEY_NULL;
	}

	/* International key mappings */
	KeySymToCPCKey[RETROK_0] = CPC_KEY_ZERO;
	KeySymToCPCKey[RETROK_1] = CPC_KEY_1;
	KeySymToCPCKey[RETROK_2] = CPC_KEY_2;
	KeySymToCPCKey[RETROK_3] = CPC_KEY_3;
	KeySymToCPCKey[RETROK_4] = CPC_KEY_4;
	KeySymToCPCKey[RETROK_5] = CPC_KEY_5;
	KeySymToCPCKey[RETROK_6] = CPC_KEY_6;
	KeySymToCPCKey[RETROK_7] = CPC_KEY_7;
	KeySymToCPCKey[RETROK_8] = CPC_KEY_8;
	KeySymToCPCKey[RETROK_9] = CPC_KEY_9;
	KeySymToCPCKey[RETROK_a] = CPC_KEY_A;
	KeySymToCPCKey[RETROK_b] = CPC_KEY_B;
	KeySymToCPCKey[RETROK_c] = CPC_KEY_C;
	KeySymToCPCKey[RETROK_d] = CPC_KEY_D;
	KeySymToCPCKey[RETROK_e] = CPC_KEY_E;
	KeySymToCPCKey[RETROK_f] = CPC_KEY_F;
	KeySymToCPCKey[RETROK_g] = CPC_KEY_G;
	KeySymToCPCKey[RETROK_h] = CPC_KEY_H;
	KeySymToCPCKey[RETROK_i] = CPC_KEY_I;
	KeySymToCPCKey[RETROK_j] = CPC_KEY_J;
	KeySymToCPCKey[RETROK_k] = CPC_KEY_K;
	KeySymToCPCKey[RETROK_l] = CPC_KEY_L;
	KeySymToCPCKey[RETROK_m] = CPC_KEY_M;
	KeySymToCPCKey[RETROK_n] = CPC_KEY_N;
	KeySymToCPCKey[RETROK_o] = CPC_KEY_O;
	KeySymToCPCKey[RETROK_p] = CPC_KEY_P;
	KeySymToCPCKey[RETROK_q] = CPC_KEY_Q;
	KeySymToCPCKey[RETROK_r] = CPC_KEY_R;
	KeySymToCPCKey[RETROK_s] = CPC_KEY_S;
	KeySymToCPCKey[RETROK_t] = CPC_KEY_T;
	KeySymToCPCKey[RETROK_u] = CPC_KEY_U;
	KeySymToCPCKey[RETROK_v] = CPC_KEY_V;
	KeySymToCPCKey[RETROK_w] = CPC_KEY_W;
	KeySymToCPCKey[RETROK_x] = CPC_KEY_X;
	KeySymToCPCKey[RETROK_y] = CPC_KEY_Y;
	KeySymToCPCKey[RETROK_z] = CPC_KEY_Z;
	KeySymToCPCKey[RETROK_SPACE] = CPC_KEY_SPACE;
	KeySymToCPCKey[RETROK_COMMA] = CPC_KEY_COMMA;
	KeySymToCPCKey[RETROK_PERIOD] = CPC_KEY_DOT;
	KeySymToCPCKey[RETROK_SEMICOLON] = CPC_KEY_COLON;
	KeySymToCPCKey[RETROK_MINUS] = CPC_KEY_MINUS;
	KeySymToCPCKey[RETROK_EQUALS] = CPC_KEY_HAT;
	KeySymToCPCKey[RETROK_LEFTBRACKET] = CPC_KEY_AT;
	KeySymToCPCKey[RETROK_RIGHTBRACKET] =CPC_KEY_OPEN_SQUARE_BRACKET;

	KeySymToCPCKey[RETROK_TAB] = CPC_KEY_TAB;
	KeySymToCPCKey[RETROK_RETURN] = CPC_KEY_RETURN;
	KeySymToCPCKey[RETROK_BACKSPACE] = CPC_KEY_DEL;
	KeySymToCPCKey[RETROK_ESCAPE] = CPC_KEY_ESC;

	//KeySymToCPCKey[RETROK_Equals & 0x0ff)] = CPC_KEY_CLR;

	KeySymToCPCKey[RETROK_UP] = CPC_KEY_CURSOR_UP;
	KeySymToCPCKey[RETROK_DOWN] = CPC_KEY_CURSOR_DOWN;
	KeySymToCPCKey[RETROK_LEFT] = CPC_KEY_CURSOR_LEFT;
	KeySymToCPCKey[RETROK_RIGHT] = CPC_KEY_CURSOR_RIGHT;

	KeySymToCPCKey[RETROK_KP0] = CPC_KEY_F0;
	KeySymToCPCKey[RETROK_KP1] = CPC_KEY_F1;
	KeySymToCPCKey[RETROK_KP2] = CPC_KEY_F2;
	KeySymToCPCKey[RETROK_KP3] = CPC_KEY_F3;
	KeySymToCPCKey[RETROK_KP4] = CPC_KEY_F4;
	KeySymToCPCKey[RETROK_KP5] = CPC_KEY_F5;
	KeySymToCPCKey[RETROK_KP6] = CPC_KEY_F6;
	KeySymToCPCKey[RETROK_KP7] = CPC_KEY_F7;
	KeySymToCPCKey[RETROK_KP8] = CPC_KEY_F8;
	KeySymToCPCKey[RETROK_KP9] = CPC_KEY_F9;

	KeySymToCPCKey[RETROK_KP_PERIOD] = CPC_KEY_FDOT;

	KeySymToCPCKey[RETROK_LSHIFT] = CPC_KEY_SHIFT;
	KeySymToCPCKey[RETROK_RSHIFT] = CPC_KEY_SHIFT;
	KeySymToCPCKey[RETROK_LCTRL] = CPC_KEY_CONTROL;
	KeySymToCPCKey[RETROK_RCTRL] = CPC_KEY_CONTROL;
	KeySymToCPCKey[RETROK_CAPSLOCK] = CPC_KEY_CAPS_LOCK;

	KeySymToCPCKey[RETROK_KP_ENTER] = CPC_KEY_SMALL_ENTER;

	KeySymToCPCKey[RETROK_DELETE] = CPC_KEY_JOY_LEFT;
	KeySymToCPCKey[RETROK_END] = CPC_KEY_JOY_DOWN;
	KeySymToCPCKey[RETROK_PAGEDOWN] = CPC_KEY_JOY_RIGHT;
	KeySymToCPCKey[RETROK_INSERT] = CPC_KEY_JOY_FIRE1;
	KeySymToCPCKey[RETROK_HOME] = CPC_KEY_JOY_UP;
	KeySymToCPCKey[RETROK_PAGEUP] = CPC_KEY_JOY_FIRE2;

	KeySymToCPCKey[0x0134] = CPC_KEY_COPY;			/* Alt */
	KeySymToCPCKey[0x0137] = CPC_KEY_COPY;			/* Compose */

}


void Retro_handle_event()
{
	
	Retro_PollEvent();

	if(SHOWKEY==1)return; //no kbdjoy while virtualkbd!

	if(MXjoy[0]&0x01)CPC_SetKey(CPC_KEY_JOY_UP);
	else if (oldjoy0&0x01)CPC_ClearKey(CPC_KEY_JOY_UP);

	if(MXjoy[0]&0x02)CPC_SetKey(CPC_KEY_JOY_DOWN);
	else if (oldjoy0&0x02)CPC_ClearKey(CPC_KEY_JOY_DOWN);

	if(MXjoy[0]&0x04)CPC_SetKey(CPC_KEY_JOY_LEFT);
	else if (oldjoy0&0x04 )CPC_ClearKey(CPC_KEY_JOY_LEFT);

	if(MXjoy[0]&0x08)CPC_SetKey(CPC_KEY_JOY_RIGHT);
	else if (oldjoy0&0x08 )CPC_ClearKey(CPC_KEY_JOY_RIGHT);

	if(MXjoy[0]&0x80)CPC_SetKey(CPC_KEY_JOY_FIRE1);
	else if (oldjoy0&0x80 )CPC_ClearKey(CPC_KEY_JOY_FIRE1);
	
	if(MXjoy[0]&0x40)CPC_SetKey(CPC_KEY_JOY_FIRE2);
	else if (oldjoy0&0x40 )CPC_ClearKey(CPC_KEY_JOY_FIRE2);

	oldjoy0=MXjoy[0];

}

void retro_key_down(int key)
{
	int code;

	if(key<512)
 		code=KeySymToCPCKey[key];	
	else code = CPC_KEY_NULL;
	CPC_SetKey(code);
}

void retro_key_up(int key)
{
	int code;

	if(key<512)
 		code=KeySymToCPCKey[key];
	else code = CPC_KEY_NULL;
	CPC_ClearKey(code);
}

//FIXME VIRTULAL KBD HANDLE

void vkbd_key(int key,int pressed){

	//printf("key(%x)=%x shift:%d (%d)\n",key,pressed,SHIFTON,((key>>4)*8)+(key&7));

	int key2=((key>>4)*8)+(key&7);

	if(pressed){

		if(SHIFTON==1)
			CPC_SetKey(21);
		
		CPC_SetKey(key2);

	}
	else {
		if(SHIFTON==1)			
			CPC_ClearKey(21);

		CPC_ClearKey(key2);
	}

}

