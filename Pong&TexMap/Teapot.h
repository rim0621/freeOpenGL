#pragma once
#include <GL/glew.h>

class Teapot
{
public:
  Teapot();
  ~Teapot();

  void draw(GLint loc_vertex, GLint loc_normal,GLint loc_texcoord);

private:
  GLint vertex_buffer;
  GLint normal_buffer;
  GLint index_buffer;

  GLint index_length;
};
