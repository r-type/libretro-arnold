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

#include "display.h"

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#include "../../src/cpc/messages.h"

#include "global.h"

#include "sound.h"


int sound_plugin = SOUND_PLUGIN_NONE;

static GRAPHICS_BUFFER_INFO BufferInfo;
static GRAPHICS_BUFFER_COLOUR_FORMAT BufferColourFormat;

void Host_HandlePrinterOutput(void)
{
}


BOOL	Host_SetDisplay(int Type, int Width, int Height, int Depth)
{
		retro_SetDisplayFullscreen(Width, Height, Depth);

	return TRUE;
}


BOOL	Host_LockGraphicsBuffer(void);
GRAPHICS_BUFFER_INFO	*Host_GetGraphicsBufferInfo(void);
void	Host_UnlockGraphicsBuffer(void);
void	Host_SetPaletteEntry(int, unsigned char, unsigned char, unsigned char);
BOOL	Host_SetDisplay(int Type, int Width, int Height, int Depth);


GRAPHICS_BUFFER_COLOUR_FORMAT *Host_GetGraphicsBufferColourFormat()
{

	retro_GetGraphicsBufferColourFormat(&BufferColourFormat);

	return &BufferColourFormat;

}

GRAPHICS_BUFFER_INFO *Host_GetGraphicsBufferInfo()
{

	retro_GetGraphicsBufferInfo(&BufferInfo);
	return &BufferInfo;

}

BOOL	Host_LockGraphicsBuffer(void)
{
	return TRUE;

}


void	Host_UnlockGraphicsBuffer(void)
{

}

void	Host_SwapGraphicsBuffers(void)
{

}

void	Host_SetPaletteEntry(int Index, unsigned char R, unsigned char G, unsigned char B)
{
}

void	Host_WriteDataToSoundBuffer(unsigned char *pData, unsigned long Length)
{
	fprintf(stderr,".\n");
}


void	Host_close_audio(void) {
	
}

BOOL	Host_AudioPlaybackPossible(void)
{
	
		return TRUE;

}

SOUND_PLAYBACK_FORMAT *Host_GetSoundPlaybackFormat(void)
{
//	return retro_GetSoundPlaybackFormat();
	return NULL;
}

BOOL XWindows_ProcessSystemEvents();


BOOL	Host_ProcessSystemEvents(void)
{

Retro_handle_event();

return FALSE;

}

/* copied from /usr/include/linux/kd.h */
#define KDGETLED	0x4B31	/* return current led state */
#define KDSETLED	0x4B32	/* set led state [lights, not flags] */
#define LED_SCR		0x01	/* scroll lock led */
#define LED_CAP		0x04	/* caps lock led */
#define LED_NUM		0x02	/* num lock led */
/*
char	*fn_console = "/dev/console";
int		fd_console;		
long	led_save;
*/
void	Host_InitDriveLEDIndicator()
{
/*
		fd_console = open(fn_console, O_RDONLY);
		if(fd_console < 0) {
			fprintf(stderr, Messages[84], fn_console);
			perror(NULL);
			fprintf(stderr, "%s", Messages[85]);
		}
		ioctl(fd_console,KDGETLED,&led_save);
*/
}

void	Host_FreeDriveLEDIndicator()
{
/*
		if (fd_console < 0) return;

		ioctl(fd_console,KDSETLED,led_save);
		close(fd_console);
*/
}

void	Host_DoDriveLEDIndicator(int Drive, BOOL State)
{
/*
		long led;
		long indicator;

		if (fd_console < 0) return;

		led = ioctl(fd_console,KDGETLED,&led);
		indicator = (Drive == 0) ? LED_SCR : LED_CAP;
		if(State) led = led | indicator;
		else led = led & ~indicator;
		ioctl(fd_console,KDSETLED,led);
*/
}


void	Host_SetDirectory(char *Directory)
{
	/* fprintf(stderr,"Host_SetDirectory(%s)\n",Directory); */
	strncpy(currentDir, Directory, MAXCURDIR);
	chdir(Directory);
}

void	Host_Throttle(void)
{	

	if (AutoType_Active())
		{
			/* update it */
			AutoType_Update();
		}

	CPC_UpdateAudio();

	goto_main_thd();
}

BOOL	Host_LockAudioBuffer(unsigned char **pBlock1, unsigned long
*pBlock1Size, unsigned char **pBlock2, unsigned long *pBlock2Size, int
AudioBufferSize)
{

return retro_LockAudioBuffer(pBlock1, pBlock1Size,
				pBlock2, pBlock2Size, AudioBufferSize);

}

void	Host_UnLockAudioBuffer(void)
{

	retro_UnLockAudioBuffer();
}

void	quit(void)
{

}

