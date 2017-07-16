#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
  Camera()
    : position_(0,2,8)
	, front_dir_(0,0,-1), up_dir_(0,1,0), right_dir_(1,0,0), fovy_(45)
  {}
  Camera(const glm::vec3& _position, const glm::vec3& _front_dir, const glm::vec3& _up_dir, float _fovy)
    : position_(_position), front_dir_(_front_dir), up_dir_(_up_dir), fovy_(_fovy)
  {
    right_dir_ = glm::cross(front_dir_, up_dir_);
  }

  glm::vec3& move_forward(float delta);
  glm::vec3& move_backward(float delta);
  glm::vec3& move_left(float delta);
  glm::vec3& move_right(float delta);
  void rotate_left(float delta);
  void rotate_right(float delta);
  void rotate_up(float delta);
  void rotate_down(float delta);

  const glm::vec3&  position() const          { return  position_; }
  const glm::vec3&  front_direction() const   { return  front_dir_; }
  const glm::vec3&  up_direction() const      { return  up_dir_; }
  const glm::vec3&  right_direction() const   { return  right_dir_; }
	const glm::vec3		center_position() const;

	const float				fovy() const							{ return fovy_; }
	void							set_fovy(float _fovy)			{ fovy_ = _fovy; }

public:
	glm::vec3 position_;     // position of the camera
  glm::vec3 front_dir_;    // front direction of the camera    (it should be a unit vector whose length is 1)
  glm::vec3 up_dir_;       // up direction of the camera       (it should be a unit vector whose length is 1)
  glm::vec3	right_dir_;    // right direction of the camera    (it should be a unit vector whose length is 1)

	float fovy_;
};
