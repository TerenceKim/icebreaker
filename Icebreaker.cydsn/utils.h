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
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
  
#define BE16(h) (((h & 0xFF00) >> 8) | \
                 ((h & 0x00FF) << 8))

#define BE32(i) (((i & 0xFF000000) >> 24) | \
                 ((i & 0x00FF0000) >>  8) | \
                 ((i & 0x0000FF00) <<  8) | \
                 ((i & 0x000000FF) << 24))
                  
void printArray(uint8_t *p, uint32_t len);

#endif /* UTILS_H */

/* [] END OF FILE */
