#ifndef HYTRIANGLE_HPP
#define HYTRIANGLE_HPP

#include <cstdlib>
#include <iostream>
#include <vector>

#include "HyNode.hpp"


/*

  HyTriangle : implements a triangle object

*/

class HyTriangle
{
protected:
  std::vector< HyNode *> aNodes;
  

public:
  HyTriangle();
  HyTriangle(HyNode* a, HyNode* b, HyNode* c);
  ~HyTriangle();
  
  int nb() {return aNodes.size();} // must be 3
  std::vector< HyNode *> & nodes() {return aNodes;}
  bool has(HyNode * n);
};

std::ostream &operator<<( std::ostream &out, HyTriangle &t );


#endif
