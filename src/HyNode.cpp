#include "HyNode.hpp"


HyNode::HyNode() : x(0), y(0), z(0)
{
}


HyNode::HyNode(double xx, double yy, double zz) : x(xx), y(yy), z(zz), aNeig(0)
{
}


HyNode * HyNode::neig(int i)
{
  if(i<aNeig.size())
    return aNeig[i];
  else
    std::cerr << "WARNING [HyNode::neig]: index out of range" << std::endl;
}

void HyNode::addNeig(HyNode * n)
{
  if(n != NULL)
  {
    // First check if n is not in aNeig
    double res=true;
    int i=0;
    while(i<aNeig.size() && res)
    {
      res = (n != aNeig[i]);
      ++i;
    }
    if(res)
    {
      aNeig.push_back(n);
    }
  }
  else
  {
    std::cerr << "WARNING [HyNode::addNeig]: try to add NULL neighbor" << std::endl;
  }
}



void HyNode::delNeig(HyNode * n)
{
  if(n != NULL)
  {
    bool res=true;
    int i=0;
    while(i<aNeig.size() && res)
    {
      if(n==aNeig[i])
      {
        aNeig.erase(aNeig.begin()+i);
        res=false;
      }
      ++i;
    }
    if(i==aNeig.size())
    {
      std::cerr << "WARNING [HyNode::delNeig]: neighbor not found" << std::endl;
    }
  }
  else
  {
    std::cerr << "WARNING [HyNode::delNeig]: try to delete NULL neighbor" << std::endl;
  }
}





std::ostream &operator<<( std::ostream &out, HyNode & n )
{
  out << n.X() << " " << n.Y() << " " << n.Z();
//  for(int i=0; i<n.deg(); ++i)
//  {
//    out << " " << n.neig(i)->X() << " " << n.neig(i)->Y() << " " << n.neig(i)->Z() << std::endl;
//  }
  return out;
}


