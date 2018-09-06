/*
 * Filename: BitInputStream.cpp
 * Description: implementations of the method in BitInputStream class
 * Author: Chenglin Liu
 * Date: 11/05/2017
 * Assignment 3
 */ 

#include "BitInputStream.h"

/* fill the bitwise buffer by reading one byte from the input stream */
void BitInputStream::fill() {
  buffer = in.get();
  nbits = 0;
}

/* read one bit from the buffer */
unsigned int BitInputStream::readBit() {
  /* all 8 bits in the buffer have been read */
  if (nbits == 8)
    fill();
  if (in.eof())
    return EOF;
  unsigned int nextBit = (buffer & (1 << nbits)) >> nbits;
  nbits++;
  return nextBit;
}
