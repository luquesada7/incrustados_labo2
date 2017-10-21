/*
 * main.hpp
 *
 *  Created on: Aug 31, 2016
 *      Author: eortiz
 */

#ifndef MAIN_HPP_
#define MAIN_HPP_
#define TIMER32_COUNT 0x0000BB80  // ~1ms with 3MHz clock
#define NUMBER_OF_SLOTS 255


// The main Setup function for the application

//extern Task *g_aTaskPointers[NUMBER_OF_SLOTS];

void Setup(void);

#endif /* MAIN_HPP_ */
