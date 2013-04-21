#include "model_loader.hpp"

#include <fstream>
#include <array>

#include <boost/algorithm/string.hpp>

void model_loader::load_model(const std::string& path, std::vector<vertex>& vertex_data) {
  using std::vector;
  using std::array;
  using std::string;
  using std::ifstream;
  using std::cout;
  using std::endl;

  using boost::is_any_of;
  using boost::split;
  using boost::erase_all;

  vertex_data.clear();

  ifstream file_stream(path);

  if (!file_stream.is_open()) {
      return;
  }

  vector<string> split_vector_1(8);
  vector<string> split_vector_2(8);

  string object_name("default");
  string group_name("default");
  string material_name("default");

  //unordered_set<string> groups;
  //unsigned int smoothing_group(0);

  string line("");
  while (getline(file_stream, line)) {
    using std::stof;
    using std::stoul;

    boost::trim(line);

    try {
      split(split_vector_1, line, is_any_of(" "), boost::token_compress_on);

      if (split_vector_1.size() > 1) {
        string type = split_vector_1[0];

        if ("#" == type) {
        } else if ("v" == type && 4 <= split_vector_1.size()) {
          auto v1 = stof(split_vector_1[1]);
          auto v2 = stof(split_vector_1[2]);
          auto v3 = stof(split_vector_1[3]);

          vertex_data.emplace_back(v1, v2, v3, 1.0f);
        } else if ("vn" == type && 4 <= split_vector_1.size()) {
          auto vn1 = stof(split_vector_1[1]);
          auto vn2 = stof(split_vector_1[2]);
          auto vn3 = stof(split_vector_1[3]);

          //vertex_normal_data.emplace_back(vn1, vn2, vn3, 1.0f);
        } else if ("f" == type && 4 <= split_vector_1.size()) {
          int v_idx[4] = {0, 0, 0, 0};
          int t_idx[4] = {0, 0, 0, 0};
          int n_idx[4] = {0, 0, 0, 0};

          for (unsigned int i = 1; i < split_vector_1.size() && 5 > i; ++i) {
            split(split_vector_2, split_vector_1[i], is_any_of("/"), boost::token_compress_on );

            v_idx[i - 1] = stoul(split_vector_2[0]);

            if (2 <= split_vector_2.size()) t_idx[i - 1]  = stoul(split_vector_2[1]);
            if (3 <= split_vector_2.size()) n_idx[i - 1]  = stoul(split_vector_2[2]);
          }

          if (3 == split_vector_1.size()) {
            //model.triangle_data.emplace_back(v_idx[0], v_idx[1], v_idx[2]);
          } else {
            //model.quadangle_data.emplace_back(v_idx[0], v_idx[1], v_idx[2], v_idx[3]);
          }
        } else if ("g" == type && 2 <= split_vector_1.size()) {
          //groups.insert(next(begin(split_vector_1)), end(split_vector_1));
        } else if ("o" == type && 2 == split_vector_1.size()) {
          object_name = split_vector_1[1];
        } else if ("usemtl" == type && 2 == split_vector_1.size()) {
          material_name = split_vector_1[1];
        }
      }

    } catch (std::exception& e) {
      std::cerr << "Line not recognized: " << line << " (" << e.what() << ")" << std::endl;
    }
  }

  //transform_indices(model);
  //get_global_extrema(model);
  //get_center_of_mass(model);
}

void model_loader::get_extrema(const std::vector<vertex>& vertex_data, glm::vec3& minimum, glm::vec3& maximum) {
    const float min_float = std::numeric_limits<float>::lowest();
    const float max_float = std::numeric_limits<float>::max();

    glm::vec4 temp_min = glm::vec4(max_float, max_float, max_float, 1.0f);
    glm::vec4 temp_max = glm::vec4(min_float, min_float, min_float, 1.0f);

    for (const auto& vertex: vertex_data) {
      temp_min = glm::min(temp_min, vertex);
      temp_max = glm::max(temp_max, vertex);
    }

    minimum = glm::vec3(temp_min.x, temp_min.y, temp_min.z);
    maximum = glm::vec3(temp_max.x, temp_max.y, temp_max.z);
}


void model_loader::transform_vertices(const glm::vec3& minimum, const glm::vec3& maximum, std::vector<vertex>& vertex_data) {
    const glm::ivec3 triangle_transformation(-1, -1, -1);
    const glm::ivec4 quadangle_transformation(-1, -1, -1, -1);

    const glm::vec4 temp_min = glm::vec4(minimum, 1.0f);


    for (auto& vertex: vertex_data) {
        vertex -= temp_min;
    }

    const glm::vec4 temp_max = glm::vec4(maximum, 1.0f) - temp_min;
    const float max_dimension = glm::max(temp_max.x, glm::max(temp_max.y, temp_max.z));

    for (auto& vertex: vertex_data) {
        vertex /= max_dimension;
    }

//    for (auto& quadangle: model.quadangle_data) {
//    quadangle += quadangle_transformation;
//    }
}



//void
//model_loader::get_center_of_mass(raw_model_data& model) {
//  const double one_over_n = 1.0 / model.vertex_data.size();

//  vertex center_of_mass(0.0f, 0.0f, 0.0f, 0.0f);

//  for (const auto& vertex: model.vertex_data) {
//    center_of_mass += vertex * one_over_n;
//  }

//  model.center_of_mass = center_of_mass;
//}


