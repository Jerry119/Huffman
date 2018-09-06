/*
 * Filename: HCNode.cpp
 * Description: implementation of the method in HCNode class
 * Author: Chenglin Liu
 * Assignment 3
 * Date: 11/05/2017
 */ 

#include "HCNode.h"

/*
 * Function name: operator<
 * Parameter: reference to another HCNode
 * Description: used in priority_queue, smaller count have higher priority
 * 		if counts are equal, smaller symbol have higher priority
 * Return: true if lfs > rhs, false otherwise 
 */
bool HCNode::operator<(const HCNode &other) {
  return (count==other.count) ? (symbol>other.symbol) : (count>other.count);
}
