#ifndef HYNODE_HPP
#define HYNODE_HPP

#include <iostream>
#include <vector>

/*

  HyNode : implements a node object

*/

class HyNode
{
protected:
  double x;
  double y;
  double z;
  std::vector< HyNode * > aNeig;


public:
  HyNode();
  HyNode(double xx, double yy, double zz);
  
  double X() {return x;}
  double Y() {return y;}
  double Z() {return z;}
  
  int deg() {return aNeig.size();}
  HyNode * neig(int i);
  void addNeig(HyNode * n);
  void delNeig(HyNode * n);
  
  double squaredDistanceTo(HyNode * n) {return (x-n->X())*(x-n->X()) + (y-n->Y())*(y-n->Y()) + (z-n->Z())*(z-n->Z()) ; }
};

std::ostream &operator<<( std::ostream &out, HyNode & n );


#endif
