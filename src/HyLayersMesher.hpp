#ifndef HYLAYERSMESHER_HPP
#define HYLAYERSMESHER_HPP

#include <iostream>
#include <vector>

#include "/usr/include/opencv/cv_compatible.h"

#include "HyNode.hpp"
#include "HyTriangle.hpp"


class HyLayersMesher
{
protected:
  std::vector<HyTriangle> mesh;
  std::vector<HyNode *> lastLayer;
  

public:
  HyLayersMesher();
  
  std::vector<HyTriangle> & m() {return mesh;};
  long int size() {return mesh.size();}
  void addTriangle(HyNode * a, HyNode * b, HyNode * c);
  
  void addFirstLayer(std::vector<HyNode *> & layer);
  void addLayer(std::vector<HyNode *> & layer);
};

std::ostream &operator<<( std::ostream &out, HyLayersMesher & m );

#endif
