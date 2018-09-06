/*
 * Filename: BitOutputStream.cpp
 * Description: implementations of the methods in BitOutputStream class
 * Author: Chenglin Liu
 * Date: 11/05/2017
 * Assignment 3
 */ 

#include "BitOutputStream.h"

/* send the buffer to the ouput stream, and clear buffer */
void BitOutputStream::flush() {
  out.put(buffer);
  out.flush();
  buffer = 0;
  nbits = 0;
}
/* write bits to the buffer */
void BitOutputStream::writeBit(unsigned int bit) {
  /* flush the bitwise buffer if it's full */
  if (nbits == 8)
    flush();
  /* writing to the buffer */
  buffer = (bit == 0) ? (buffer & (255 ^ (1 << nbits))) : 
		(buffer | (1 << nbits));
  
  nbits++;
}
