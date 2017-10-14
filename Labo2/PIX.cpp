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
#include <stdlib.h>

PIX::PIX()
{
  const float l_fPixDelta = 55;
  const int l_iPixLine = 128;
  m_u8MyTaskID = 1;
  m_u8NextTaskID = 2; // - When finished execute Task 2 = PAINT
}

uint8_t PIX::run()
{
  //################################
  // Selecting color to paint pixels
  // - false = blue
  // - true = brown
  //################################
  if(m_fNewZ > m_fPastZ)
  {
      m_bColor = true;
  }else{
      m_bColor = false;
  }

  //################################
  // Calculating number of pixels
  // that need to be painted
  //################################
  m_fDelta = abs(m_fNewZ- m_fPastZ);
  m_iLines = 1 + ((m_fDelta*l_fPixDelta - l_iPixLine)/l_iPixLine);
  m_fPastZ = m_fNewZ; //- saving new m_fPastZ

  //- Flag to tell the Scheduler the Task wants to send a message
  g_aSendMessageFlag[m_u8MyTaskID] = 1;

  //- Flag of Next Task that should be executed
  g_aExecuteNextFrame[m_u8NextTaskID] = 1;
}

uint8_t PIX::setup()
{
  m_fPastZ = 8000;//Falta calcular
}

uint8_t PAINT::readMessage(st_Message * l_stNewMessage)
{
    m_fNewZ = l_stNewMessage->std_fFloatData;
}

uint8_t PAINT::sendMessage(st_Message * l_stNewMessage)
{
    l_stNewMessage->std_bBoolData = m_bColor;
    l_stNewMessage->std_u16IntData = m_iLines;
}
