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

#include "../../src/cpc/cpcglob.h"

#define SOUND_PLUGIN_NONE 0
#define SOUND_PLUGIN_RETRO 1
#define SOUND_PLUGIN_AUTO 2

extern int sound_plugin;

char *soundpluginNames[] = {"NONE","RETRO","AUTO"};
#define NR_SOUND_PLUGINS 3

void convert8to16bit(signed short *ptr, long cptr) {
	signed short *dest;
	unsigned char *src;
	long srcbytes;
	//fprintf(stderr,"convert8to16bit ptr: %i, cptr:%i\n", ptr, cptr);
	srcbytes = cptr*4;
	dest = ptr + (srcbytes-2);
	src = (unsigned char *) (ptr + (srcbytes/2-1));
	//src = ptr + (srcbytes/2-1);
	while(srcbytes-- > 0) {
		//fprintf(stderr,"convert8to16bit dest: %i, src:%i, d_dest: %i, d_src: %i\n", dest, src, (((long)dest)-((long)ptr)), (((long)src)-((long)ptr)));
		*dest-- = ((*src--)-128)<<8;
	}
}

int getSoundplugin(char *s) {
	int i;
	for (i=0; i<NR_SOUND_PLUGINS; i++) {
		if (!strcmp(soundpluginNames[i],s)) {
			return i;
		}
	}
	return SOUND_PLUGIN_AUTO;
}

int autoDetectSoundplugin() {

	if (retro_AudioPlaybackPossible()) {
		return SOUND_PLUGIN_RETRO;
	}

	return SOUND_PLUGIN_NONE;
}

BOOL sound_throttle(void) {

			return FALSE;

}

