 /*
 * The notice following this comment block is provided as-is, intact, and
 * unmodified, as is required by the stipulations therein.
 * August 24, 2021
 * -ramblecube
 */
 
 /****************************************************************************
 * Copyright 2009 The Lemon Man and thanks to luccax, Wiipower, Aurelio and crediar
 * Copyright 2010 Dimok
 * Copyright 2011 FIX94, oggzee
 *
 * Original forwarder source by
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 ***************************************************************************/

#include <gccore.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fat.h>
#include <ogc/machine/processor.h>
#include <wiiuse/wpad.h>

#define EXECUTE_ADDR	((u8 *) 0x92000000)
#define BOOTER_ADDR		((u8 *) 0x93000000)
#define ARGS_ADDR		((u8 *) 0x93200000)

typedef void (*entrypoint) (void);
extern void __exception_setreload(int t);
extern void __exception_closeall();
extern const u8 app_booter_bin[];
extern const u32 app_booter_bin_size;

int main(int argc, char *argv[])
{
	u32 cookie;
	FILE *exeFile = NULL;
	void *exeBuffer = (void *)EXECUTE_ADDR;
	u32 exeSize = 0;
	entrypoint exeEntryPoint;
	__exception_setreload(0);

	void *xfb = NULL;
	GXRModeObj *rmode = NULL;

	/*
	* Initialize video subsystem
	*/

	VIDEO_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(TRUE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) 
		VIDEO_WaitVSync();

	/*
	* Attempt to initialize FAT filesystem (SD card) support, launch Wii Shop
	* with error code in case of failure.
	*/
	
	if (!fatInitDefault()) {
		WII_LaunchTitleWithArgs(0x0001000248414241L, 0, "/error?error=FAT_INIT_FAILED", NULL);
	}

	/*
	* Initialize Wii remote subsystem
	*/

	WPAD_Init();

	/*
	* Attempt to open & create file handler for target DOL, launch Wii Shop with
	* error code in case of failure.
	*/

	exeFile = fopen("/apps/oscdownload/boot.dol","rb");

	if (exeFile < 0) {
		fclose(exeFile);
		fatUnmount(0);
		WII_LaunchTitleWithArgs(0x0001000248414241L, 0, "/error?error=DOL_OPEN_FAILED", NULL);
	}

	/*
	* Attempt to read target DOL into buffer, launch Wii Shop with error code in
	* case of failure.
	*/

	fseek(exeFile, 0, SEEK_END);
	exeSize = ftell(exeFile);
	rewind(exeFile);

	if (fread(exeBuffer, 1, exeSize, exeFile) != exeSize) {
		fclose(exeFile);
		fatUnmount(0);
		WII_LaunchTitleWithArgs(0x0001000248414241L, 0, "/error?error=DOL_BUFF_FAILED", NULL);
	}

	/*
	* Close file handler, unmount FAT (SD card) file system, reload + launch
	* DOL in buffer
	*/

	fclose(exeFile);

	memcpy(BOOTER_ADDR, app_booter_bin, app_booter_bin_size);
	DCFlushRange(BOOTER_ADDR, app_booter_bin_size);

	fatUnmount(0);

	exeEntryPoint = (entrypoint)BOOTER_ADDR;
	SYS_ResetSystem(SYS_SHUTDOWN, 0, 0);
	_CPU_ISR_Disable(cookie);
	__exception_closeall();
	exeEntryPoint();
	_CPU_ISR_Restore(cookie);
	return 0;
}
