#pragma once
#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>

struct Group
{
public:
  Group(const std::string& name);

public:
  std::string               m_name;

  std::vector<glm::vec3>    m_vertices;
  std::vector<glm::vec3>    m_normals;
  std::vector<glm::vec2>    m_texcoords;
  std::string               m_mtl_name;
};

struct Material
{
public:
  Material();
  Material(const std::string& name,
    glm::vec4& ambient, glm::vec4& diffuse, glm::vec4& specular, float& shininess);

public:
  std::string     m_name;

  glm::vec4       m_ambient;
  glm::vec4       m_diffuse;
  glm::vec4       m_specular;
  float           m_shininess;
};

class Object
{
public:
  Object() {
	T = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	R = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	S = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
	cenPos = glm::vec3(0,0,0);
  roPos = glm::vec3(0,1,0);
  scaPos = glm::vec3(1,1,1);
  ang =0;
  }
  glm::mat4 M;
  void draw(int loc_u_pvm_matrix, int loc_a_vertex, int loc_a_normal,
    int loc_u_material_ambient, int loc_u_material_diffuse,
    int loc_u_material_specular, int loc_u_material_shininess, int loc_a_texcoord,
	glm::mat4 mat_PV);
  void print();
  std::string PATH;

  void o_translate(float x, float y, float z);
  void o_rotate(float angle, float x, float y, float z);
  void o_scale(float x, float y, float z);
  glm::vec3 CenPos();
  glm::vec3 RoPos();
  glm::vec3 ScaPos();
  float getAngle();

  void boundingBox(int loc_a_vertex,int loc_u_pvm_matrix,glm::mat4 mat_PVM);
	bool load_simple_obj(const std::string& filename);
  bool load_simple_mtl(const std::string& filename);

private:
  // std::string PATH;
  std::vector<Group>              m_groups;
  std::map<std::string, Material> m_materials;
  glm::mat4 T;
  glm::mat4 S;
  glm::mat4 R;
  glm::vec3 cenPos;
  glm::vec3 roPos;
  glm::vec3 scaPos;
  float ang;
};
