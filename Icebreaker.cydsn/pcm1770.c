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
#include "pcm1770.h"
#include <cypins.h>
#include <Application.h>
#include <Codec.h>
#include <SPI_CLK.h>
#include <SPI_CS.h>
#include <SPI_MOSI.h>
#include <PD.h>

#define SPI_CS_PIN      3
#define SPI_CLK_PIN     2
#define SPI_MOSI_PIN    0

#define NUM_OF_STEPS    63

#define PCM1770_VOL_MIN -63
#define PCM1770_VOL_MAX 0

#define WritePin(pp, s) pp ## _Write(s)

#define ConfigPin(pp, m)  CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT2_DR, pp ## _PIN, m)

#define SPI_CS_Config(m)   ConfigPin(SPI_CS, m)
#define SPI_CLK_Config(m)  ConfigPin(SPI_CLK, m)
#define SPI_MOSI_Config(m) ConfigPin(SPI_MOSI, m)
//#define SPI_CS_Write(s)    WritePin(SPI_CS, s)
//#define SPI_CLK_Write(s)   WritePin(SPI_CLK, s)
//#define SPI_MOSI_Write(s)  WritePin(SPI_MOSI, s)

static uint8 pcm1770_regs[] =
{
    0,  // Reserved
    63, // PCM1770_REG_HPOUT_L
    63, // PCM1770_REG_HPOUT_R
    0,  // PCM1770_REG_OVERSAMPLING_CONTROL 3
    0,  // PCM1770_REG_ZCAT_PWRD 4
};

void pcm1770_init(void)
{
    //CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT2_DR, 3, CY_SYS_PINS_DM_STRONG);
    //CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT2_DR, 2, CY_SYS_PINS_DM_STRONG);
    //CY_SYS_PINS_SET_DRIVE_MODE(CYREG_PRT2_DR, 0, CY_SYS_PINS_DM_STRONG);
    //SPI_CS_Config(CY_SYS_PINS_DM_STRONG);
    //SPI_CLK_Config(CY_SYS_PINS_DM_STRONG);
    //SPI_MOSI_Config(CY_SYS_PINS_DM_STRONG);
    
    //SPI_CS_Write(1);
    //SPI_CLK_Write(1);
    //SPI_MOSI_Write(1);

    CyDelay(5);

    // Power on
    PD_Write(1);
    
    CyDelay(5);
    
    pcm1770_reg_write(PCM1770_REG_OVERSAMPLING_CONTROL, 0x01);
    pcm1770_reg_write(PCM1770_REG_ZCAT_PWRD, 0x10);
}

static void pcm1770_send_halfword(uint16 data, bool toggle_cs)
{
    int i;

    if (toggle_cs)
    {
        SPI_CS_Write(0);
    }

    for (i = 0; i < 16; i++)
    {
        SPI_CLK_Write(0);
        SPI_MOSI_Write((data & 0x8000) != 0);
        data <<= 1;
        SPI_CLK_Write(1);
    }

    if (toggle_cs)
    {
        SPI_CS_Write(1);
    }
}

void pcm1770_send_data(uint16 *pData, uint16 len)
{
    int i;
    
    SPI_CS_Write(0);
    
    for (i = 0; i < len; i++)
    {
        pcm1770_send_halfword(pData[i], false);
    }
}

void pcm1770_reg_write(uint8 reg, uint8 data)
{
    uint16 tmp = reg;
    
    pcm1770_regs[reg] = data;
    
    tmp = (tmp << 8) | data;
    pcm1770_send_halfword(tmp, true);
}

void pcm1770_volume_set(int8 volume)
{
    uint8 data = pcm1770_regs[PCM1770_REG_HPOUT_L];
    pcm1770_reg_hpout_l_s *pData = (pcm1770_reg_hpout_l_s *)&data;

    if (volume > PCM1770_VOL_MAX)
    {
        volume = PCM1770_VOL_MAX;
    }
    
    if (volume < PCM1770_VOL_MIN)
    {
        volume = PCM1770_VOL_MIN;
    }

    D_PRINTF(DEBUG, "%s: %d -> %d\n", __FUNCTION__, pData->atl - NUM_OF_STEPS, volume);

    pData->atl = volume + NUM_OF_STEPS;
    pcm1770_reg_write(1, data);
    pcm1770_reg_write(2, volume + NUM_OF_STEPS);
}

void pcm1770_volume_set_level(uint8 level)
{
    int32 volume = level;

    // Round with 1000th digit precision
    volume = ((volume * 1000 / PC_VOLUME_MSB_MAX) * NUM_OF_STEPS + 500) / 1000 + PCM1770_VOL_MIN;

    pcm1770_volume_set((int8) volume);
}

void pcm1770_mute_set(bool left, bool right)
{
    uint8 data = pcm1770_regs[PCM1770_REG_HPOUT_L];
    pcm1770_reg_hpout_l_s *pData = (pcm1770_reg_hpout_l_s *)&data;

    pData->mutl = left;
    pData->mutr = right;

    pcm1770_reg_write(PCM1770_REG_HPOUT_L, data);
}

uint8 pcm1770_reg_read(uint8 reg)
{
    if (reg >= PCM1770_NUM_OF_REGS)
    {
        return 0;
    }

    return pcm1770_regs[reg];
}

/* [] END OF FILE */
