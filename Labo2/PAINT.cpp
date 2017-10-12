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
    const float l_fPixDelta = 128.0 / 7000.0;
    const int l_iPixLine = 128;
}

uint8_t PAINT::run()
{
    m_stRect.xMin = 0;
    m_stRect.xMax = 127;

    if (m_bColor)
    {
        m_iNewLine = m_iLastLine - m_iLines;
        if (m_iNewLine < 0)
        {
            m_stRect.yMin = 0;
            m_stRect.yMax = m_iLastLine;
        }
        else
        {
            m_stRect.yMin = m_iNewLine;
            m_stRect.yMax = m_iLastLine;
        }

        l_uint16tulValue = 0XAA00; //brown

        Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect,
                                        l_uint16tulValue);
    }
    else
    {
        m_iNewLine = m_iLastLine + m_iLines;

        if (m_iNewLine > 128)
        {
            m_stRect.yMin = m_iLastLine;
            m_stRect.yMax = 127;
        }
        else
        {
            m_stRect.yMin = m_iLastLine;
            m_stRect.yMax = m_iNewLine;
        }

        l_uint16tulValue = 0X00CF; //blue

        Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect,
                                        l_uint16tulValue);
    }

}

uint8_t PAINT::setup()
{
  m_iLastLine = 64; //middle of LCD
  //Setup de la pantalla?
}
