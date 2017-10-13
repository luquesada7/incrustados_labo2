/*
 * Strct.hpp
 *
 *  Created on: Oct 13, 2017
 *      Authors: Jose Pablo Martinez &
                 Luisa Fernanda Quesada
 */

#ifndef STRCT_HPP_
#define STRCT_HPP_
#define __NOP __nop

struct st_Message{
    uint8_t std_u8DestTaskID; // - Task ID de la tarea destino
    uint16_t std_u16IntData ; // - Mensaje de tipo integer
    float std_fFloatData; // - Mensaje de tipo float
};

struct st_Node{
    st_Message std_stMssg;
    st_Node *std_pnext;
};

#endif /* STRCT_HPP_ */
