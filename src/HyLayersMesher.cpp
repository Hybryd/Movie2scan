#include "HyLayersMesher.hpp"


HyLayersMesher::HyLayersMesher() : mesh(0), lastLayer(0)
{
  
}



void HyLayersMesher::addTriangle(HyNode * a, HyNode * b, HyNode * c)
{
  mesh.push_back(HyTriangle(a,b,c));
}

void HyLayersMesher::addFirstLayer(std::vector<HyNode *> & layer)
{
  for(int i=0;i<layer.size()-1;++i)
  {
    layer[i]->addNeig(layer[i+1]);
    layer[i+1]->addNeig(layer[i]);
    lastLayer.push_back(layer[i]);
  }
  lastLayer.push_back(layer[layer.size()-1]);
}

//
// Layer is a vector of pointers of HyNodes.
// HyNodes are supposed to be ordered in Z axis.
//

void HyLayersMesher::addLayer(std::vector<HyNode *> & layer)
{

  // Triangulate the space between lastLayer and layer
  // VERY NAIVE ALGORITHM :
  // Bind lastLayer[i] with layer[i]
  // Choose between lastLayer[i+1] with layer[i+1] the most adapted (based on distance)
  // If it is lastLayer[i+1], add the two triangles (lastLayer[i],lastLayer[i+1],layer[i]) and (layer[i],layer[i+1],lastLayer[i+1]), and the contrary if it is layer[i+1]
  // Continue until reaching the end of one of the layers
  // Triangulate the last node of the layer with all the last nodes of the other layer
  
  HyNode * cur1   = lastLayer[0];
  HyNode * next1  = lastLayer[1];
  
  HyNode * cur2   = layer[0];
  HyNode * next2  = layer[1];
  
  int i=1;
  
  while(i<lastLayer.size() && i<layer.size())
  {
    cur1->addNeig(cur2);
    cur2->addNeig(cur1);
    
    cur2->addNeig(next2);
    next2->addNeig(cur2);
    
    next1->addNeig(next2);
    next2->addNeig(next1);
    
    if(cur1->squaredDistanceTo(next2) <= cur2->squaredDistanceTo(next1))
    {
      // Update neighborhood and add the triangles
      cur1->addNeig(next2);
      next2->addNeig(cur1);
    
      addTriangle(cur1,cur2,next2);
      addTriangle(cur1,next2,next1);
    }
    else
    {
      cur2->addNeig(next1);
      next1->addNeig(cur2);
    
      addTriangle(cur1,cur2,next1);
      addTriangle(cur2,next2,next1);
    }
    cur1=next1;
    next1=lastLayer[i+1];
    
    cur2=next2;
    next2=layer[i+1];
    
    ++i;
  }
  
  // Finally, copy layer in lastLayer
  lastLayer.clear();
  for(int i=0;i<layer.size();++i)
    lastLayer.push_back(layer[i]);

}

std::ostream &operator<<( std::ostream &out, HyLayersMesher & m )
{
  for(long int i=0;i<m.size();++i)
  {
    out << m.m()[i] << std::endl << std::endl;
  }
  
  return out;
}


