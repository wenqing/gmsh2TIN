/*!
   A class for nodes

  \author Wenqing Wang
*/
#ifndef Node_H
#define Node_H

#include <memory>
#include <cstddef>
class Node
{
   public:

      Node(const size_t index, const double x[]):
         _index(index), _coordinate(std::unique_ptr<double[]> (new double[3]))
      {
         _coordinate[0] = x[0];
         _coordinate[1] = x[1];
         _coordinate[2] = x[2];
      }

      ~Node()
      {
      }

      void setID(const std::size_t id)
      {
         _index = id;
      }

      std::size_t getID() const
      {
         return _index;
      }

      /// Get coordiantes
      double *getCoordinates() const
      {
         return _coordinate.get();
      }

      void setMark(const bool mark)
      {
         _mark = mark;
      }

      bool getMark() const
      {
         return _mark;
      }

   private:
      std::size_t _index;       ///< Index
      std::unique_ptr<double[]> _coordinate; ///< Coodinate of the node
      bool _mark;       ///< status
};

#endif
