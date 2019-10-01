/*!
   \file Mesh.cpp
   \brief definitions of the members of class Mesh
   \author Wenqing Wang
 */

#include "Mesh.h"

#include <algorithm>
#include <limits>
#include <ctime>
#include <cfloat>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool Mesh::read(const std::string& in_file_name)
{
   ifstream ins(in_file_name);
   if ( !ins.good() )
   {
      cout << "Could not open file " << in_file_name;
      return false;
   }

   string str_buff;
   // Read word "$MeshFormat"
   ins >> str_buff >> ws;

   // Read version number
   string ver_num;
   int file_format, data_size;
   ins >> ver_num >> file_format >> data_size;

   ins >> ws;
   // Read word "$MeshFormat"
   ins >> str_buff >> ws;

   // Read word "$Nodes"
   ins >> str_buff >> ws;

   //--------------------------------------------------
   // Read nodes
   size_t nn;
   ins >> nn >> ws;

   _nodes.resize(nn);

   size_t index0 = 0;
   size_t index = 0;
   vector<size_t> id_gap;
   vector<size_t> node_has_gap;
   double x[3];

   for (size_t i=0; i<nn; i++)
   {
      ins >> index;
      ins >> x[0] >> x[1] >> x[2] >> ws;

      _nodes[i] = new Node(index, x);
      if ((index-index0)>=2)
      {
         id_gap.push_back(index-index0-1);
         node_has_gap.push_back(index);
      }
      index0 = index;
   }

   // Read word "$EndNodes"
   ins >>  str_buff >> ws;

   //--------------------------------------------------
   // Read element
   size_t ne;
   // Read word "$Elements"
   ins >>  str_buff >> ws;

   // Read number of elements
   ins >> ne >> ws;
   _elems.resize(ne);

   int et = 0; // element type
   unsigned ipart = 0; // partidion index
   unsigned mat_id = 0; //materal id;
   int ntags = 0; // number of tags;
   int geo_id = 0;  // elementary goemetrical entity.
   int int_buffer;

   for (size_t i=0; i<ne; i++)
   {

      ins >> index >> et;

      if(et != 2 && et != 3)
      {
         cout << "Only triangle or quadrilateral element is allowed. " << std::endl;
         return false;
      }

      const int nnodes = (et == 2) ? 3 : 4;
      Element *elem = new Element(i, nnodes);
      _elems[i] = elem;

      ins >> ntags >> mat_id >> geo_id;
      if (ntags >= 3)
      {
         ins >> ipart;
         for (int k=0; k<ntags-3; k++)
            ins >> int_buffer;
      }
      else
         ipart = 0;

      //
      for (int j=0; j<nnodes; j++)
      {
         size_t node_id;
         ins >> node_id;

         // Renumer in order to have continuous numbers
         for (int k=static_cast<int>(node_has_gap.size()-1); k>=0; k--)
         {
            if (node_id >= node_has_gap[k])
               node_id -= id_gap[k];
         }

         elem->setNode(j, _nodes[node_id-1]);
      }
   }


   // Check unused nodes
   size_t counter, size0;
   size0 = _nodes.size();

   for (size_t i=0; i<size0; i++)
      _nodes[i]->setMark(false);

   for (size_t i=0; i<_elems.size(); i++)
   {
      Element *elem = _elems[i];
      Node **e_nodes = elem->getNodes();

      for (int k=0; k<elem->getNodeNumber(); k++)
      {
         e_nodes[k]->setMark(true);
      }
   }

   // Remove unused nodes
   counter = 0;
   _nodes.erase(std::remove_if (_nodes.begin(), _nodes.end(),
                                [&counter](Node *node) -> bool //lambda function
   {
      bool inactive = false;
      if ( node->getMark() == false )
      {
         delete node;
         inactive = true;
      }
      else
      {
         node->setID(counter);
         counter++;
      }
      return inactive;
   }
                               ),  _nodes.end() );

   ins.close();

   return true;
}

bool Mesh::write(const std::string& out_file_name) const
{
   ofstream os( out_file_name, ios::trunc );
   if ( !os.good() )
   {
      std::cout << "Could not open file " << out_file_name <<std::endl;
      return false;
   }

   int counter = 0;
   for (auto elem : _elems)
   {
      Node **e_nodes = elem->getNodes();

      os << counter++;
      
      const int nnodes = elem->getNodeNumber();
      for (int k=0; k<3; k++)
      {
         double* x = e_nodes[k]->getCoordinates();
         os << " " << x[0]
            << " " << x[1]
            << " " << x[2];
      }
      os << "\n";

      if (nnodes == 4)
      {
         os << counter++;
         double* x = e_nodes[2]->getCoordinates();
         os << " " << x[0]
            << " " << x[1]
            << " " << x[2];
         x = e_nodes[3]->getCoordinates();
         os << " " << x[0]
            << " " << x[1]
            << " " << x[2];
         x = e_nodes[0]->getCoordinates();
         os << " " << x[0]
            << " " << x[1]
            << " " << x[2];
      }
      os << "\n";
   }
   os.flush();

   return true;
}

bool Mesh::writeGmsh(const std::string &out_file_name) const
{
   ofstream os( out_file_name, ios::trunc );
   if ( !os.good() )
   {
      std::cout << "Could not open file " << out_file_name <<std::endl;
      return false;
   }

   setw(14);
   os.precision(12);
   os.setf(ios::scientific);

   os<<"$MeshFormat\n2 0 8\n$EndMeshFormat\n$Nodes"<<endl;

   os<< _nodes.size() <<endl;
   for (auto node : _nodes)
   {
      double* x = node->getCoordinates();
      os << node->getID()+1 << " "
         << x[0] << " "
         << x[1] << " "
         << x[2] << std::endl;
   }
   os<<"$EndNodes\n$Elements"<<endl;

   os<< _elems.size() <<endl;
   for (auto elem : _elems)
   {
      const char deli = ' ';

      const int ele_type = (elem->getNodeNumber()==3) ? 2 : 3;
      os<<elem->getID() + 1 << deli
        <<ele_type << deli << "3" << deli
        <<" 1 " << deli << " 1 " << deli
        << "1 " << deli;

      Node** e_nodes = elem->getNodes();
      for (int i=0; i<elem->getNodeNumber(); i++)
      {
         os << e_nodes[i]->getID() + 1 << deli;
      }
      os<<"\n";
   }

   os.flush();

   os << "$EndElements" << endl;
   os.close();

   return true;
}


