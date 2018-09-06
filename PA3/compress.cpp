/*
 * Filename: compress.cpp
 * Description: the program test the methods implemented in HCTree class by
 * 		compressing the files and write to the output file
 * Author: Chenglin Liu, xiaojie Li
 * Assignment 3
 * Date: 11/05/2017
 */ 

#include <iostream>
#include "HCTree.h"
#include <bitset>
using namespace std;


int main(int argc, char *argv[]) {
  vector<unsigned int> freqs(SIZE, 0);
  int nextChar;
  bool empty_file = false;
	
  /* incorrect number of arguments in command line */
  if (argc != REQ_ARG_C) {
    cerr << "The program requires two file names.\n";
    return EXIT_FAILURE;
  }  

  ifstream ifs;
  ifs.open(argv[FIRST_ARG]);
  
  /* fail opening file */
  if (!ifs.is_open()) {
    cerr << "The first file entered cannot be open.\n";
    return EXIT_FAILURE;
  }
  /* file is empty */
  if ((nextChar = ifs.get()) == EOF)
    empty_file = true;
  
  unsigned int count = 0;
  /* count the frequence of character in the file and store it in 
   * freqs vector */
  while (nextChar != EOF) { 
    freqs[nextChar]++;
    if (freqs[nextChar] == 1)
      count++;
    nextChar = ifs.get();
  }
  
  ifs.close();
  

  ofstream ofs;
  ofs.open(argv[SEC_ARG]);
  /* fail opening second file */
  if (!ofs.is_open()) {
    cerr << "The second file cannot be opened for writing.\n";
    return EXIT_FAILURE;
  }
  /* compressing the empty file, do nothing */
  if (empty_file) {
    ofs.close();
    return 0;
  }


  HCTree myTree;
  /* building the Huffman tree */
  myTree.build(freqs);

  /* writing the header */
  ofs.put((byte)(count - 1));
  
  BitOutputStream bos(ofs);
  myTree.encode_Header(bos);
 
  ifs.open(argv[FIRST_ARG]);
  /* read the message again for encoding */
  while ((nextChar = ifs.get()) != EOF) 
    myTree.encode((byte)nextChar, bos);
  /* flush whatever is left in the buffer */
  bos.flush();

  ifs.close();
  ofs.close();
  
  return 0;
}
