#include <stdlib.h>

#include <stdtype.h>
#include "../EmuStructs.h"
#include "../EmuCores.h"
#include "../EmuHelper.h"

#include "2612intf.h"
#ifdef EC_YM2612_GPGX
#include "fmopn.h"
#endif
#ifdef EC_YM2612_GENS
#include "ym2612.h"
#endif


static void ym2612_gens_update(void* chip, UINT32 samples, DEV_SMPL** outputs);
static UINT8 device_start_ym2612_mame(const DEV_GEN_CFG* cfg, DEV_INFO* retDevInf);
static UINT8 device_start_ym2612_gens(const DEV_GEN_CFG* cfg, DEV_INFO* retDevInf);


#ifdef EC_YM2612_GPGX
static DEVDEF_RWFUNC devFunc_MAME[] =
{
	{RWF_REGISTER | RWF_WRITE, DEVRW_A8D8, 0, ym2612_write},
	{RWF_REGISTER | RWF_READ, DEVRW_A8D8, 0, ym2612_read},
	{0x00, 0x00, 0, NULL}
};
static DEV_DEF devDef_MAME =
{
	"YM2612", "GPGX", FCC_GPGX,
	
	device_start_ym2612_mame,
	ym2612_shutdown,
	ym2612_reset_chip,
	ym2612_update_one,
	
	ym2612_setoptions,	// SetOptionBits
	ym2612_set_mutemask,
	NULL,	// SetPanning
	NULL,	// SetSampleRateChangeCallback
	NULL,	// LinkDevice
	
	devFunc_MAME,	// rwFuncs
};
#endif
#ifdef EC_YM2612_GENS
static DEVDEF_RWFUNC devFunc_Gens[] =
{
	{RWF_REGISTER | RWF_WRITE, DEVRW_A8D8, 0, YM2612_Write},
	{RWF_REGISTER | RWF_READ, DEVRW_A8D8, 0, YM2612_Read},
	{0x00, 0x00, 0, NULL}
};
static DEV_DEF devDef_Gens =
{
	"YM2612", "Gens", FCC_GENS,
	
	device_start_ym2612_gens,
	(DEVFUNC_CTRL)YM2612_End,
	(DEVFUNC_CTRL)YM2612_Reset,
	ym2612_gens_update,
	
	(DEVFUNC_OPTMASK)YM2612_SetOptions,	// SetOptionBits
	(DEVFUNC_OPTMASK)YM2612_SetMute,
	NULL,	// SetPanning
	NULL,	// SetSampleRateChangeCallback
	NULL,	// LinkDevice
	
	devFunc_Gens,	// rwFuncs
};
#endif

const DEV_DEF* devDefList_YM2612[] =
{
#ifdef EC_YM2612_GPGX
	&devDef_MAME,
#endif
#ifdef EC_YM2612_GENS
	&devDef_Gens,
#endif
	NULL
};


#ifdef EC_YM2612_GENS
static void ym2612_gens_update(void* chip, UINT32 samples, DEV_SMPL** outputs)
{
	ym2612_* YM2612 = (ym2612_*)chip;
	
	YM2612_ClearBuffer(outputs, samples);
	YM2612_Update(YM2612, outputs, samples);
	YM2612_DacAndTimers_Update(YM2612, outputs, samples);
	
	return;
}
#endif


#ifdef EC_YM2612_GPGX
static UINT8 device_start_ym2612_mame(const DEV_GEN_CFG* cfg, DEV_INFO* retDevInf)
{
	void* chip;
	DEV_DATA* devData;
	UINT32 clock;
	UINT32 rate;
	
	clock = CHPCLK_CLOCK(cfg->clock);
	
	rate = clock / 72 / 2;
	//if (PseudoSt & 0x02))
	//	rate *= 2;
	SRATE_CUSTOM_HIGHEST(cfg->srMode, rate, cfg->smplRate);
	
	chip = ym2612_init(NULL, clock, rate, NULL, NULL);
	
	devData = (DEV_DATA*)chip;
	devData->chipInf = chip;
	INIT_DEVINF(retDevInf, devData, rate, &devDef_MAME);
	return 0x00;
}
#endif

#ifdef EC_YM2612_GENS
static UINT8 device_start_ym2612_gens(const DEV_GEN_CFG* cfg, DEV_INFO* retDevInf)
{
	ym2612_* chip;
	DEV_DATA* devData;
	UINT32 clock;
	UINT32 rate;
	
	clock = CHPCLK_CLOCK(cfg->clock);
	
	rate = clock / 72 / 2;
	SRATE_CUSTOM_HIGHEST(cfg->srMode, rate, cfg->smplRate);
	
	chip = YM2612_Init(clock, rate, 0);
	
	devData = (DEV_DATA*)chip;
	devData->chipInf = chip;
	INIT_DEVINF(retDevInf, devData, rate, &devDef_Gens);
	return 0x00;
}
#endif
