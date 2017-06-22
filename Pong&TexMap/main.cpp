// Defines the entry point for the console application.
//
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "Teapot.h"
#include "Shader.h"

#include "transform.hpp"
#include "SOIL.h"

void init();
void display();
void reshape(int, int);
void idle();

GLuint    program;

GLint     loc_a_vertex;
GLint     loc_a_normal;

GLint     loc_a_texcoord;
GLuint    texid;
GLint     loc_u_texid;

GLint     loc_u_pvm_matrix;
GLint     loc_u_vm_matrix;
GLint     loc_u_normal_matrix;

GLint     loc_u_light_vector_vc;

GLint     loc_u_light_ambient;
GLint     loc_u_light_diffuse;
GLint     loc_u_light_specular;

GLint     loc_u_material_ambient;
GLint     loc_u_material_diffuse;
GLint     loc_u_material_specular;
GLint     loc_u_material_shininess;

float model_scale = 5.0f;
float model_angle = 0.0f;

kmuvcl::math::mat4x4f mat_PVM, mat_VM;
kmuvcl::math::mat3x3f mat_Normal;

kmuvcl::math::vec4f light_vector       = kmuvcl::math::vec4f(10.0f, 10.0f, 10.0f, 0.0f);
kmuvcl::math::vec4f light_ambient      = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
kmuvcl::math::vec4f light_diffuse      = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
kmuvcl::math::vec4f light_specular     = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);

kmuvcl::math::vec4f material_ambient   = kmuvcl::math::vec4f(0.464f, 0.393f, 0.094f, 1.0f);
kmuvcl::math::vec4f material_diffuse   = kmuvcl::math::vec4f(0.464f, 0.393f, 0.094f, 1.0f);
kmuvcl::math::vec4f material_specular  = kmuvcl::math::vec4f(1.0f, 1.0f, 1.0f, 1.0f);
float               material_shininess = 50.0f;

Teapot* tespot;

std::chrono::time_point<std::chrono::system_clock> prev, curr;

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 640);
  glutCreateWindow("Modeling & Navigating Your Studio");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  if (glewInit() != GLEW_OK)
  {
    std::cerr << "failed to initialize glew" << std::endl;
    return -1;
  }

  init();

  glutMainLoop();

  return 0;
}

void init()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glFrontFace(GL_CCW);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // for wireframe rendering

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);

  program = Shader::create_program("./shader/phong_vert.glsl", "./shader/phong_frag.glsl");

  loc_u_pvm_matrix         = glGetUniformLocation(program, "u_pvm_matrix");
  loc_u_vm_matrix          = glGetUniformLocation(program, "u_vm_matrix");
  loc_u_normal_matrix      = glGetUniformLocation(program, "u_normal_matrix");

  loc_u_light_vector_vc    = glGetUniformLocation(program, "u_light_vector_vc");
  loc_u_light_ambient      = glGetUniformLocation(program, "u_light_ambient");
  loc_u_light_diffuse      = glGetUniformLocation(program, "u_light_diffuse");
  loc_u_light_specular     = glGetUniformLocation(program, "u_light_specular");

  loc_u_material_ambient   = glGetUniformLocation(program, "u_material_ambient");
  loc_u_material_diffuse   = glGetUniformLocation(program, "u_material_diffuse");
  loc_u_material_specular  = glGetUniformLocation(program, "u_material_specular");
  loc_u_material_shininess = glGetUniformLocation(program, "u_material_shininess");

  loc_a_vertex             = glGetAttribLocation(program, "a_vertex");
  loc_a_normal             = glGetAttribLocation(program, "a_normal");

  loc_u_texid          =glGetAttribLocation(program,"u_texid");
  loc_a_texcoord=glGetAttribLocation(program,"a_texcoord");

  int width, height, channels;

  unsigned char* image = SOIL_load_image("frame.jpg",
     &width, &height, &channels, SOIL_LOAD_RGB);

  glGenTextures(1,&texid);
  glBindTexture(GL_TEXTURE_2D,texid);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);


 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//클떄
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);


  SOIL_free_image_data(image);


  tespot = new Teapot();

  prev = curr = std::chrono::system_clock::now();
}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  // Camera setting
  kmuvcl::math::mat4x4f mat_Proj, mat_View, mat_Model;

  // camera intrinsic param
  mat_Proj = kmuvcl::math::perspective(60.0f, 1.0f, 0.001f, 10000.0f);

  // camera extrinsic param
  mat_View = kmuvcl::math::lookAt(
    0.0f, 0.0f, 3.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f);

  mat_Model = kmuvcl::math::scale(model_scale, model_scale, model_scale);
  mat_Model = kmuvcl::math::rotate(model_angle*0.7f, 0.0f, 0.0f, 1.0f) * mat_Model;
  mat_Model = kmuvcl::math::rotate(model_angle,      0.0f, 1.0f, 0.0f) * mat_Model;
  mat_Model = kmuvcl::math::rotate(model_angle*0.5f, 1.0f, 0.0f, 0.0f) * mat_Model;

  mat_VM = mat_View*mat_Model;

  mat_PVM = mat_Proj*mat_View*mat_Model;

  mat_Normal(0, 0) = mat_VM(0, 0);
  mat_Normal(0, 1) = mat_VM(0, 1);
  mat_Normal(0, 2) = mat_VM(0, 2);
  mat_Normal(1, 0) = mat_VM(1, 0);
  mat_Normal(1, 1) = mat_VM(1, 1);
  mat_Normal(1, 2) = mat_VM(1, 2);
  mat_Normal(2, 0) = mat_VM(2, 0);
  mat_Normal(2, 1) = mat_VM(2, 1);
  mat_Normal(2, 2) = mat_VM(2, 2);

glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D,texid);
glUniform1i(loc_u_texid,0);

  kmuvcl::math::vec4f light_vector_vc;
  light_vector_vc = mat_View * light_vector;

  glUniformMatrix4fv(loc_u_pvm_matrix,    1, false, mat_PVM);
  glUniformMatrix4fv(loc_u_vm_matrix,     1, false, mat_VM);
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, mat_Normal);


  glUniform3fv(loc_u_light_vector_vc,   1, light_vector_vc);
  glUniform4fv(loc_u_light_ambient,     1, light_ambient);
  glUniform4fv(loc_u_light_diffuse,     1, light_diffuse);
  glUniform4fv(loc_u_light_specular,    1, light_specular);

  glUniform4fv(loc_u_material_ambient,  1, material_ambient);
  glUniform4fv(loc_u_material_diffuse,  1, material_diffuse);
  glUniform4fv(loc_u_material_specular, 1, material_specular);
  glUniform1f(loc_u_material_shininess, material_shininess);

  // Draw teapot
  tespot->draw(loc_a_vertex, loc_a_normal,loc_a_texcoord);





  glUseProgram(0);
  Shader::check_gl_error("draw");

  glutSwapBuffers();
}

void reshape(int width, int height)
{
  glViewport(0, 0, width, height);
}

void idle()
{
  curr = std::chrono::system_clock::now();

  std::chrono::duration<float> elaped_seconds = (curr - prev);

  model_angle += 30 * elaped_seconds.count();

  prev = curr;

  glutPostRedisplay();
}
