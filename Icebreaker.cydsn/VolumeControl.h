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
#ifndef VOLUME_CONTROL_H
#define VOLUME_CONTROL_H

#include <Config.h>
#include <Application.h>
#include <stdint.h>
    
enum
{
    vol_ctrl_right,
    vol_ctrl_left,
    vol_ctrl_both
};
    
extern int16 adcSample[];

void VolumeControlInit(void);
void VolumeControlService(void);
    
#endif /* VOLUME_CONTROL_H */

/* [] END OF FILE */
