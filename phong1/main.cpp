// Defines the entry point for the console application.
//
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <fstream>

#include "Object.h"
#include "Camera.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>


void init();
void display();
void reshape(int, int);
void idle();
void keyboard(unsigned char, int, int);
void special(int, int, int);

GLuint		program;

GLint	loc_a_vertex;
GLint     loc_a_normal;

GLint	loc_u_pvm_matrix;
GLint     loc_u_normal_matrix;
GLint     loc_u_vm_matrix;

GLint     loc_u_light_vector_vc;

GLint     loc_u_light_ambient;
GLint     loc_u_light_diffuse;
GLint     loc_u_light_specular;

GLint     loc_u_material_ambient;
GLint     loc_u_material_diffuse;
GLint     loc_u_material_specular;
GLint     loc_u_material_shininess;


glm::mat4 mat_PVM;
glm::mat4 mat_VM;
glm::mat3 mat_Normal;

std::string g_filename;
Object		  g_model;      // object
Camera		  g_camera;			// viewer (you)


glm::vec4 light_vector       = glm::vec4(10.0f, 10.0f, 10.0f, 0.0f);
glm::vec4 light_ambient      = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 light_diffuse      = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 light_specular     = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);


int main(int argc, char* argv[])
{
  if (argc > 1)
  {
    g_filename = argv[1];
  }
  else
  {
    g_filename = "./data/cube.obj";
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 640);
  glutCreateWindow("Modeling & Navigating Your Studio");

  glutDisplayFunc(display);
	glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
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
  g_model.load_simple_obj(g_filename);
  g_model.print();

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // for filled polygon rendering

  // TODO: 1. Implement vertex/fragment shader codes for Phong shading model
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);

  program = Shader::create_program("./shader/simple.vert", "./shader/simple.frag");

  loc_u_pvm_matrix         = glGetUniformLocation(program, "u_pvm_matrix");
  loc_u_normal_matrix      = glGetUniformLocation(program, "u_normal_matrix");
  loc_u_vm_matrix          = glGetUniformLocation(program, "u_vm_matrix");

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
  // TODO: 2. Get locations of shader variables in shader program

}

void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	// Camera setting
	glm::mat4   mat_Proj, mat_View, mat_Model;


	// camera extrinsic param
	mat_View = glm::lookAt(
		g_camera.position(),				// eye position
		g_camera.center_position(), // center position
		g_camera.up_direction()			// up direction
		);

	// camera intrinsic param
	mat_Proj  = glm::perspective(g_camera.fovy(), 1.0f, 0.001f, 10000.0f);

  mat_Model = glm::mat4(1.0);

  mat_PVM   = mat_Proj*mat_View*mat_Model;

  mat_VM = mat_View*mat_Model;

  mat_Normal[0][0] = mat_VM[0][0];
  mat_Normal[0][1] = mat_VM[0][1];
  mat_Normal[0][2] = mat_VM[0][2];
  mat_Normal[1][0] = mat_VM[1][0];
  mat_Normal[1][1] = mat_VM[1][1];
  mat_Normal[1][2] = mat_VM[1][2];
  mat_Normal[2][0] = mat_VM[2][0];
  mat_Normal[2][1] = mat_VM[2][1];
  mat_Normal[2][2] = mat_VM[2][2];

  glm::vec4 light_vector_vc;
  light_vector_vc = mat_View * light_vector;

  // TODO: 3. Extend Object::draw function to pass variables to the shader program

	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix4fv(loc_u_vm_matrix,     1, false, glm::value_ptr(mat_VM));
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, glm::value_ptr(mat_Normal));

  glUniform3fv(loc_u_light_vector_vc,   1, glm::value_ptr(light_vector_vc));
  glUniform4fv(loc_u_light_ambient,     1, glm::value_ptr(light_ambient));
  glUniform4fv(loc_u_light_diffuse,     1, glm::value_ptr(light_diffuse));
  glUniform4fv(loc_u_light_specular,    1, glm::value_ptr(light_specular));

  g_model.draw(loc_a_vertex, loc_a_normal, loc_u_material_ambient, loc_u_material_diffuse, loc_u_material_specular, loc_u_material_shininess);
	glUseProgram(0);

	Shader::check_gl_error("draw");

  glutSwapBuffers();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
	if ('a' == key || 'A' == key)
	{
		g_camera.rotate_left(0.1);
	}
	else if ('d' == key || 'D' == key)
	{
		g_camera.rotate_right(0.1);
	}

  glutPostRedisplay();
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		g_camera.move_forward(1.0);
		break;
	case GLUT_KEY_DOWN:
		g_camera.move_backward(1.0);
		break;
	case GLUT_KEY_LEFT:
		g_camera.move_left(1.0);
		break;
	case GLUT_KEY_RIGHT:
		g_camera.move_right(1.0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void idle()
{
  glutPostRedisplay();
}
