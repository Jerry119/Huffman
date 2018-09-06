/*
 * Filename: BitInputStream.h
 * Description: the program is used for Bit I/O to allow reading from file
 * 		one bit at a time
 * Author: Chenglin Liu
 * Date: 11/05/2017
 * Assignment 3
 */

#ifndef BITINPUTSTREAM_H
#define BITINPUTSTREAM_H

#include <iostream>

using namespace std;

class BitInputStream {
private:
  istream &in;    /* use istream to read in one byte */ 
  char buffer;    /* store the read-in byte */
  int nbits;      /* number of bits that have been read from the buffer */

public:
  /* constructor */
  BitInputStream(istream &is) : in(is), buffer(0), nbits(8) {}
  /*
   * fill the buffer by reading one byte from the input stream
   * Return: None
   */  
  void fill();

  /*
   * read one bit at a time from the buffer
   * Return: the bit we just read
   */  
  unsigned int readBit();
};

#endif
