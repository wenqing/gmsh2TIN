/*!
  \file Element.h
  \brief Declair a class for triangular element

  \author Wenqing Wang
*/

#ifndef ELEMENT_H
#define ELEMENT_H

#include <memory>
#include <cstddef>

#include "Node.h"

class Element
{
   public:
      Element(const std::size_t id)
         :_index(id), _nodes(new Node*[3])
      {
      }

      ~Element()
      {
         delete _nodes;
         _nodes = nullptr;
      }

      void setNode(const int local_id, Node * const node)
      {
         _nodes[local_id] = node;
      }

      std::size_t getID() const
      {
         return _index;
      }

      Node** getNodes() const
      {
         return _nodes;
      }

   protected:
      std::size_t _index;       ///< Index

      /// Nodes of the element
      Node** _nodes;
};

#endif
