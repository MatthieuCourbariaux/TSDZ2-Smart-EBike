/*
 * TongSheng TSDZ2 motor controller firmware/
 *
 * Copyright (C) Casainho, 2018.
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include <stdio.h>
#include "stm8s.h"
#include "utils.h"

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  if (x < in_min) { return out_min; }       // if input min is smaller than output min, return the output min value
  else if (x > in_max) { return out_max; }  // if input max is bigger than output max, return the output max value

  // map the input to the output range.
  // round up if mapping bigger ranges to smaller ranges
  else  if ((in_max - in_min) > (out_max - out_min))
    return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
  // round down if mapping smaller ranges to bigger ranges
  else
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



int32_t map_inverse(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  // if input is smaller/bigger than expected return the min/max out ranges value
  if (x < in_min)
    return out_min;
  else if (x > in_max)
    return out_max;

  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



uint8_t ui8_min(uint8_t value_a, uint8_t value_b)
{
  if (value_a < value_b) return value_a;
  else return value_b;
}



uint8_t ui8_max(uint8_t value_a, uint8_t value_b)
{
  if (value_a > value_b) return value_a;
  else return value_b;
}



void ui8_limit_max(uint8_t *ui8_p_value, uint8_t ui8_max_value)
{
  if (*ui8_p_value > ui8_max_value) { *ui8_p_value = ui8_max_value; }
}



void ui8_filter(uint8_t *ui8_new_value, uint8_t *ui8_old_value, uint8_t ui8_alpha)
{
  if (ui8_alpha > 10) { ui8_alpha = 10; }
  
  *ui8_old_value = (((ui8_alpha * *ui8_new_value) + ((10 - ui8_alpha) * *ui8_old_value)) + 5) / 10;
}



void ui16_filter(uint16_t *ui16_new_value, uint16_t *ui16_old_value, uint16_t ui16_alpha)
{
  if (ui16_alpha > 10) { ui16_alpha = 10; }
  
  *ui16_old_value = (((ui16_alpha * *ui16_new_value) + ((10 - ui16_alpha) * *ui16_old_value)) + 5) / 10;
}



// from here: https://github.com/FxDev/PetitModbus/blob/master/PetitModbus.c
/*
 * Function Name        : CRC16
 * @param[in]           : ui8_data  - Data to Calculate CRC
 * @param[in/out]       : ui16_crc   - Anlik CRC degeri
 * @How to use          : First initial data has to be 0xFFFF.
 */
void crc16(uint8_t ui8_data, uint16_t* ui16_crc)
{
  unsigned int i;

  *ui16_crc = *ui16_crc ^(uint16_t) ui8_data;
  
  for (i = 8; i > 0; i--)
  {
    if (*ui16_crc & 0x0001) { *ui16_crc = (*ui16_crc >> 1) ^ 0xA001; }
    else { *ui16_crc >>= 1; }
  }
}
