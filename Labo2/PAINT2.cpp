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
  uint16_t l_uint16tulValue;
  m_stRectInfo l_stRect = {sXMin = 0, sXMax = 127};

  if (m_bColor)
        {
            m_iNewLine = m_iLastLine + m_iLines;
            l_stRect.sYMin = m_iLastLine;
            l_stRect.sYMax = m_iNewLine;

            l_uint16tulValue = 0XAA; //brown

            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &l_stRect,
                                         l_uint16tulValue)
        }
        else
        {
            m_iNewLine = m_iLastLine - m_iLines;
            l_stRect.sYMin = m_iNewLine;
            l_stRect.sYMax = m_iLastLine;

            l_uint16tulValue = 0X36; //blue

            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &l_stRect,
                                         l_uint16tulValue)
        }
    }

uint8_t PAINT::setup()
{
  m_iLastLine = 64; //middle of LCD
  //Setup de la pantalla?
}
