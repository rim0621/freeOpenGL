
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include "vec.hpp"
#include "mat.hpp"
#include "transform.hpp"
#include "time.h"

#define MAX_P 1000
using namespace kmuvcl::math;

GLuint create_shader_from_file(const std::string& filename, GLuint shader_type);

GLuint program; 
GLint  loc_a_position;
GLint  loc_a_color;
GLint  loc_u_M;
GLint  loc_u_V;
GLint  loc_u_P;
int t = 0;
float start = t;
float ang = 25;
float P_Num = 100;
float pre_x = 0;
float pre_y = 0;
float degree_x = 0;
float degree_y = 0;
float R = 1.0f;
float B = 1.0f;
float G = 1.0f;
mat4x4f P, V;
float angle_cube = 0.0f;


float eyex = 0.0f, eyey = 2.0f, eyez = 25.0f;
float centerx = 0.0f, centery = 2.0f, centerz = 0.0f; 
float upx = 0.0f, upy = 1.0f, upz = 0.0f;
class line {
  float position[176];
  float color[176];
  GLubyte indices[44];
  float x;
  float y;
  mat4x4f M, T;

public:
  line(){
    for(int i = 0 ; i < 11 ; i ++ ){
      position[i*8] = i*0.4 - 2.0f; position[i*8+1] = 0.0f; position[i*8+2] = 2.0f; position[i*8+3] = 1.0f;
      position[i*8+4] = i*0.4 - 2.0f; position[i*8+5] = 0.0f; position[i*8+6] = -2.0f; position[i*8+7] = 1.0f;
    }
    for(int i = 11 ; i < 22 ; i ++ ){
      position[i*8] = 2.0f; position[i*8+1] = 0.0f; position[i*8+2] = i*0.4 - 6.4f; position[i*8+3] = 1.0f;
      position[i*8+4] = -2.0f; position[i*8+5] = 0.0f; position[i*8+6] = i*0.4 - 6.4f; position[i*8+7] = 1.0f;
    }
    for(int i = 0 ; i < 22 ; i ++){
      color[i*8] = 1.0f; color[i*8+1] = 1.0f; color[i*8+2] = 1.0f; color[i*8+3] = 1.0f;
      color[i*8+4] = 1.0f; color[i*8+5] = 1.0f; color[i*8+6] = 1.0f; color[i*8+7] = 1.0f;
    }
    for(int i = 0 ; i < 22 ; i ++){
      indices[i*2] = i*2; indices[i*2+1] = i*2+1;
    }
    x = 0;
    y = 0;

  }
  void draw_line(){

    T = translate(0.0f, 0.0f, 0.0f);
    M = T;
    glUniformMatrix4fv(loc_u_M, 1, false, M);
    glUniformMatrix4fv(loc_u_V, 1, false, V);
    glUniformMatrix4fv(loc_u_P, 1, false, P);
    glVertexAttribPointer(loc_a_position, 4, GL_FLOAT, GL_FALSE, 0, position);
    glVertexAttribPointer(loc_a_color, 4, GL_FLOAT, GL_FALSE, 0, color);

    glEnableVertexAttribArray(loc_a_color);
    glEnableVertexAttribArray(loc_a_position);

    glDrawElements(GL_LINES, 44, GL_UNSIGNED_BYTE, indices);


    glDisableVertexAttribArray(loc_a_color);
    glDisableVertexAttribArray(loc_a_position);

  }
};
class Point {
  float x;
  float y;
  float z;
  float start;
  float v_x;
  float v_y;
  float v_z;
  mat4x4f M, T, R;
  float color[32];
  float position[32];
  GLubyte indices[36];


public:
  Point(){
    x = 0;
    y = 0;
    z = 0;
    start = t;
    v_x = ((float)(rand()%400+1)-200)/100000;
    v_y = 0.01 + ((float)(rand()%20+1))/1000;
    v_z = ((float)(rand()%400+1)-200)/100000;
    //V = lookAt(eyex, eyey, eyez,
    //            centerx, centery, centerz,
    //            upx, upy, upz);
    //  P = perspective(ang, 1.0f, 0.001f, 10000.0f);

    color[0] = 1.0f; color[1] = 1.0f; color[2] = 1.0f; color[3] = 1.0f;
    color[4] = 1.0f; color[5] = 1.0f; color[6] = 1.0f; color[7] = 1.0f;
    color[8] = 1.0f; color[9] = 1.0f; color[10] = 1.0f; color[11] = 1.0f;
    color[12] = 1.0f; color[13] = 1.0f; color[14] = 1.0f; color[15] = 1.0f;
    color[16] = 1.0f; color[17] = 1.0f; color[18] = 1.0f; color[19] = 1.0f;
    color[20] = 1.0f; color[21] = 1.0f; color[22] = 1.0f; color[23] = 1.0f;
    color[24] = 1.0f; color[25] = 1.0f; color[26] = 1.0f; color[27] = 1.0f;
    color[28] = 1.0f; color[29] = 1.0f; color[30] = 1.0f; color[31] = 1.0f;

    position[0] = -0.015f; position[1] = -0.015f; position[2] = 0.015f; position[3] = 1.0f; //0
    position[4] = 0.015f; position[5] = -0.015f; position[6] = 0.015f; position[7] = 1.0f;  //1
    position[8] = 0.015f; position[9] = 0.015; position[10] = 0.015f; position[11] = 1.0f; //2
    position[12] = -0.015f; position[13] = 0.015f; position[14] = 0.015f; position[15] = 1.0f;  //3
    position[16] = -0.015f; position[17] = -0.015f; position[18] = -0.015f; position[19] = 1.0f; //4
    position[20] = 0.015f; position[21] = -0.015f; position[22] = -0.015f; position[23] = 1.0f;  //5
    position[24] = 0.015f; position[25] = 0.015f; position[26] = -0.015f; position[27] = 1.0f; //6
    position[28] = -0.015f; position[29] = 0.015f; position[30] = -0.015f; position[31] = 1.0f;  //7

    indices[0] = 0; indices[1] = 1; indices[2] = 2; indices[3] = 0; indices[4] = 2; indices[5] = 3;
    indices[6] = 1; indices[7] = 2; indices[8] = 5; indices[9] = 2; indices[10] = 5; indices[11] = 6;
    indices[12] = 4; indices[13] = 5; indices[14] = 6; indices[15] = 4; indices[16] = 6; indices[17] = 7;
    indices[18] = 0; indices[19] = 3; indices[20] = 4; indices[21] = 3; indices[22] = 4; indices[23] = 7;
    indices[24] = 3; indices[25] = 4; indices[26] = 6; indices[27] = 3; indices[28] = 6; indices[29] = 7;
    indices[30] = 0; indices[31] = 1; indices[32] = 5; indices[33] = 0; indices[34] = 4; indices[35] = 5;
  };
  void update(){

    x += 2*v_x;
    y += 3*v_y - 0.00098 * (t-start);
    z += 2*v_z;

    if (y <= 0.0f){
      cube_bounce();
    }

    if (y <= -0.1f){
      x = 0;
      y = 0;
      z = 0;
      start = t;
      v_x = 0.004*(float)(rand()%4-1.5);
      v_y = 0.01 + ((float)(rand()%4+1))/200;
      v_z = 0.004*(float)(rand()%4-1.5);
    };

    T = translate(x, y, z);
    R = rotate(angle_cube,0.0f,1.0f,1.0f);
    M = T*R;

    glUniformMatrix4fv(loc_u_M, 1, false, M);
    glUniformMatrix4fv(loc_u_V, 1, false, V);
    glUniformMatrix4fv(loc_u_P, 1, false, P);


    glVertexAttribPointer(loc_a_position, 4, GL_FLOAT, GL_FALSE, 0, position);
    glVertexAttribPointer(loc_a_color, 4, GL_FLOAT, GL_FALSE, 0, color);

    glEnableVertexAttribArray(loc_a_color);
    glEnableVertexAttribArray(loc_a_position);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

    glPointSize(4.0f);


    glDisableVertexAttribArray(loc_a_color);
    glDisableVertexAttribArray(loc_a_position);

  }
  void color_change(float R,float G,float B)
  {

    color[0] = R; color[1] = G; color[2] = B; color[3] = 1.0f;
    color[4] = R; color[5] = G; color[6] = B; color[7] = 1.0f;
    color[8] = R; color[9] = G; color[10] = B; color[11] = 1.0f;
    color[12] = R; color[13] = G; color[14] = B; color[15] = 1.0f;
    color[16] = R; color[17] = G; color[18] =B; color[19] = 1.0f;
    color[20] = R; color[21] = G; color[22] = B; color[23] = 1.0f;
    color[24] = R; color[25] = G; color[26] = B; color[27] = 1.0f;
    color[28] = R; color[29] = G; color[30] = B; color[31] = 1.0f;
  }
  void cube_bounce()
{


    this->y -=3*v_y - 0.0009 * (t-start);

}


  void init(){

    position[0] = -0.015f; position[1] = -0.015f; position[2] = 0.015f; position[3] = 1.0f; //0
    position[4] = 0.015f; position[5] = -0.015f; position[6] = 0.015f; position[7] = 1.0f;  //1
    position[8] = 0.015f; position[9] = 0.015; position[10] = 0.015f; position[11] = 1.0f; //2
    position[12] = -0.015f; position[13] = 0.015f; position[14] = 0.015f; position[15] = 1.0f;  //3
    position[16] = -0.015f; position[17] = -0.015f; position[18] = -0.015f; position[19] = 1.0f; //4
    position[20] = 0.015f; position[21] = -0.015f; position[22] = -0.015f; position[23] = 1.0f;  //5
    position[24] = 0.015f; position[25] = 0.015f; position[26] = -0.015f; position[27] = 1.0f; //6
    position[28] = -0.015f; position[29] = 0.015f; position[30] = -0.015f; position[31] = 1.0f;  //7
    x = 0;
    y = 0;
    z = 0;
  }

};



void init();
void mydisplay();
void mykeyboard(unsigned char key, int x, int y);
void myidle();
void mymouse(int button, int state, int x, int y);
void timer(int value);
void specialkey(int key, int x, int y);
void createMenus();
void MenuEvents(int option);
void mymouse(int x, int y);
void passive_mymouse(int x, int y);

Point points[1000];
line line;



int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(1000, 1000);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("Hello FreeGLUT");

  init();
  glutDisplayFunc(mydisplay);
  glutKeyboardFunc(mykeyboard);
  glutSpecialFunc(specialkey);
  glutPassiveMotionFunc(passive_mymouse);
  glutMotionFunc(mymouse);
  glutTimerFunc(1,timer,0);
  createMenus();

  glutMainLoop();

  return 0;
}
void passive_mymouse(int x, int y){
  //std::cout<<"x :"<<x<<" y :"<<y<<std::endl;
  pre_x = x;
  pre_y = y;
}
void mymouse(int x, int y){
  //std::cout<<"x :"<<x<<" y :"<<y<<std::endl;
  degree_x += x - pre_x;
  degree_y += y - pre_y;
  pre_x = x;
  pre_y = y;
}
void timer (int value){

  if(t%10==0)
  {
    angle_cube+=30.0f;
  }


  t += 1;
  if (t > 100000){
    t = 0;
  }
  glutPostRedisplay();
  glutTimerFunc(1, timer, value);
}
void specialkey(int key, int x, int y){
  if ( key == GLUT_KEY_HOME){
    ang = 25;
    degree_x = 0.0f;  degree_y = 0.0f;
    eyex = 0.0f; eyey = 2.0f; eyez = 25.0f;
    centerx = 0.0f; centery = 2.0f;
  }
}
void mykeyboard(unsigned char key, int x, int y){
  //std::cout << "key pressed" << std::endl;
  if (key == 'd' || key == 'D'){
    eyex += 0.1f;
    centerx +=0.1f;
    glutPostRedisplay(); 
  }

  if (key == 'z' || key == 'Z'){
    eyez += 1.0f;
    centerz +=1.0f;
    glutPostRedisplay(); 
  }

  if(eyez>10)
  {
    if (key == 'x' || key == 'X'){
      eyez -= 1.0f;
      centerz -=1.0f;
      glutPostRedisplay();
    }
    std::cout<<eyez<<std::endl;
  }
  else
  {
    eyez+=0.1f;
    centerz+=0.1f;
    std::cout<<"blocked"<<std::endl;
  }
  if (key == 'a' || key == 'A'){
    eyex -= 1.0f;
    centerx -=1.0f;
    glutPostRedisplay();
  }
  if (key == 'w' || key == 'W'){
    eyey += 0.1f;
    centery +=0.1f;
    glutPostRedisplay();
  }
  if (key == 's' || key == 'S'){
    eyey -= 0.1f;
    centery -=0.1f;
    glutPostRedisplay();
  }
  if (key == '-' || key == '_'){
    if ( ang < 80){
      ang += 1;
    }
    glutPostRedisplay();
  }
  if (key == 0x2B || key == '='){
    if (ang >1){
      ang += -1;
    }
    glutPostRedisplay(); 
  }
}
GLuint create_shader_from_file(const std::string& filename, GLuint shader_type){
  GLuint shader = 0;
  shader = glCreateShader(shader_type);
  std::ifstream shader_file(filename.c_str());
  std::string shader_string;
  shader_string.assign(
    (std::istreambuf_iterator<char>(shader_file)),
    std::istreambuf_iterator<char>());

  const GLchar* shader_src = shader_string.c_str();

  glShaderSource(shader, 1, (const GLchar**)&shader_src, NULL);
  glCompileShader(shader);

  return shader;
}
void init(){
  glewInit();

  GLuint vertex_shader
    = create_shader_from_file("./shader/vertex.glsl", GL_VERTEX_SHADER);

  GLuint fragment_shader
    = create_shader_from_file("./shader/fragment.glsl", GL_FRAGMENT_SHADER);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  loc_u_M = glGetUniformLocation(program, "u_M");
  loc_u_V = glGetUniformLocation(program, "u_V");
  loc_u_P = glGetUniformLocation(program, "u_P");


  loc_a_position = glGetAttribLocation(program, "a_position");
  loc_a_color    = glGetAttribLocation(program, "a_color");



  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
void mydisplay(){


  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  glUseProgram(program);

  V = lookAt(eyex, eyey, eyez,
              centerx, centery, centerz,
              upx, upy, upz)*rotate(degree_x, 0.0f, 1.0f, 0.0f)*rotate(degree_y, 1.0f, 0.0f, 0.0f);
  P = perspective(ang, 1.0f, 0.001f, 10000.0f);

  line.draw_line();
  for(int i = 0 ; i <P_Num; i++){
  points[i].update();
  points[i].color_change((float)((rand()%10)+1)/10,(float)((rand()%10)+1)/10,(float)((rand()%10)+1)/10);
  }

  glUseProgram(0);

  glutSwapBuffers();
}
void createMenus(){
  int menu;
  menu = glutCreateMenu(MenuEvents);

  glutAddMenuEntry("10", 0);
  glutAddMenuEntry("50", 1);
  glutAddMenuEntry("100", 2);
  glutAddMenuEntry("500", 3);
  glutAddMenuEntry("1000", 4);

  glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void MenuEvents(int option){
  switch (option)
  {
    case 0 :
      P_Num = 10;
      for(int i = P_Num ; i < MAX_P ; i++ ){
        points[i].init();
      }
      break;
    case 1 :
      P_Num = 50;
      for(int i = P_Num ; i < MAX_P ; i++ ){
        points[i].init();
      }
      break;
    case 2 :
      P_Num = 100;
      for(int i = P_Num ; i < MAX_P ; i++ ){
        points[i].init();
      }
      break;
    case 3 :
      P_Num = 500;
      for(int i = P_Num ; i < MAX_P ; i++ ){
        points[i].init();
      }
      break;
    case 4 :
      P_Num = 1000;
      for(int i = P_Num ; i < MAX_P ; i++ ){
        points[i].init();
      }
      break;
  }
}
