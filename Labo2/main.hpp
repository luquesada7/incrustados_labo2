/*
 * main.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_
#define TIMER32_COUNT 0x00000BB8  // ~1ms with 3MHz clock

#define NUMBER_OF_SLOTS 255
#define NULL            0

/* Variables globales del programa */
extern st_Node *g_pLinkedList;
extern uint8_t g_aSendMessageFlag[255];
extern uint8_t g_aExecuteNextFrame[255];
extern Task * g_aTaskPointers[255];

// The main Setup function for the application
void Setup(void);

#endif /* MAIN_HPP_ */
