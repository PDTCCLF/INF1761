#include <memory>
#include <vector>
#include "cube.h" // Assuming cube.h has been defined previously

class Table;
using TablePtr = std::shared_ptr<Table>;

#ifndef TABLE_H
#define TABLE_H

#include "shape.h"

class Table : public Shape {
  std::vector<CubePtr> cubes; // A vector to store the cubes
  unsigned int m_vao; // The VAO for the table

protected:
  Table(); // Protected constructor
  // You might want to add additional protected functions or members here

public:
  static TablePtr Make(); // Static method to create an instance of the Table
  virtual ~Table(); // Virtual destructor
  virtual void Draw(StatePtr st); // Function to draw the table

  // Additional public functions or members could be added here
};

#endif
