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
#include <Application.h>
#include <utils.h>
#include <stdint.h>

void printArray(uint8_t *p, uint32_t len)
{
  while (len--)
  {
    D_PRINTF(DEBUG, "%02X ", *p++);
  }
  D_PRINTF(DEBUG, "\n");
}
/* [] END OF FILE */
