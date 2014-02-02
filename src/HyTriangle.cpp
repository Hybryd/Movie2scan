#include "HyTriangle.hpp"


HyTriangle::HyTriangle()
{
}

HyTriangle::HyTriangle(HyNode* a, HyNode* b, HyNode* c)
{
  if(a!=NULL)
    aNodes.push_back(a);
  else
    std::cerr << "ERROR [HyTriangle::HyTriangle]: first argument is NULL" << std::endl;

  if(b!=NULL)
    aNodes.push_back(b);
  else
    std::cerr << "ERROR [HyTriangle::HyTriangle]: second argument is NULL" << std::endl;
  
  if(c!=NULL)
    aNodes.push_back(c);
  else
    std::cerr << "ERROR [HyTriangle::HyTriangle]: third argument is NULL" << std::endl;
}



HyTriangle::~HyTriangle()
{
}

/*
  Return if n is one of the node of the HyTriangle
*/

bool HyTriangle::has(HyNode * n)
{
  bool res=false;
  if(n!=NULL)
  {
    int i=0;
    while(i<aNodes.size() && !res)
    {
      res |= (aNodes[i] == n);
      ++i;
    }
  }
  else
  {
    std::cerr << "WARNING in HyTriangle::has : argument is NULL" << std::endl;
  }
  return res;
}


std::ostream &operator<<( std::ostream &out, HyTriangle & t )
{
  for(int i=0;i<t.nb();++i)
  {
    out << (*t.nodes()[i]) << std::endl;
  }
  out << (*t.nodes()[0]) << std::endl;
  return out;
}



