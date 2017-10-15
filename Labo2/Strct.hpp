/*
 * Strct.hpp
 *
 *  Created on: Oct 13, 2017
 *      Authors: Luisa Fernanda Quesada &
 *               Jose Pablo Martinez
 */

#ifndef STRCT_HPP_
#define STRCT_HPP_
#define __NOP __nop

struct st_Message{
    char *std_pDestKey; // - Key of Destination of message
    uint16_t std_u16IntData ; // - Mensaje de tipo integer
    float std_fFloatData; // - Mensaje de tipo float
    bool std_bBoolData; // - Mensaje de tipo bool
};

struct st_Node {
    st_Message std_stMssg;
    st_Node *std_pnext;
};

#endif /* STRCT_HPP_ */
