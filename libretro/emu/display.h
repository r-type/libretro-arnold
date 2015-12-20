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

#include "../../src/cpc/host.h"

#include <unistd.h>
#include <sys/time.h>
#include "../../src/cpc/cpc.h"
void retro_SetDisplayWindowed(int Width, int Height, int Depth);
void retro_SetDisplayFullscreen(int Width, int Height, int Depth);
void retro_SetDoubled( BOOL doubled );
void retro_GetGraphicsBufferColourFormat(GRAPHICS_BUFFER_COLOUR_FORMAT *pFormat);
int retro_CheckDisplay(void);
void retro_GetGraphicsBufferInfo(GRAPHICS_BUFFER_INFO *pBufferInfo);
BOOL retro_LockGraphicsBuffer(void);
void retro_UnlockGraphicsBuffer(void);
void retro_SwapGraphicsBuffers(void);
BOOL retro_ProcessSystemEvents(void);
void retro_Throttle(void);
extern int retro_LockSpeed;
void retro_HandleMouse(int event);
#define QWERTY 0
#define QWERTZ 1
#define AZERTY 2
#define SPANISH 3
void retro_InitialiseKeyboardMapping(int layout);


