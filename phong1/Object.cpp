#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>

#include "Object.h"
#include <glm/ext.hpp>

Group::Group(const std::string& name)
{
  m_name = name;

  m_vertices.clear();
  m_normals.clear();
  m_texcoords.clear();

  m_mtl_name = "Default";
}

Material::Material()
{
  m_name = "Default";

  m_ambient   = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
  m_diffuse   = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
  m_specular  = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
  m_shininess = 1.0f;
}

Material::Material(const std::string& name,
  glm::vec4& ambient, glm::vec4& diffuse, glm::vec4& specular, float& shininess)
{
  m_name = name;

  m_ambient   = ambient;
  m_diffuse   = diffuse;
  m_specular  = specular;
  m_shininess = shininess;
}

void Object::draw(int loc_a_vertex, int loc_a_normal,
  int loc_u_material_ambient, int loc_u_material_diffuse,
  int loc_u_material_specular, int loc_u_material_shininess)
{
  for (size_t i=0; i<m_groups.size(); ++i)
  {
    Group& group = m_groups[i];
    std::cout << group.m_mtl_name << std::endl;

    Material& mtl = m_materials[group.m_mtl_name];
    glm::vec4& ambient  = mtl.m_ambient;
    glm::vec4& diffuse  = mtl.m_diffuse;
    glm::vec4& specular = mtl.m_specular;
    float& shininess    = mtl.m_shininess;

    // TODO: extend the code in below for Phong shading model
    glUniform4fv(loc_u_material_ambient,  1, glm::value_ptr(ambient));
    glUniform4fv(loc_u_material_diffuse,  1, glm::value_ptr(diffuse));
    glUniform4fv(loc_u_material_specular, 1, glm::value_ptr(specular));
    glUniform1f(loc_u_material_shininess, shininess);
    //std::cout<<"================="<<std::endl;
    //std::cout<<ambient.r<<std::endl;
    glVertexAttribPointer(loc_a_vertex, 3  , GL_FLOAT, false, 0, group.m_vertices.data());
    glVertexAttribPointer(loc_a_normal, 3  , GL_FLOAT, false, 0, group.m_vertices.data());

    glEnableVertexAttribArray(loc_a_vertex);
    glEnableVertexAttribArray(loc_a_normal);


    glDrawArrays(GL_TRIANGLES, 0, group.m_vertices.size());

    glDisableVertexAttribArray(loc_a_vertex);
    glDisableVertexAttribArray(loc_a_normal);
  }
}

void Object::print()
{
  for (size_t i = 0; i < m_groups.size(); ++i)
  {
    Group& group = m_groups[i];

    Material& mtl = m_materials[group.m_mtl_name];
    glm::vec4& ambient = mtl.m_ambient;
    glm::vec4& diffuse = mtl.m_diffuse;
    glm::vec4& specular = mtl.m_specular;
    float& shininess = mtl.m_shininess;

    std::cout << "Group: " << group.m_name << std::endl;

    std::cout << "\tUsemtl: " << group.m_mtl_name << std::endl;
    std::cout << "\t\tAmbient: " << ambient.r << " " << ambient.g << " " << ambient.b << std::endl;
    std::cout << "\t\tDiffuse: " << diffuse.r << " " << diffuse.g << " " << diffuse.b << std::endl;
    std::cout << "\t\tSpecular: " << specular.r << " " << specular.g << " " << specular.b << std::endl;
    std::cout << "\t\tShininess: " << shininess << std::endl;

    std::cout << "\tVertices: " << group.m_vertices.size() << std::endl;
    for (size_t i = 0; i < group.m_vertices.size(); ++i)
    {
      //std::cout << "\t\t" << group.m_vertices[i].x << " " << group.m_vertices[i].y << " " << group.m_vertices[i].z << std::endl;

    }
    std::cout << "\tTexcoord: " << group.m_texcoords.size() << std::endl;
    for (size_t i = 0; i < group.m_texcoords.size(); ++i)
    {
      //std::cout << "\t\t" << group.m_texcoords[i].x << " " << group.m_texcoords[i].y << std::endl;
    }

    std::cout << "\tNormal: " << group.m_normals.size() << std::endl;
    for (size_t i = 0; i < group.m_normals.size(); ++i)
    {
      //std::cout << "\t\t" << group.m_normals[i].x << " " << group.m_normals[i].y << " " << group.m_normals[i].z << std::endl;
    }
  }
}

bool Object::load_simple_obj(const std::string& filename)
{
	std::ifstream file(filename.c_str());

	if (!file.is_open())
	{
		std::cerr << "failed to open file: " << filename << std::endl;
		return false;
	}

  std::string type_str;
  char slash;				// get only on character '\'

  std::string line;
  std::locale loc;

  bool has_texcoord = false;
  bool has_normal   = false;;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;

  m_groups.push_back(Group("Default"));
  m_materials["Default"] = Material();

  PATH = filename.substr(0, filename.find_last_of('/'));

  std::stringstream ss;

	while (!file.eof())
	{
		std::getline(file, line);

		ss.clear();
		ss.str(line);

		// comment or space
		if (line[0] == '#' || std::isspace(line[0], loc))
		{
			continue; // skip
		}
    else if (line.substr(0, 7) == "mtllib ") // open material file
    {
      std::string mtl_filename;
      ss >> type_str >> mtl_filename;

      std::string fullpath = PATH + '/' + mtl_filename;
      load_simple_mtl(fullpath);
    }
		else if (line.substr(0, 2) == "v ")   // vertex
		{
			glm::vec3 vertex;
			ss >> type_str >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		}
		else if (line.substr(0, 3) == "vt ")  // texture coordinate
		{
			glm::vec2 texcoord;
			ss >> type_str >> texcoord.s >> texcoord.t;
			texcoords.push_back(texcoord);

      has_texcoord = true;
		}
		else if (line.substr(0, 3) == "vn ")  // vertex normal
		{
			glm::vec3 norm;
			ss >> type_str >> norm.x >> norm.y >> norm.z;
			normals.push_back(norm);

      has_normal = true;
		}
    else if (line.substr(0, 2) == "g ")   // group
    {
      std::string group_name;
      ss >> type_str >> group_name;

      m_groups.push_back(Group(group_name));
    }
    else if (line.substr(0, 7) == "usemtl ") // materail
    {
      std::string mtl_name;
      ss >> type_str >> mtl_name;

      m_groups.back().m_mtl_name = mtl_name;
    }
		// faces
		else if (line.substr(0, 2) == "f ")
		{
      Group& group = m_groups.back();

      std::vector<std::string> lines;

      glm::u16vec3 vertex_idx;
      glm::u16vec3 texcoord_idx;
      glm::u16vec3 normal_idx;

      if (!has_texcoord && !has_normal) // f v
      {
        ss >> type_str >> vertex_idx.x >> vertex_idx.y >> vertex_idx.z;

        group.m_vertices.push_back(vertices[vertex_idx[0] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[1] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[2] - 1]);
      }
      else if (has_texcoord && !has_normal) // f v/vt
      {
        ss >> type_str >>
          vertex_idx.x >> slash >> texcoord_idx.x >>
          vertex_idx.y >> slash >> texcoord_idx.y >>
          vertex_idx.z >> slash >> texcoord_idx.z;

        group.m_vertices.push_back(vertices[vertex_idx[0] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[1] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[2] - 1]);

        group.m_texcoords.push_back(texcoords[texcoord_idx[0] - 1]);
        group.m_texcoords.push_back(texcoords[texcoord_idx[1] - 1]);
        group.m_texcoords.push_back(texcoords[texcoord_idx[2] - 1]);
      }
      else if (!has_texcoord && has_normal) // f v//vn
      {
        ss >> type_str >>
          vertex_idx.x >> slash >> slash >> normal_idx.x >>
          vertex_idx.y >> slash >> slash >> normal_idx.y >>
          vertex_idx.z >> slash >> slash >> normal_idx.z;

        group.m_vertices.push_back(vertices[vertex_idx[0] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[1] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[2] - 1]);

        group.m_normals.push_back(normals[normal_idx[0] - 1]);
        group.m_normals.push_back(normals[normal_idx[1] - 1]);
        group.m_normals.push_back(normals[normal_idx[2] - 1]);
      }
      else if (has_texcoord && has_normal) // f v/vt/vn
      {
        ss >> type_str >>
          vertex_idx.x >> slash >> texcoord_idx.x >> slash >> normal_idx.x >>
          vertex_idx.y >> slash >> texcoord_idx.y >> slash >> normal_idx.y >>
          vertex_idx.z >> slash >> texcoord_idx.z >> slash >> normal_idx.z;

        group.m_vertices.push_back(vertices[vertex_idx[0] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[1] - 1]);
        group.m_vertices.push_back(vertices[vertex_idx[2] - 1]);

        group.m_texcoords.push_back(texcoords[texcoord_idx[0] - 1]);
        group.m_texcoords.push_back(texcoords[texcoord_idx[1] - 1]);
        group.m_texcoords.push_back(texcoords[texcoord_idx[2] - 1]);

        group.m_normals.push_back(normals[normal_idx[0] - 1]);
        group.m_normals.push_back(normals[normal_idx[1] - 1]);
        group.m_normals.push_back(normals[normal_idx[2] - 1]);
      }
    }
	}

	std::cout << "finished to read: " << filename << std::endl;
	return true;
}

bool Object::load_simple_mtl(const std::string& filename)
{
  std::ifstream file(filename.c_str());

  if (!file.is_open())
  {
    std::cerr << "failed to open file: " << filename << std::endl;
    return false;
  }

  float r, g, b;
  std::string type_str;

  std::string line;
  std::locale loc;

  std::string   name;
  glm::vec4     ambient;
  glm::vec4     diffuse;
  glm::vec4     specular;
  float         shininess;

  bool is_name = false;
  bool is_ka   = false;
  bool is_kd   = false;
  bool is_ks   = false;
  bool is_ns   = false;

  std::stringstream ss;

  while (!file.eof())
  {
    std::getline(file, line);

    ss.clear();
    ss.str(line);

    // comment or space
    if (line[0] == '#' || std::isspace(line[0], loc))
    {
      continue; // skip
    }
    else if (line.substr(0, 7) == "newmtl ")
    {
        ss >> type_str >> name;
        is_name = true;
    }
    else if (line.substr(0, 3) == "Ka ")
    {
      ss >> type_str >> r >> g >> b;
      ambient = glm::vec4(r, g, b, 1.0f);
      is_ka = true;
    }
    else if (line.substr(0, 3) == "Kd ")
    {
      ss >> type_str >> r >> g >> b;
      diffuse = glm::vec4(r, g, b, 1.0f);
      is_kd = true;
    }
    else if (line.substr(0, 3) == "Ks ")
    {
      ss >> type_str >> r >> g >> b;
      specular = glm::vec4(r, g, b, 1.0f);
      is_ks = true;
    }
    else if (line.substr(0, 3) == "Ns ")
    {
      ss >> type_str >> shininess;
      is_ns = true;
    }

    if (is_name == true && is_ka == true && is_kd == true && is_ks == true && is_ns == true)
    {
      m_materials[name] = Material(name, ambient, diffuse, specular, shininess);

      is_name = false;
      is_ka = false;
      is_kd = false;
      is_ks = false;
      is_ns = false;
    }

  }
    std::cout << "finished to read: " << filename << std::endl;
    return true;
}
