/*
 * PIXELS.cpp
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
#include <cmath>

PIXELS::PIXELS()
{
}

uint8_t PIXELS::run()
{
  l_fPixDelta = 1000; //Falta calcular
  l_iPixLine = 128;

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
  }

uint8_t LED::setup()
{
  m_fPastZ = 8000;//Falta calcular
}
