/*
 * main.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_
#define TIMER32_COUNT 0x00000BB8  // ~1ms with 3MHz clock

/* Variables globales del programa */
extern st_Node *g_pLinkedList;
extern int g_aSendMessageFlag[255];
extern int g_aExecuteNextFrame[255];

// The main Setup function for the application
void Setup(void);

#endif /* MAIN_HPP_ */
