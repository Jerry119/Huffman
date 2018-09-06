/*
 * Filename: BitOutputStream.h
 * Description: the program implements the Bit I/O to allow writing to the 
 * 		disk one bit at a time
 * Author: Chenglin Liu
 * Date: 11/05/2017
 * Assignment 3
 */ 

#ifndef BITOUTPUTSTREAM_H
#define BITOUTPUTSTREAM_H

#include <iostream>

using namespace std;

class BitOutputStream {
private:
  char buffer;     /* bitwise buffer to store the bits */
  int nbits;	   /* number of bits that have been written to the buffer */
  ostream &out;    /* to allow output to the file */

public:
  /* constructor */
  BitOutputStream(ostream &os) : out(os), buffer(0), nbits(0) {}

  /* send the bitwise buffer to the ouput stream, and clear the buffer 
   * Return: None */
  void flush();

  /* write bit to the bitwise buffer
   * return: None */
  void writeBit(unsigned int bit);

};

#endif
