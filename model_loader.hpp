/*
 * model_loader.hpp
 *
 *  Created on: Dec 25, 2012
 *      Author: svp
 */

#ifndef MODEL_LOADER_HPP_
#define MODEL_LOADER_HPP_

#include <vector>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <boost/utility.hpp>

typedef glm::vec4 vertex;
typedef glm::vec4 vertex_normal;
typedef glm::ivec3 triangle;
typedef glm::ivec4 quadangle;

//struct raw_model_data {
//  std::vector<vertex> vertex_data;
//  std::vector<vertex_normal> vertex_normal_data;

//  std::vector<triangle> triangle_data;
//  std::vector<quadangle> quadangle_data;

//  glm::vec4 minimum, maximum, center_of_mass;

//  raw_model_data() { std::cout << "Constructed" << std::endl; }
//  ~raw_model_data() { std::cout << "Deconstructed" << std::endl; }

//  raw_model_data& operator=(const raw_model_data&) = delete;

//  raw_model_data& operator=(raw_model_data&& other) {
//    vertex_data = std::move(other.vertex_data);
//    vertex_normal_data = std::move(other.vertex_normal_data);
//    triangle_data = std::move(other.triangle_data);
//    quadangle_data = std::move(other.quadangle_data);

//    minimum = other.minimum;
//    maximum = other.maximum;
//    center_of_mass = other.center_of_mass;

//    return *this;
//  }
//};

namespace model_loader {
  void load_model(const std::string& path, std::vector<vertex>& vertex_data);


  //void transform_indices(raw_model_data& model);
  void get_extrema(const std::vector<vertex>& vertex_data, glm::vec3& minimun, glm::vec3& maximum);
  void transform_vertices(const glm::vec3& minimum, const glm::vec3& maximum, std::vector<vertex>& vertex_data);
  //void get_center_of_mass(raw_model_data& model);
}

#endif /* MODEL_LOADER_HPP_ */
