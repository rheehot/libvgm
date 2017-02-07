#ifndef __SN76496_H__
#define __SN76496_H__

#include <stdtype.h>
#include "../snddef.h"

UINT8 sn76496_ready_r(void *chip, UINT8 offset);
void sn76496_write_reg(void *chip, UINT8 offset, UINT8 data);
void sn76496_stereo_w(void *chip, UINT8 offset, UINT8 data);

void SN76496Update(void *param, UINT32 samples, DEV_SMPL** outputs);
UINT32 sn76496_start(void **chip, UINT32 clock, int shiftregwidth, int noisetaps,
					int negate, int stereo, int clockdivider, int freq0);
void sn76496_connect_t6w28(void *noisechip, void *tonechip);
void sn76496_shutdown(void *chip);
void sn76496_reset(void *chip);
void sn76496_freq_limiter(void* chip, int sample_rate);
void sn76496_set_mutemask(void *chip, UINT32 MuteMask);

#endif	// __SN76496_H__
