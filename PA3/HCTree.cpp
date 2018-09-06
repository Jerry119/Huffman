/*
 * Filename: HCTree.cpp
 * Description: implementations of the methods in HCTree class
 * Author: Chenglin Liu
 * Assignment 3
 * Date: 11/05/2017
 */ 

#include "HCTree.h"
#include <stack>
#include <string>

/* destructor */
HCTree::~HCTree() {
  deleteAllNode(root);
}

/* helper function for the destructor to free up the memory */
void HCTree::deleteAllNode(HCNode *node) {
  if (node->c0)
    deleteAllNode(node->c0);
  if (node->c1)
    deleteAllNode(node->c1);
  delete node;
}

/*
 * Function name: build
 * Description: build the Huffman tree based on the freqs vector passed in,
 * 		using priority_queue
 * Parameter: reference to a vector containg frequency
 * Return: None
 */
void HCTree::build(const vector<unsigned int>& freqs){
  /* create a min heap */
  priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> minheap;
  /* store the nonzero frequences in min heap */
  for (int i = 0; i < freqs.size(); i++) {
    if (freqs[i] != 0) {
      leaves[i] = new HCNode(freqs[i], (byte)i);
      minheap.push(leaves[i]);
    }
  }
    
  HCNode *leastNode;
  /* building Huffman tree */
  while (minheap.size() != 1) {
    HCNode *newNode;
    /* get the smallest node in the heap */
    leastNode = minheap.top();
    /* take out the smallest node */
    minheap.pop();
    /* create a new node */
    newNode = new HCNode(leastNode->count, leastNode->symbol);
    newNode->c0 = leastNode;
    leastNode->p = newNode;

    /* get the second least node */
    leastNode = minheap.top();
    minheap.pop();
    /* update the new node */
    newNode->count += leastNode->count;
    newNode->c1 = leastNode;
    leastNode->p = newNode;
    /* insert new node to the heap */
    minheap.push(newNode);
  } 
  /* set the root */
  root = minheap.top();
}

/*
 * Function name: encode()
 * Description: encode the symbol passed in, and use BitOutputStream to ouput
 * 		bits using std::stack
 * Parameter: symbol to be encoded
 * 	      reference to the BitOutputStream
 * Return: none
 */
void HCTree::encode(byte symbol, BitOutputStream &out) const {
  stack<int> myStack;
  HCNode *node = leaves[(int)symbol];
  byte tmp;
  /* if leaf node doesn't have parent, meaning it's the only node */ 
  if (!node->p) {
    out.writeBit(0);
    return;
  }

  /* store the encoding to the stack, traverse from bottom to root */
  while (node->p) {
    tmp = node->symbol;
    node = node->p;
    (node->symbol == tmp) ? myStack.push(0) : myStack.push(1);
  }
  /* get the encoded message from the stack and output using BitOutputStream */
  while (!myStack.empty()) {
    out.writeBit(myStack.top());
    myStack.pop();
  }
}


/*
 * Function name: encode()
 * Description: encode the symbol passed in, and use ofstream to ouput
 * 		bytes using std::stack
 * Parameter: symbol to be encoded
 * 	      reference to the ofstream object
 * Return: none
 */
void HCTree::encode(byte symbol, ofstream& out) const {
  stack<int> myStack;
  HCNode *node = leaves[(int)symbol];
  byte tmp;
  /* only one node in the tree */
  if (!node->p) {
    out << 0;
    return;
  }
  /* store the encoding to the stack, traverse from bottom to root */
  while (node->p) {
    tmp = node->symbol;
    node = node->p;
    (node->symbol == tmp) ? myStack.push(0) : myStack.push(1);
  }
      
  /* writing to the given ofstream */
  while (!myStack.empty()){
    out << myStack.top();
    myStack.pop();
  }
}

/*
 * Description: using the pre-order traversal to traversal the tree,
 * 		if it's a leaf node, write bit 0 to file, otherwise,
 * 		write bit 1
 */ 
void HCTree::pre_traversal(BitOutputStream &out, HCNode *node) const {
  if (!node->c0 && !node->c1) {
    out.writeBit(0);
    bitset<BYTE> leaf((int)node->symbol);
    for (int i = 0; i < BYTE; i++)
      out.writeBit(leaf[i]);
  }
  else {
    out.writeBit(1);
    if (node->c0)
      pre_traversal(out, node->c0);
    if (node->c1)
      pre_traversal(out, node->c1);
  }
    
}
/*
 * description: encoding the file header, using the pre_traversal function  
 **/
void HCTree::encode_Header(BitOutputStream &out) const{
  bitset<FOUR_BYTE> total_char(root->count);
  for (int i = 0; i < FOUR_BYTE; i++)
    out.writeBit(total_char[i]);
  pre_traversal(out, root);    
}

/*
 * Function name: decode()
 * Description: read from the input bit by bit using BitInputStream, and return
 * 		the decoded symbol
 * Parameter: reference to the BitInputStream
 * Return: the decoded symbol
 */
int HCTree::decode(BitInputStream &in) const {
  int b;
  HCNode *curr = root;
  /* reading from the input bit by bit */
  while ((b = in.readBit()) != EOF && root->count > 0) {
    /* case when only one unqiue character in the message */
    if (!root->c0 && !root->c1) {
      root->count--;
      return (int)root->symbol;
    } 
    /* traverse down the tree */
    curr = (b == 0) ? curr->c0 : curr->c1;
    /* leaf node */
    if (!curr->c0 && !curr->c1){
      b = (int)curr->symbol;
      root->count--;
      return b;
    }
  }

  return EOF;
}

/*
 * Function name: decode()
 * Description: read from the input one byte at a time using ifstream, 
 * and return the decoded symbol
 * Parameter: reference to the ifstream
 * Return: the decoded symbol
 */
int HCTree::decode(ifstream &in) const {
  int b;
  HCNode *curr = root;
  
  while ((b = in.get()) != EOF) {
    /* if only one node in the huffman tree */
    if (!root->c0 && !root->c1) {
      return root->symbol;
    }
    curr = (b == '0') ? curr->c0 : curr->c1;
    /* curr is a leaf */
    if (!curr->c0 && !curr->c1){
      b = (int)curr->symbol;
      break;
    }
  }
 
  return b;
}

/*
 * Description: rebuilding the tree, if see a 0, it means it's a leaf node,
 * 		then create a leaf node, if it's 1, assign it's c0 and c1
 */ 
HCNode* HCTree::rebuildTree(BitInputStream &in, int &index) {
  /* leaf node */
  if (in.readBit() == 0) {
    index++;
    /* get the symbol */
    bitset<BYTE> nextChar;
    for (int i = 0; i < BYTE; i++)
      nextChar.set(i, in.readBit());
    byte symbol = (byte)nextChar.to_ulong();
    leaves[(int)symbol] = new HCNode(0, symbol);
    return leaves[(int)symbol];
  }
  else {
    HCNode *c0 = rebuildTree(in, index);
    HCNode *c1 = rebuildTree(in, index);
    return new HCNode(0, 0, c0, c1);
  }
}
/*
 * description: decoding the file header using rebuildTree function
 */ 
void HCTree::decode_Header(BitInputStream &in, const int &count,
		const int &total_char) {
  int index = 0;
  while (index != count)
    root = rebuildTree(in, index);
  root->count = total_char;
}
