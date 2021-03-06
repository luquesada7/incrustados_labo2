/*
 * PIX.cpp
 *
 *  Created on: Oct 9, 2017
 *      Authors: Luisa Fernanda Quesada &
 *               Jose Pablo Martinez
 */

 /*
  * Mensaje recibido: m_fNewZ de ADC
  * Mensaje enviado: m_bColor y m_iLines a PAINT
 */

#include "PIX.hpp"


PIX::PIX(uint16_t)
{
  l_fPixDelta = 128.0 / 6000.0;;
  l_iPixLine = 128;
}

uint8_t PIX::run()
{

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
  //m_iLines = 1 + ((m_fDelta*l_fPixDelta - l_iPixLine)/l_iPixLine);
   m_iLines = 1 + ((m_fDelta * l_fPixDelta * l_iPixLine - l_iPixLine) / l_iPixLine);
   m_fPastZ = m_fNewZ; //- saving new m_fPastZ


  m_bMssgFlag = true;
  m_bRunFlag = false;

  return(NO_ERR);
};

  //################################
  // Setup
  // - Defines an initial value for
  //   PastZ variable
  //################################

uint8_t PIX::setup()
{
  m_fPastZ = 8500;//Falta calcular
  m_fNewZ = 8500;
  //m_bRunFlag = false;
  m_bRunFlag = false;
  return(NO_ERR);
};

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
    return(NO_ERR);
};

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
    *l_stNewMessage = m_stMssg;
    m_bMssgFlag = false;
    return(NO_ERR);
}
