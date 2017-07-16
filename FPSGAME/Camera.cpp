#include "Camera.h"
#include <glm/ext.hpp>

const glm::vec3 Camera::center_position() const
{
  return  glm::vec3(position_.x + front_dir_.x,
                 position_.y + front_dir_.y,
                 position_.z + front_dir_.z);
}

// TODO: fill up the following functions properly
glm::vec3& Camera::move_forward(float delta)
{	//eyey
  position_.z-=(-delta * front_dir_.z);
  position_.x-=(-delta * front_dir_.x);
  return position_;
}

glm::vec3& Camera::move_backward(float delta)
{
  position_.z+=(-delta * front_dir_.z);
  position_.x+=(-delta * front_dir_.x);
  return position_;
}

glm::vec3& Camera::move_left(float delta)
{
  position_.x-=(-delta * front_dir_.z);
  position_.z-=(delta * front_dir_.x);
  return position_;
}

glm::vec3& Camera::move_right(float delta)
{
  position_.x+=(-delta * front_dir_.z);
  position_.z+=(delta * front_dir_.x);
  return position_;
}

void Camera::rotate_left(float delta)
{
  rotate_right(-delta);
}

void Camera::rotate_right(float delta)
{
  front_dir_ = glm::rotateY(front_dir_, -delta);
	right_dir_ = glm::cross(front_dir_, up_dir_);
}
void Camera::rotate_up(float delta)
{
  front_dir_ = glm::rotateX(front_dir_, -delta);
	up_dir_ = glm::cross(right_dir_, front_dir_);
}
void Camera::rotate_down(float delta)
{
  rotate_up(-delta);
}
