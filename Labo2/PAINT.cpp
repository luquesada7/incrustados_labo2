/*
 * PAINT.cpp
 *
 *  Created on: Oct 9, 2017
 *      Authors: Jose Pablo Martinez &
                 Luisa Fernanda Quesada
 */

 /*
  * Mensaje recibido: m_iLines y m_bColor de PIX
  * Mensaje enviado: desde PAINT m_iLastLine = m_iLastLine + m_iLines
 */

#include "PAINT.hpp"

PAINT::PAINT()
{
}

uint8_t PAINT::run()
{
  m_iPixels = m_iLines*128;
  m_iLastPixel = m_iLastLine*128;

  HAL_LCD_writeCommand(CM_RAMWR);
  if(bool)
  {
    int i;
    for(i=(m_iLastPixel+1); i>=(m_iPixels+m_iLastPixel); i++)
    {
      if(i>16384)
      {
        break;
      }
      HAL_LCD_writeData(0XAA); //brown
      HAL_LCD_writeData(0XAA); //brown
    }
  }else{
    int j;
    for(j=(m_iLastPixel); j<=(m_iPixels-m_iLastPixel); j--)
    {
      if(j<0)
      {
        break;
      }
      HAL_LCD_writeData(0XAA); //blue
      HAL_LCD_writeData(0XAA); //blue
    }
  }

  HAL_LCD_delay(10);
  HAL_LCD_writeCommand(CM_DISPON);
}

uint8_t PAINT::setup()
{
  m_iLastLine = 64; //middle of LCD
  //Setup de la pantalla? 
}
