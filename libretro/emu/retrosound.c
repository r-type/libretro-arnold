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
#include <stdlib.h>
#include "../../src/cpc/messages.h"
#include "sound.h"

//SOUND_PLAYBACK_FORMAT SoundFormat;

static signed short backbuffer[1760*2];

static int commitBufferSize;

BOOL	retro_AudioPlaybackPossible(void)
{
	return TRUE;
}

BOOL	retro_LockAudioBuffer(unsigned char **pBlock1, unsigned long
*pBlock1Size, unsigned char **pBlock2, unsigned long *pBlock2Size, int
AudioBufferSize)
{
	*pBlock1 = (unsigned char *) backbuffer;
	*pBlock1Size = AudioBufferSize;
	*pBlock2 = NULL;
	*pBlock2Size = 0; 
	commitBufferSize = AudioBufferSize/2;

	return TRUE;
}

SOUND_PLAYBACK_FORMAT *retro_GetSoundPlaybackFormat(void)
{
	SOUND_PLAYBACK_FORMAT *SoundFormat =(SOUND_PLAYBACK_FORMAT*)malloc(sizeof(SOUND_PLAYBACK_FORMAT));  

	SoundFormat->NumberOfChannels = 2;
	SoundFormat->BitsPerSample = 8;
	SoundFormat->Frequency = 44100;
	fprintf(stderr,"oss_GetSoundPlaybackFormat channels:%i, BitsPerSample: %i, Frequency: %i\n", SoundFormat->NumberOfChannels, SoundFormat->BitsPerSample, SoundFormat->Frequency);
	return SoundFormat;
}

void	retro_UnLockAudioBuffer(void)
{
	signed short *ptr;
	int err;
	long cptr;
	static int skipfirst = 1;

	int i;

	if (skipfirst) {
		skipfirst--;
		return;
	}

	ptr = backbuffer;
	cptr = commitBufferSize;

	convert8to16bit(ptr, cptr);

	while (cptr > 0) {

		retro_audio_cb(*ptr,*(ptr+1) );

		ptr += 2;
		cptr -=1;
	}

	return;

}


