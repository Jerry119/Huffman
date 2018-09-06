/*
 * Filename: uncompress.cpp
 * Description: the program tests the methods implemented in HCTree class by
 *  		uncompressing the file and write it to the output file
 * Author: Chenglin Liu, Xiaojie Li
 * Date: 11/05/2017
 * Assignment 3
 */ 


#include <iostream>
#include <bitset>
#include "HCTree.h"

using namespace std;

int main(int argc, char *argv[]){
  vector<unsigned int> freqs(SIZE, 0);
  ifstream ifs;
  ofstream ofs;
  /* incorrect number of arguments in the command line */
  if (argc != REQ_ARG_C) {
    cerr << "The program requires 2 file names.\n";
    return EXIT_FAILURE;
  }
  
  ifs.open(argv[FIRST_ARG]);
  /* file can't be opened */
  if (!ifs.is_open()) {
    cerr << "The first file cannot be opened\n";
    return EXIT_FAILURE;
  }
  
  int count;
  unsigned int nextChar;
  /* uncompressing the empty file */
  if ((count = ifs.get()) == EOF) {
    ofs.open(argv[SEC_ARG]);
    if (!ofs.is_open()) {
      cerr << "Second file cannot be opened to output.\n";
      ifs.close();
      return EXIT_FAILURE;
    }
    ofs.close();
    ifs.close();
    return 0;
  }
    
  BitInputStream bis(ifs);
  count++;
  /* get the total number of character */ 
  bitset<FOUR_BYTE> total_char;
  for (int i = 0; i < FOUR_BYTE; i++) {
    unsigned int bit = bis.readBit();
    if (bit == EOF) {
      cerr << "EOF reached while reading the file header.\n";
      ifs.close();
      return EXIT_FAILURE;
    }
    total_char.set(i, bit);
  }
  
  HCTree myTree;
  /* reconstructing the tree */
  myTree.decode_Header(bis, count, total_char.to_ulong());
  
  ofs.open(argv[SEC_ARG]);
  /* fail opening file */
  if (!ofs.is_open()) {
    cerr << "The second file cannot be opened for output.\n";
    ifs.close();
    return EXIT_FAILURE;
  }
  
  /* decoding the message */
  while ((nextChar = myTree.decode(bis)) != EOF)
    ofs << (unsigned char)nextChar;
  
  ifs.close();
  ofs.close();
  return 0;
}

