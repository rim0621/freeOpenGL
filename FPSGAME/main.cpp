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
#include "SOIL.h"
#include <cmath>

using namespace std;

void init();
void display();
void reshape(int, int);
void idle();
void keyboard(unsigned char, int, int);
void special(int, int, int);
void mousemotion(int, int);
void menu_item(int);
void mouseButton(int, int, int, int);
void callMenu();
void centerLine();
bool isCollide(glm::vec3);
void inputFile();
void outputFile();
void printCon();

#define FILE_INPUTMODE 1
#define FILE_OUTPUTMODE 2
#define FILE_PRINT 3

#define OBJNUM 6

static int debug = 1;
int mode = 0;

GLuint		program;

GLint			loc_a_vertex;
GLint     loc_u_vm_matrix;
GLint			loc_u_pvm_matrix;
GLint     loc_u_normal_matrix;


GLint     loc_a_texcoord;
GLuint    texid;
GLint     loc_u_texid;

GLint     loc_u_light_vector_vc;
GLint     loc_a_normal;
GLint     loc_u_light_ambient;
GLint     loc_u_light_diffuse;
GLint     loc_u_light_specular;

GLint     loc_u_material_ambient;
GLint     loc_u_material_diffuse;
GLint     loc_u_material_specular;
GLint     loc_u_material_shininess;



glm::mat4 mat_PVM, mat_VM;
glm::mat3 mat_Normal;

Camera		g_camera;											// viewer (you)
Object    objArr[OBJNUM];
Object    g_fan;
Object    g_gun;

glm::vec4 light_vector       = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
glm::vec4 light_ambient      = glm::vec4(0.3f, 0.3f, 0.0f, 1.0f);
glm::vec4 light_diffuse      = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec4 light_specular     = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

float the=0.0f;
float angle = 0;

int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);//rgba
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Modeling & Navigating Your Studio");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutIdleFunc(idle);


  glutPassiveMotionFunc(mousemotion);

  callMenu();//calling fuc menus

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

  objArr[0].load_simple_obj("./data/dwarf.obj");
  objArr[1].load_simple_obj("./data/dog.obj");
  objArr[2].load_simple_obj("./data/elephant.obj");
  objArr[3].load_simple_obj("./data/status.obj");
  objArr[4].load_simple_obj("./data/hourse.obj");
  objArr[5].load_simple_obj("./data/step.obj");

  g_fan.load_simple_obj("./data/fan.obj");
  g_gun.load_simple_obj("./data/final1.obj");

  glFrontFace(GL_CCW);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);




  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // for wireframe rendering


  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_DEPTH_TEST);

  glutSetCursor(GLUT_CURSOR_NONE);

	program = Shader::create_program("./shader/simple.vert", "./shader/simple.frag");

	loc_u_pvm_matrix	= glGetUniformLocation(program, "u_pvm_matrix");
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

	loc_a_vertex			= glGetAttribLocation(program, "a_vertex");

loc_a_normal             = glGetAttribLocation(program, "a_normal");

  loc_u_texid       = glGetAttribLocation(program,"u_texid");
  loc_a_texcoord    = glGetAttribLocation(program,"a_texcoord");


  //texture load

 int width, height, channels;

 unsigned char* image = SOIL_load_image("GunBoot.png",
  &width, &height, &channels, SOIL_LOAD_RGB);

  glGenTextures(1,&texid);
  glBindTexture(GL_TEXTURE_2D,texid);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);


  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//클떄
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);


 SOIL_free_image_data(image);

 	g_fan.o_translate(0,8,0);
 	g_fan.o_scale(1.5,1.5,1.5);

  objArr[0].o_translate(4,0,-5);
  objArr[0].o_scale(0.6,0.6,0.6);
  objArr[0].o_rotate(10,0,1,0);

  objArr[1].o_translate(-4,1,-5);
 	objArr[1].o_scale(0.3,0.3,0.3);
 	objArr[1].o_rotate(90,0,1,0);

  objArr[2].o_translate(-4,1,-10);
  objArr[2].o_scale(0.02,0.02,0.02);

 	objArr[3].o_translate(4,2,-10);
  objArr[3].o_scale(0.02,0.02,0.02);

 	objArr[4].o_translate(-4,0,0);
 	objArr[4].o_scale(0.8,0.8,0.8);
 	objArr[4].o_rotate(90,0,1,0);

 	objArr[5].o_translate(5,0,0);
 	objArr[5].o_scale(0.01,0.01,0.01);
 	objArr[5].o_rotate(90,0,1,0);
}


void display()
{
  if(mode==0)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    // for wireframe rendering
  if(mode==1)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

      glm::vec4 light_vector_vc;
      light_vector_vc = mat_View * light_vector;


	mat_Proj = glm::perspective(g_camera.fovy(), 1.0f, 0.001f, 10000.0f);
	glm::mat4 mat_PV = mat_Proj*mat_View;
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PV));
  centerLine();

	g_fan.o_rotate(angle,0,1,0);
  g_fan.draw(loc_u_pvm_matrix, loc_a_vertex,  loc_a_normal,
             loc_u_material_ambient,  loc_u_material_diffuse,
             loc_u_material_specular,  loc_u_material_shininess, loc_a_texcoord, mat_PV);

    for(int i=0 ; i<OBJNUM ; i++){
      objArr[i].draw(loc_u_pvm_matrix, loc_a_vertex,  loc_a_normal,
            loc_u_material_ambient,  loc_u_material_diffuse,
            loc_u_material_specular,  loc_u_material_shininess, loc_a_texcoord, mat_PV);
    }
    g_gun.o_translate(g_camera.position_.x+0.5, g_camera.position_.y-1.5,g_camera.position_.z+g_camera.front_dir_.z-1);
    g_gun.o_scale(0.01f,0.01f,0.01f);
    g_gun.o_rotate(3.5,0,1,0);
    g_gun.o_rotate(0.1f,0,0,1);
    g_gun.o_rotate(the,0, 1, 0);
    g_gun.draw(loc_u_pvm_matrix,loc_a_vertex,  loc_a_normal,
    loc_u_material_ambient,  loc_u_material_diffuse,
    loc_u_material_specular,  loc_u_material_shininess, loc_a_texcoord,mat_PV);
    glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PV));
    glUniformMatrix4fv(loc_u_vm_matrix,     1, false, glm::value_ptr(mat_VM));
     glUniformMatrix3fv(loc_u_normal_matrix, 1, false, glm::value_ptr(mat_Normal));
     g_gun.M = glm::mat4(1.0);


    mat_PV *= glm::translate(glm::vec3(g_camera.position_.x + g_camera.front_dir_.x, g_camera.position_.y + g_camera.front_dir_.y, g_camera.position_.z + g_camera.front_dir_.z));;
    glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PV));
    glutSolidSphere(0.01,20,20);
    glFlush();

    mat_PV *= glm::translate(glm::vec3(g_camera.position_.x, g_camera.position_.y-0.3,g_camera.position_.z-0.1))* glm::scale(glm::vec3(0.008f,0.008f,0.008f))*glm::rotate(1.5f,glm::vec3(0.0f, 0.0f, 1.0f))*glm::rotate(1.3f,glm::vec3(0.0f, 1.0f, 0.0f))*glm::rotate(the,glm::vec3(0.0f, 0.0f, 1.0f))
    * glm::mat4(1.0);
  	glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PV));
    glUniformMatrix4fv(loc_u_vm_matrix,     1, false, glm::value_ptr(mat_VM));
     glUniformMatrix3fv(loc_u_normal_matrix, 1, false, glm::value_ptr(mat_Normal));
     glUniform3fv(loc_u_light_vector_vc,   1, glm::value_ptr(light_vector_vc));
       glUniform4fv(loc_u_light_ambient,     1, glm::value_ptr(light_ambient));
       glUniform4fv(loc_u_light_diffuse,     1, glm::value_ptr(light_diffuse));
       glUniform4fv(loc_u_light_specular,    1, glm::value_ptr(light_specular));

   mat_PV *= glm::mat4(1.0)*glm::mat4(1.0);
   glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PV));

	glUseProgram(0);
	Shader::check_gl_error("draw");

  glutSwapBuffers();


}
void centerLine(){

  for(int i = 0 ; i <= 40; i++)
  {
    glColor3f(1.f,0.f,0.f);    // red
    if (i==20) glLineWidth(2.5f);
    else glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);    // x
    glVertex3f(20.0, 0.0, -20.0+i);  // start
    glVertex3f(-20.0, 0.0, -20.0+i);  // fin
    glEnd();

    glColor3f(0.f,0.f,1.f);    // blue
    glBegin(GL_LINE_LOOP);    // z
     glVertex3f(-20.0+i, 0.0, 20.0);
     glVertex3f(-20.0+i, 0.0, -20.0);
    glEnd();
  }

glColor3f(0.f,1.f,0.f);    // Green
glLineWidth(2.5f);
glBegin(GL_LINE_LOOP);    // y
 glVertex3f(0.0, 10.0, 0.0);
 glVertex3f(0.0, -10.0, 0.0);
glEnd();
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
  // TODO: properly handle keyboard event
  glm::vec3 cpos;
  if(key=='Q'||key=='q'){
    g_camera.rotate_left(0.01f);
  }
  if(key=='E'||key=='e'){
    g_camera.rotate_right(0.01f);
  }
  if(key == 'A'||key=='a'){
    cpos = g_camera.move_left(0.1f);
    if(isCollide(cpos)){
      g_camera.move_right(0.1f);
    }
  }
  if(key == 'D'||key=='d'){
    cpos = g_camera.move_right(0.1f);
    if(isCollide(cpos)){
      g_camera.move_left(0.1f);
    }
  }
  if(key=='W'||key=='w'){
    cpos = g_camera.move_forward(0.1f);
    if(isCollide(cpos)){
      g_camera.move_backward(0.1f);
    }
  }
  if(key=='S'||key=='s'){
    cpos = g_camera.move_backward(0.1f);
    if(isCollide(cpos)){
      g_camera.move_forward(0.1f);
    }
  }
  glutPostRedisplay();
}

void special(int key, int x, int y)
{
  glm::vec3 cpos;
	// TODO: properly handle special keyboard event
  if(key == GLUT_KEY_RIGHT){
    cpos = g_camera.move_right(0.1f);
    if(isCollide(cpos)){
      g_camera.move_left(0.1f);
    }
  }
  if(key == GLUT_KEY_LEFT){
      cpos = g_camera.move_left(0.1f);
      if(isCollide(cpos))
      {
        g_camera.move_right(0.1f);
      }
  }
  if(key == GLUT_KEY_UP){
      cpos = g_camera.move_forward(0.1f);
      if(isCollide(cpos)){
        g_camera.move_backward(0.1f);
      }
  }
  if(key == GLUT_KEY_DOWN){
      cpos = g_camera.move_backward(0.1f);
      if(isCollide(cpos)){
        g_camera.move_forward(0.1f);
      }
  }
  glutPostRedisplay();

}
void mousemotion(int x, int y)
{
  static int prex = 400;
  static int prey = 400;

  if(x < prex){
    g_camera.rotate_left(0.01f);
       the+=0.01f;
  }
  if(x>prex){
    g_camera.rotate_right(0.01f);
        the-=0.01f;
  }
  // if(y>prey){
  //    g_camera.rotate_up(0.01f);
  // }
  //  if(y<prey){
  //    g_camera.rotate_down(0.01f);
  // }

  glutPostRedisplay();
}

void idle()
{
	// TODO: update your variables in idle time

  int target = 99;

  glm::vec3 cPos = g_camera.position();
  glm::vec3 cFront = g_camera.front_direction();

  unsigned char objSelFlag[OBJNUM] = {0};
  double distance[OBJNUM];
  // int select;
  for(int i =0; i<OBJNUM;i++){
    glm::vec3 tmp1 = glm::vec3(objArr[i].CenPos()[0]-cPos[0],0, objArr[i].CenPos()[2]-cPos[2]);
    glm::vec3 tmp2 = cFront;

    distance[i] = sqrt(tmp1[0] * tmp1[0]+tmp1[2]*tmp1[2]);
    double dot = tmp1[0] * tmp2[0] + tmp1[2]*tmp2[2];
    double vecSize = sqrt(tmp1[0]*tmp1[0]+tmp1[2]*tmp1[2]) * sqrt(tmp2[0]*tmp2[0]+ tmp2[2]*tmp2[2]);
    if((dot/vecSize>=0) && (dot/vecSize<=cos(20*(M_PI/180))))
      objSelFlag[i] = 1;
    else objSelFlag[i] = 0;
  }

  int selCnt = 0;
  for(int i = 0; i<OBJNUM;i++){
    if(objSelFlag[i]==1)
      selCnt++;
  }
  if(selCnt == 0) target =99;
  else if(selCnt ==1){
    for(int i=0; i<OBJNUM;i++){
      if(objSelFlag[i]==1)
        target = i+1;
    }
  }
  else{
    double minDistance =100;
    for(int i=0; i<OBJNUM;i++){
      if(distance[i]<minDistance){
        minDistance = distance[i];
        target = i;
      }
    }
  }

	angle += 2.0;
	if (angle > 360.0)
		angle -= 360.0;


	glutPostRedisplay();
}
void callMenu(){
    int subMenu1 = glutCreateMenu(menu_item);
    glutAddMenuEntry("Input", FILE_INPUTMODE);
    glutAddMenuEntry("Output", FILE_OUTPUTMODE);
    glutAddMenuEntry("Print", FILE_PRINT);

    int menu = glutCreateMenu(menu_item);
    glutAddSubMenu("File", subMenu1);

    glutSetMenu(menu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void menu_item(int item)
{
  if(debug) printf("menu item item=%d\n", item);
  switch (item) {
    case FILE_INPUTMODE:
      printf("File, Input\n");
      inputFile();
      break;
    case FILE_OUTPUTMODE:
      printf("File, Output\n");
      outputFile();
      break;
    case FILE_PRINT:
      printf("File, Print\n");
      printCon();
      break;
    default:
      break;
  }
  glutPostRedisplay();
}

bool isCollide(glm::vec3 cPos)
{
  float cPosX = cPos[0], cPosZ = cPos[2];

  for(int i=0 ; i<OBJNUM ; i++)
  {
    float oPosX = objArr[i].CenPos()[0];
    float oPosZ = objArr[i].CenPos()[2];
    if( oPosX-2.0f <= cPosX && oPosX+2.0f >= cPosX
       &&
        oPosZ-2.0f <= cPosZ && oPosZ+2.0f >= cPosZ )
      return true;
  }
  return false;

}
void inputFile(){
  ifstream inFile("output.txt");

  for(int k = 0 ; k < OBJNUM;k++){
    string objname;
    float Pos[3][3];
    float angle;
    inFile >> objname;
    for(int i=0; i<3; i++){
      for(int j =0 ; j<3 ; j++){
         inFile >> Pos[i][j];
        //  cout << CenPos()[i] << endl;
       }
    }inFile>>angle;
    objArr[k].o_translate(Pos[0][0],Pos[0][1],Pos[0][2]);
    objArr[k].o_scale(Pos[1][0],Pos[1][1],Pos[1][2]);
    objArr[k].o_rotate(angle, Pos[2][0],Pos[2][1],Pos[2][2]);
  }
  inFile.close();
  glutPostRedisplay();
}
void outputFile(){
  ofstream outFile("output.txt");

  for(int k = 0 ; k< OBJNUM; k++){
    outFile<<objArr[k].PATH<<endl;
    for(int i=0;i<3;i++){
      outFile<<objArr[k].CenPos()[i]<<" "<<endl;
    }
    for(int i=0;i<3;i++){
      outFile<<objArr[k].ScaPos()[i]<<" "<<endl;
    }
    for(int i=0;i<3;i++){
      outFile<<objArr[k].RoPos()[i]<<" "<<endl;
    }
    outFile<<objArr[k].getAngle()<<" "<<endl;
  }
  outFile.close();
}
void printCon(){

  for(int k = 0 ; k< OBJNUM; k++){
    cout<<objArr[k].PATH<<endl;
    for(int i=0;i<3;i++){
      cout<<objArr[k].CenPos()[i]<<" ";
    }
    cout<<endl;
    for(int i=0;i<3;i++){
      cout<<objArr[k].ScaPos()[i]<<" ";
    }
    cout<<endl;
    for(int i=0;i<3;i++){
      cout<<objArr[k].RoPos()[i]<<" ";
    }
    cout<<objArr[k].getAngle()<<" "<<endl;
  }
}
