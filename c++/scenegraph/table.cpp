#include "table.h"
#include "error.h"

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <OpenGL/gl3.h>
#endif

TablePtr Table::Make()
{
  return TablePtr(new Table());
}

Table::Table()
{
    // Initialize the necessary components for the table
}

Table::~Table()
{
    // Perform any necessary cleanup here
}

void Table::Draw(StatePtr st)
{
    // Implement the drawing logic for the table using cubes
    // You can utilize the cubes vector and draw each cube here
    // Make sure to set up the necessary OpenGL configurations for drawing the table
}
