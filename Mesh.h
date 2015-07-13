/*!
   \file Mesh.h
   \brief class for triangular element mesh
   \author Wenqing Wang
 */
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

#include "Node.h"
#include "Element.h"

class Mesh
{
   public:
      Mesh() = default;
      ~Mesh()
      {
         for(auto node : _nodes)
         {
            delete node;
            node = nullptr;
         }
         for(auto elem : _elems)
         {
            delete elem;
            elem = nullptr;
         }
      }

      bool read(const std::string& in_file_name);

      bool write(const std::string& out_file_name) const;

      bool writeGmsh(const std::string &out_file_name) const;

   private:
      /// All nodes
      std::vector<Node*> _nodes;

      /// All elements
      std::vector<Element*> _elems;
};

#endif

