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
  m_pKey = "PIX";
  m_stMssg.std_pDestKey = "PAINT";
}

uint8_t PIX::run()
{
    m_bRunFlag = 0;
  //################################
  // Selecting color to paint pixels
  // - false = blue
  // - true = brown
  //################################
  if(m_fNewZ > m_fPastZ) {
      m_bColor = true;
  } else{
      m_bColor = false;
  }

  //################################
  // Calculating number of pixels
  // that need to be painted
  //################################
  m_fDelta = abs(m_fNewZ- m_fPastZ);
  m_iLines = 1 + ((m_fDelta*l_fPixDelta - l_iPixLine)/l_iPixLine);
  m_fPastZ = m_fNewZ; //- saving new m_fPastZ
}

uint8_t PIX::setup()
{
  m_fPastZ = 8000;//Falta calcular
}

uint8_t PAINT::readMessage(st_Message *l_stNewMessage)
{
    m_fNewZ = l_stNewMessage->std_fFloatData;
    m_bRunFlag = 1;
}

uint8_t PAINT::sendMessage(st_Message *l_stNewMessage)
{
    m_stMssg->std_bBoolData = m_bColor;
    m_stMssg->std_u16IntData = m_iLines;

    l_stNewMessage = m_stMssg;
    m_bMssgFlag = 0;
}
