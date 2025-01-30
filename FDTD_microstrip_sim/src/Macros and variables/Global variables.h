#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <string>

extern glm::vec3 __y_norm_vec3;
extern glm::vec3 __color_main;
extern glm::vec3 __color_secondary;
 
extern glm::vec3 __y_norm_vec3;
extern glm::vec3 __x_norm_vec3;
extern glm::vec3 __z_norm_vec3;

extern glm::vec3 __color_red;
extern glm::vec3 __color_green;
extern glm::vec3 __color_blue;
extern glm::vec3 __color_black;

extern glm::vec3 __zero_vec3;

extern glm::vec3 __initial_origin;

extern glm::vec3 __initial_translation_vec3;
extern glm::vec3 __initial_scaling_vec3;
extern glm::vec3 __initial_rotating_vec3;
extern glm::mat4 __initial_model_mat4;
extern glm::mat4 __initial_inverse_model_mat4;

extern float __initial_rotation_angle;
extern float __initial_zero_value;

extern const char* __fontFile;