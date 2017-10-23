/*
 * PIX.cpp
 *
 *  Created on: Oct 9, 2017
 *      Authors: Jose Pablo Martinez &
 Luisa Fernanda Quesada
 */

/*
 * Mensaje recibido: m_fNewZ de ADC
 * Mensaje enviado: m_bColor y m_iLines a PAINT
 */

#include "PIX.hpp"

PIX::PIX(uint16_t)
{
    l_fPixDelta = 128.0 / 5600.0; // (# Pixel Lines on LCD)/(Change in Z accelerometer) 
    l_iPixLine = 128; // (# Pixels per line in LCD)
}

//################################
// Run
// - Sets color to paint pixels
// - Calculates numer of pixel lines
//   to be painted
//################################

uint8_t PIX::run()
{
    m_bMinLimit = 0; 
    m_bMaxLimit = 0;

    //################################
    // Selecting color to paint pixels
    // - false = blue
    // - true = brown
    //################################

    if (m_fNewZ > m_fPastZ)
    {
        m_bColor = true;
    }
    else
    {
        m_bColor = false;
    }

    //################################
    // Limiting the newZ
    //################################

    /*if (m_fNewZ > 11350)
    {
        m_bMaxLimit = 1;
    }
    else
    {
        if (m_fNewZ < 5000)
        {
            m_bMinLimit = 1;
        }
    }*/



    //################################
    // Calculating number of pixels
    // that need to be painted
    //################################
    m_fDelta = abs(m_fNewZ - m_fPastZ);
    m_iLines = 1 + ((m_fDelta * l_fPixDelta * l_iPixLine - l_iPixLine) / l_iPixLine);
    m_fPastZ = m_fNewZ; //- Saving new m_fPastZ

    m_bMssgFlag = true; //- Sets MssgFlag high
    m_bRunFlag = false; //- Sets RunFlag low

    return (NO_ERR);
}
;

//################################
// Setup
// - Defines an initial value for
//   PastZ variable
//################################

uint8_t PIX::setup()
{
    m_bMinLimit = 0;
    m_bMaxLimit = 0;
    m_fPastZ = 8700; //- Z accelerometer value in vertical position (starting position)
    m_bRunFlag = false; //- Sets RunFlag low
    return (NO_ERR);
}
;

//################################
// Reads message received
// - Reads float data for ADC Z
//   component and stores is in
//   m_fNewZ
// - Turns on RunFlag
//################################

uint8_t PIX::readMessage(st_Message *l_stNewMessage)
{
    m_fNewZ = l_stNewMessage->std_u16IntData;
    m_bRunFlag = true;
    m_breceivedTestMailbox = true;
    return (NO_ERR);
}
;

//################################
// Sends message
// - Sends the color that needs to
//   be painted on the LED screen
//   and the amount of pixel lines
// - Turns off MssgFlag
//################################

uint8_t PIX::sendMessage(st_Message *l_stNewMessage)
{
    m_stMssg.std_bBoolData = m_bColor;
    m_stMssg.std_u16IntData = m_iLines;
    m_stMssg.std_bBoolData2 = m_bMaxLimit;
    m_stMssg.std_bBoolData2 = m_bMinLimit;
    *l_stNewMessage = m_stMssg;
    m_bMssgFlag = false;
    return (NO_ERR);
}
