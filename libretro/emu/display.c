/*
 *  Arnold emulator (c) Copyright, Kevin Thacker 1995-2001
 *
 *  This file is part of the Arnold emulator source code distribution.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
/* display_retro */

#include "display.h"
#include "sound.h"
#include <string.h>
#include <stdlib.h>
#include "../../src/cpc/messages.h"

//#define USE_ASM 1
#define Uint16 uint16_t
#define Uint32 uint32_t

#include "libretro-core.h"
#include "retroscreen.h"

unsigned int *screen;
BOOL fullscreen = FALSE;	//FIXME
BOOL toggleFullscreenLater = FALSE;
int scale = 1;

int mode =0;

static INLINE void debug(char *s) {
	fprintf(stderr,"%s\n",s);
}

void retro_InitialiseKeyboardMapping(int);
void retro_InitialiseJoysticks(void);

void retro_SetDisplay(int Width, int Height, int Depth, BOOL wantfullscreen) {

	fullscreen = wantfullscreen;
	fprintf(stderr, Messages[106],
		Width, Height, Depth);

	screen = Retro_Screen;
	retrow=Width;
	retroh=Height;

}

void retro_SetDisplayWindowed(int Width, int Height, int Depth) {

retro_SetDisplay(Width, Height, Depth, FALSE);
	//SDL_ShowCursor(SDL_ENABLE);

}

void retro_SetDisplayFullscreen(int Width, int Height, int Depth) {

	retro_SetDisplay(Width, Height, Depth, TRUE);
	//SDL_ShowCursor(SDL_DISABLE);

}

void retro_toggleDisplayFullscreen() {

}

void retro_SetDoubled( BOOL doubled ) {

	int w, h;

	scale = doubled ? 2 : 1;
	if ( doubled ) {
		w = retrow * 2;
		h = retroh * 2;
	} else {
		w = retrow / 2;
		h = retroh / 2;
	}
	retro_SetDisplay( w, h, 32, 0);

}

static INLINE unsigned long CalcBPPFromMask(unsigned long Mask)
{
	unsigned long LocalShift = 0;
	unsigned long LocalBPP = 0;
	unsigned long LocalMask = Mask;

	if (LocalMask!=0)
	{
		do
		{
			if ((LocalMask & 0x01)!=0)
				break;

			LocalMask = LocalMask >>1;
			LocalShift++;
		}
		while (1==1);

		do
		{
			if ((LocalMask & 0x01)!=1)
				break;

			LocalMask = LocalMask>>1;
			LocalBPP++;
		}
		while (1==1);
	}

	return LocalBPP;
}

void retro_GetGraphicsBufferColourFormat(GRAPHICS_BUFFER_COLOUR_FORMAT *pFormat) {

	pFormat->Red.Mask = 0x00ff0000;
	pFormat->Red.BPP = 8;
	pFormat->Red.Shift =16;

	pFormat->Green.Mask = 0x0000ff00;
	pFormat->Green.BPP = 8;
	pFormat->Green.Shift = 8;

	pFormat->Blue.Mask =0x000000ff ;
	pFormat->Blue.BPP = 8;
	pFormat->Blue.Shift = 0;

	pFormat->BPP = 32;

	printf("BPP: %d\r\n",pFormat->BPP);
	printf("Red: M: %08x B: %d S: %d\r\n",pFormat->Red.Mask, pFormat->Red.BPP, pFormat->Red.Shift);
	printf("Green: M: %08x B: %d S: %d\r\n", pFormat->Green.Mask, pFormat->Green.BPP, pFormat->Green.Shift);
	printf("Blue: M: %08x B: %d S: %d\r\n",pFormat->Blue.Mask, pFormat->Blue.BPP, pFormat->Blue.Shift);

}

int retro_CheckDisplay(void) {
	return 0;
}

void retro_GetGraphicsBufferInfo(GRAPHICS_BUFFER_INFO *pBufferInfo) {

	pBufferInfo->pSurface = Retro_Screen;
	pBufferInfo->Width = retrow;
	pBufferInfo->Height = retroh;
	pBufferInfo->Pitch = retrow*4;

	//printf("get buffer info\r\n");
	//printf("W: %d H: %d P: %d\r\n",pBufferInfo->Width, pBufferInfo->Height,
		//pBufferInfo->Pitch);
}

BOOL retro_LockGraphicsBuffer(void) {
 return TRUE;
}

void retro_UnlockGraphicsBuffer(void) {
	
}

void scale32_array_fw_memcpy(Uint16 pitch, Uint32 *pixels32, int w, int h, int bpp) {
	int x, y, x2, y2;
	int ll = pitch/bpp;
	Uint32 *src, *dst, *dst2, pel, *srcstop;
	int i;

	y2 = h-1;
	for ( y = h/2-1; y >= 0; y-- ) {
		x2 = 0;
		for (x = 0; x < w/2; x ++) {
			pixels32[y2*ll+x2] = pixels32[y*ll+x];
			x2++;
			pixels32[y2*ll+x2] = pixels32[y*ll+x];
			x2++;
		}
		memcpy(pixels32+(y2-1)*ll, pixels32+y2*ll, pitch);
		y2-=2;
	}
}

void retro_DoubleGraphicsBuffer32(void) {
	int x, y;
	Uint16 pitch = retrow*4;
	Uint32 *pixels32 = (Uint32 *) Retro_Screen;
	int w = retrow;
	int h = retroh;
	int bpp = 32;
	int ll = pitch/bpp;
	Uint32 *src, *dst, *dst2, pel, *srcstop;
	int i;

	scale32_array_fw_memcpy(pitch, pixels32, w, h, bpp);

}

void retro_DoubleGraphicsBuffer16(void) {
	
}

void retro_DoubleGraphicsBuffer8(void) {
	
}

void retro_DoubleGraphicsBuffer(void) {

	
	retro_DoubleGraphicsBuffer32();

}

void retro_SwapGraphicsBuffers(void) {
	if ( scale == 2 ) retro_DoubleGraphicsBuffer();

}


#define FRAMES_PER_SEC 50
int retro_LockSpeed = TRUE;
//#ifdef BUSYWAIT
//static unsigned long PreviousTime=0;
//unsigned long TimeError = 0;
//#endif

void retro_Throttle(void) {
#if 0
	if (toggleFullscreenLater) {
		toggleFullscreenLater = FALSE;
		sdl_toggleDisplayFullscreen();
	}
	if (sdl_LockSpeed)
	{
		static Uint32 next_tick = 0;
		Uint32 this_tick;

		if (!sound_throttle()) {
			/* Wait for the next frame */
			this_tick = SDL_GetTicks();
			if ( this_tick < next_tick ) {
				SDL_Delay(next_tick-this_tick);
				next_tick = next_tick + (1000/FRAMES_PER_SEC);
			} else {
				next_tick = this_tick + (1000/FRAMES_PER_SEC);
			}
		}


	}

	CPC_UpdateAudio();
	sdl_HandleMouse(NULL);
#endif

}

//#include "keyboard_sdl.c"

