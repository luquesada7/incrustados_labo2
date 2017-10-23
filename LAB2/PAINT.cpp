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

PAINT::PAINT(uint16_t)
{
    l_fPixDelta = 128.0 / 6000.0;
    l_iPixLine = 128;
    //m_pKey = "PAINT";
}

//################################
// Run
// - Prints rectangle on LCD
//   depending on the data received
//   by PIX.
//################################

uint8_t PAINT::run()
{

    m_stRect.xMin = 0; // - Defines the x coordinate variables for the starting
    m_stRect.xMax = 127; // and finishing point of the rectangle to be drawn
    uint16_t l_uint16tulValue;

    /*If color is brown (true) calculate m_iNewLine
     and paint LCD from m_iLastLine to m_iNewLine*/
    if (m_iLines > 5)
    {
        m_iLines = m_iLines - 5;
        m_iPaintLines = 5;
    }
    else
    {
        m_iPaintLines = m_iLines;
        m_breceivedTestMailbox = false;
        m_bRunFlag = false;
    }

    if (m_bColor)
    {
        m_iNewLine = m_iLastLine - m_iPaintLines;
        if (m_iNewLine < 0 || m_bMaxLimit == 1)
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
        m_iNewLine = m_iLastLine + m_iPaintLines;

        if (m_iNewLine > 128 || m_bMinLimit == 1)
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

    m_iLastLine = m_iNewLine; //- saving past m_iNewLine on m_iLastLine

    return (NO_ERR);
}

//################################
// Setup
// - Defines initial value for
//   m_iLastLine as 64 (middle line
//   LCD screen
//################################

uint8_t PAINT::setup()
{
    m_iLastLine = 64; //middle of LCD
    m_bRunFlag = false; // - Sets RunFlag off
    m_breceivedTestMailbox = false;

    Graphics_Rectangle l_stRectInit;
    uint16_t l_uint16tValue;
    l_stRectInit.xMin = 0;
    l_stRectInit.xMax = 127;
    l_stRectInit.yMin = 0;
    l_stRectInit.yMax = 63;
    l_uint16tValue = 0X00CF; //blue
    Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &l_stRectInit,
                                    l_uint16tValue);
    l_stRectInit.xMin = 0;
    l_stRectInit.xMax = 127;
    l_stRectInit.yMin = 64;
    l_stRectInit.yMax = 127;
    l_uint16tValue = 0XAA00; //brown
    Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &l_stRectInit,
                                    l_uint16tValue);

    return (NO_ERR);
}

//################################
// Reads message received
// - Reads int data for m_iLines
//   and bool data for m_bColor
// - Turns on RunFlag
//################################

uint8_t PAINT::readMessage(st_Message *l_stNewMessage)
{
    m_iLines = l_stNewMessage->std_u16IntData;
    m_bColor = l_stNewMessage->std_bBoolData;
    m_bMaxLimit = l_stNewMessage->std_bBoolData2;
    m_bMinLimit = l_stNewMessage->std_bBoolData3;
    m_breceivedTestMailbox = true;
    m_bRunFlag = true;
    return (NO_ERR);
}
