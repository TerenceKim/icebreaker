/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef PCM1770_H
#define PCM1770_H
    
#include <Application.h>
    
#define PCM1770_REG_HPOUT_L 1
#define PCM1770_REG_HPOUT_R 2
#define PCM1770_REG_OVERSAMPLING_CONTROL 3
#define PCM1770_REG_ZCAT_PWRD 4
    
#define PCM1770_NUM_OF_REGS   5 // including 1 for 0th index

typedef struct
{
    unsigned atl:6;
    unsigned mutl:1;
    unsigned mutr:1;
} pcm1770_reg_hpout_l_s;
    
void pcm1770_init(void);
void pcm1770_volume_set(int8 volume);
void pcm1770_volume_set_level(uint8 level);
void pcm1770_mute_set(bool left, bool right);
void pcm1770_reg_write(uint8 reg, uint8 data);
uint8 pcm1770_reg_read(uint8 reg);

#endif /* PCM1770_H */

/* [] END OF FILE */
