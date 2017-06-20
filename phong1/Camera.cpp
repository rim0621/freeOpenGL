#include "Camera.h"
#include <glm/ext.hpp>

#include <iostream>

const glm::vec3 Camera::center_position() const
{
  return  glm::vec3(position_.x + front_dir_.x,
                 position_.y + front_dir_.y, 
                 position_.z + front_dir_.z);
}

void Camera::move_forward(float delta)
{
	position_ += delta * front_dir_;	
}

void Camera::move_backward(float delta)
{
  move_forward(-delta);
}

void Camera::move_left(float delta)
{
	position_ -= delta * right_dir_;
}

void Camera::move_right(float delta)
{
  move_left(-delta);
}

void Camera::rotate_left(float delta)
{
	front_dir_ = glm::rotateY(front_dir_, -delta);
	right_dir_ = glm::cross(front_dir_, up_dir_);
}

void Camera::rotate_right(float delta)
{
  rotate_left(-delta);
}