/*
 * PAINT.cpp
 *
 *  Created on: Oct 9, 2017
 *      Authors: Jose Pablo Martinez &
                 Luisa Fernanda Quesada
 */

/*
 * Message received: m_iLines and m_bColor from PIX
 */

#include "HOR.hpp"

HOR::HOR(uint16_t)
{
    m_stRect.xMin = 0;
    m_stRect.xMax = 127;
}


uint8_t HOR::run()
{
    int j;
    // Calcula los cambios en X
    m_iX = 128*(m_uint16ADC14Rx-8250)/(3100.0);
    // Calcula los cambios en Z
    m_iZ = 128*(11400-m_uint16ADC14Rz)/(6500.0);

    if(m_iX<0)
    {
        // Si el cambio es a la derecha
        // Rectangulo superior del cielo
        m_stRect.yMin = -1;
        m_stRect.yMax =  m_iZ + m_iX;
        m_uint16tulValue = 0X00FF; //blue
        Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect, m_uint16tulValue);

        // Rectangulo con línea de inclinacion
        for (j = 0; j <= -2*m_iX; j++)
        {
            m_stRect2.xMin = 0;
            m_stRect2.xMax = (128/(-2.0*m_iX))*j;
            m_stRect2.yMin = m_iZ + m_iX + j - 1;
            m_stRect2.yMax = m_iZ + m_iX + j;

            // Calcula la hipotenusa del triangulo formado y discretiza el valor a 2.0*m_iX
            m_iHip = (sqrt(128*128 + 4*m_iX*m_iX))/(-2.0*m_iX);
            // Dibuja la línea blanca
            m_stRect1.xMin = m_stRect2.xMax ;
            m_stRect1.xMax = m_stRect2.xMax + m_iHip;
            m_stRect1.yMin = m_iZ + m_iX + j - 2;
            m_stRect1.yMax = m_iZ + m_iX + j - 1;

            m_uint16tulValue = 0XAA00; //brown
            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect2, m_uint16tulValue);

            m_uint16tulValue = 0XFFFF; //white
            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect1, m_uint16tulValue);

            m_stRect2.xMin = (128/(-2.0*m_iX))*j;
            m_stRect2.xMax = 128;
            m_stRect2.yMin = m_iZ + m_iX + j - 3;
            m_stRect2.yMax = m_iZ + m_iX + j - 2;
            m_uint16tulValue = 0X00FF; //blue
            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect2, m_uint16tulValue);
        }

        // Rectangulo inferior de la tierra
        m_stRect.yMin = m_iZ - m_iX;
        m_stRect.yMax = 128;
        m_uint16tulValue = 0XAA00; //brown
        Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect, m_uint16tulValue);

    }
    else if (m_iX>0)
    {
        // Cuando se inclina a la izquierda
        m_stRect.yMin = -1;
        m_stRect.yMax =  m_iZ - m_iX ;
        m_uint16tulValue = 0X00FF; //blue
        Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect, m_uint16tulValue);

        for (j = 0; j <= 2*m_iX; j++)
        {
            m_stRect2.xMin = 0;
            m_stRect2.xMax = 128-(128/(2.0*m_iX))*j;
            m_stRect2.yMin = m_iZ - m_iX + j - 1;
            m_stRect2.yMax = m_iZ - m_iX + j ;
            m_uint16tulValue = 0X00FF; //blue
            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect2, m_uint16tulValue);

            m_iHip = (sqrt(128*128 + 4*m_iX*m_iX))/(2.0*m_iX);

            m_stRect1.xMin = m_stRect2.xMax;
            m_stRect1.xMax = m_stRect2.xMax + m_iHip;
            m_stRect1.yMin = m_iZ - m_iX + j ;
            m_stRect1.yMax = m_iZ - m_iX + j + 1;
            m_uint16tulValue = 0XFFFF; //white
            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect1, m_uint16tulValue);


            m_stRect2.xMin = 128-(128/(2.0*m_iX))*j-m_iHip;
            m_stRect2.xMax = 128;
            m_stRect2.yMin = m_iZ - m_iX + j + 1;
            m_stRect2.yMax = m_iZ - m_iX + j + 2;
            m_uint16tulValue = 0XAA00; //brown
            Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect2, m_uint16tulValue);
        }

        m_stRect.yMin = m_iZ + m_iX + 2 ;
        m_stRect.yMax = 128;
        m_uint16tulValue = 0XAA00; //brown
        Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect, m_uint16tulValue);
    }
    else
    {
        // Cuando no hay inclinación
        m_stRect.yMin = -1;
        m_stRect.yMax =  m_iZ + m_iX;
        m_uint16tulValue = 0X00FF; //blue
        Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect, m_uint16tulValue);

        m_stRect.yMin = m_iZ - m_iX  ;
        m_stRect.yMax = m_iZ - m_iX + 1;
        m_uint16tulValue = 0XFFFF; //white
        Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect, m_uint16tulValue);


        m_stRect.yMin = m_iZ - m_iX + 1;
        m_stRect.yMax = 128;
        m_uint16tulValue = 0XAA00; //brown
        Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRect, m_uint16tulValue);
    }

    m_bRunFlag = false; //- Sets RunFlag low
    return (NO_ERR);
}

//################################
// Setup
// - Defines initial value for
//   m_iLastLine as 64 (middle line
//   LCD screen
//################################

uint8_t HOR::setup()
{

    m_bRunFlag = false; // - Sets RunFlag low
 
//################################
// - Initializing screen, top half
//   sky, bottom half ground
//################################

    Graphics_Rectangle m_stRectInit;
    uint16_t l_uint16tValue;
    m_stRectInit.xMin = 0;
    m_stRectInit.xMax = 127;

    m_stRectInit.yMin = 0;
    m_stRectInit.yMax = 63;
    l_uint16tValue = 0X00CF; //- Blue
    Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRectInit,
                                    l_uint16tValue);

    m_stRectInit.yMin = 64;
    m_stRectInit.yMax = 65;
    l_uint16tValue = 0XFFFF; //- White
    Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRectInit,
                                    l_uint16tValue);
    m_stRectInit.yMin = 65;
    m_stRectInit.yMax = 127;
    l_uint16tValue = 0XAA00; //- Brown
    Graphics_fillRectangleOnDisplay(&g_sCrystalfontz128x128, &m_stRectInit,
                                    l_uint16tValue);

    return (NO_ERR);
}

//################################
// Reads message received
// - Reads int data for m_iLines
//   and bool data for m_bColor
// - Turns on RunFlag
//################################

uint8_t HOR::readMessage(st_Message *l_stNewMessage)
{
    m_uint16ADC14Rz = l_stNewMessage->std_u16IntData;
    m_uint16ADC14Rx = l_stNewMessage->std_u16IntData2;
    m_bRunFlag = true;
    return (NO_ERR);
}
