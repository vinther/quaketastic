#include "mesh_loader.hpp"

#include "../typedefs.hpp"
#include "../resources/resource_cache.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <array>

#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>

namespace qts {

mesh mesh_loader::load(const std::string& path) {
	using namespace std;
	using namespace boost;

    mesh mesh;

    ifstream file_stream(path);

    string line = "";
    string key = "";
    string ws = "";

    float x, y, z, w;
    int a, b, c, d;

    int v_idx[4], t_idx[4], n_idx[4];

    vertex vertex;
    vertex_normal vertex_normal;

    glm::uvec4 vertex_idx, texture_idx, normal_idx;
    vector<glm::vec3> normals;
    vector<glm::uint> normals_idxs;

    unsigned int j;
    while (file_stream.good() && !file_stream.eof() && getline(file_stream, line)) {
        key = "";

        boost::trim(line);
        stringstream str(line);
        str >> key;

        if ("v" == key) {
            str >> vertex.x >> vertex.y >> vertex.z;
            mesh.vertices.push_back(vertex);
        } else if ("vn" == key) {
        	str >> vertex_normal.x >> vertex_normal.y >> vertex_normal.z;
			normals.push_back(vertex_normal);
        } else if ("f" == key) {
        	normal_idx = glm::uvec4(0);
            char interupt;
            bool has_normals = false;

            for(j = 0;str.good() && !str.eof() && j < 4; ++j) {
            	if (!(str >> vertex_idx[j]))
            		continue;

            	if ('/' == str.peek()) {
            		str >> interupt >> texture_idx[j];

            		if ('/' == str.peek()) {
						str >> interupt >> normal_idx[j];
						has_normals = true;
					}
            	}
            }

            vertex_idx = vertex_idx - 1;
            normal_idx = normal_idx - 1;

            mesh.triangles.push_back(vertex_idx[0]);
			mesh.triangles.push_back(vertex_idx[1]);
			mesh.triangles.push_back(vertex_idx[2]);

			if (has_normals) {
				normals_idxs.push_back(normal_idx[0]);
				normals_idxs.push_back(normal_idx[1]);
				normals_idxs.push_back(normal_idx[2]);
			}

            if (4 == j) {
                mesh.triangles.push_back(vertex_idx[2]);
                mesh.triangles.push_back(vertex_idx[3]);
                mesh.triangles.push_back(vertex_idx[0]);

                if (has_normals) {
    				normals_idxs.push_back(normal_idx[2]);
    				normals_idxs.push_back(normal_idx[3]);
    				normals_idxs.push_back(normal_idx[0]);
                }
            }
        }
    }

  string object_name = "default";
  string group_name = "default";
  string material_name ="default";



  mesh.vertex_normals.resize(mesh.vertices.size(), glm::vec3(0.0f));
  if (normals_idxs.size() > 0) {
	  for (unsigned int i = 0; i < mesh.triangles.size(); i += 3) {
		  unsigned int ia = mesh.triangles[i];
		  unsigned int ib = mesh.triangles[i+1];
		  unsigned int ic = mesh.triangles[i+2];

		  mesh.vertex_normals[ia] = normals[normals_idxs[i ]] + mesh.vertex_normals[ia];
		  mesh.vertex_normals[ib] = normals[normals_idxs[i+1]] + mesh.vertex_normals[ib];
		  mesh.vertex_normals[ic] = normals[normals_idxs[i+2]] + mesh.vertex_normals[ic];
	  }
  } else {
	  for (unsigned int i = 0; i < mesh.triangles.size(); i += 3) {
		  unsigned int ia = mesh.triangles[i];
		  unsigned int ib = mesh.triangles[i+1];
		  unsigned int ic = mesh.triangles[i+2];

		  assert(ia <= mesh.vertices.size());
		  assert(ib <= mesh.vertices.size());
		  assert(ic <= mesh.vertices.size());

		  glm::vec3 normal = glm::normalize(glm::cross(
				  glm::vec3(mesh.vertices[ib]) - glm::vec3(mesh.vertices[ia]),
				  glm::vec3(mesh.vertices[ic]) - glm::vec3(mesh.vertices[ia])));

		  mesh.vertex_normals[ia] = normal + mesh.vertex_normals[ia];
		  mesh.vertex_normals[ib] = normal + mesh.vertex_normals[ib];
		  mesh.vertex_normals[ic] = normal + mesh.vertex_normals[ic];
	  }
  }

  for (auto& normal : mesh.vertex_normals) {
	  normal = glm::normalize(normal);
  }

  std::hash<std::string> hash_fn;
  mesh.hash = hash_fn(path);

  return mesh;
}

}



  //unordered_set<string> groups;
  //unsigned int smoothing_group(0);


//  while (getline(file_stream, line)) {
//    using std::stof;
//    using std::stoul;

//    boost::trim(line);

//    try {
//        boost::split(split_vector_1, line, boost::algorithm::is_space(), boost::token_compress_on);

//      if (split_vector_1.size() > 1) {
//        string type = split_vector_1[0];

//        if ("v" == type && 4 <= split_vector_1.size()) {
//          float v1 = stof(split_vector_1[1]);
//          float v2 = stof(split_vector_1[2]);
//          float v3 = stof(split_vector_1[3]);

//          vertex_data.emplace_back(v1, v2, v3, 1.0f);
//        } else if ("f" == type && 4 <= split_vector_1.size()) {
//          int v_idx[4] = {0, 0, 0, 0};
//          int t_idx[4] = {0, 0, 0, 0};
//          int n_idx[4] = {0, 0, 0, 0};

//          for (unsigned int i = 1; i < split_vector_1.size() && 5 > i; ++i) {
//            split(split_vector_2, split_vector_1[i], boost::algorithm::is_space(), boost::token_compress_on);

//            v_idx[i - 1] = stoul(split_vector_2[0]);

//            if (2 <= split_vector_2.size()) t_idx[i - 1]  = stoul(split_vector_2[1]);
//            if (3 <= split_vector_2.size()) n_idx[i - 1]  = stoul(split_vector_2[2]);
//          }

//          if (3 == split_vector_1.size()) {
//            //model.triangle_data.emplace_back(v_idx[0], v_idx[1], v_idx[2]);
//          } else {
//            //model.quadangle_data.emplace_back(v_idx[0], v_idx[1], v_idx[2], v_idx[3]);
//          }
//        }
//      }

//    } catch (std::exception& e) {
//      std::cerr << "Line not recognized: " << line << " (" << e.what() << ")" << std::endl;
//    }
//  }


//void model_loader::get_extrema(const std::vector<vertex>& vertex_data, glm::vec3& minimum, glm::vec3& maximum) {
//    const float min_float = std::numeric_limits<float>::lowest();
//    const float max_float = std::numeric_limits<float>::max();

//    glm::vec4 temp_min = glm::vec4(max_float, max_float, max_float, 1.0f);
//    glm::vec4 temp_max = glm::vec4(min_float, min_float, min_float, 1.0f);

//    for (const auto& vertex: vertex_data) {
//      temp_min = glm::min(temp_min, vertex);
//      temp_max = glm::max(temp_max, vertex);
//    }

//    minimum = glm::vec3(temp_min.x, temp_min.y, temp_min.z);
//    maximum = glm::vec3(temp_max.x, temp_max.y, temp_max.z);
//}


//void model_loader::transform_vertices(const glm::vec3& minimum, const glm::vec3& maximum, std::vector<vertex>& vertex_data) {
//    const glm::ivec3 triangle_transformation(-1, -1, -1);
//    const glm::ivec4 quadangle_transformation(-1, -1, -1, -1);

//    const glm::vec4 temp_min = glm::vec4(minimum, 1.0f);


//    for (auto& vertex: vertex_data) {
//        vertex -= temp_min;
//    }

//    const glm::vec4 temp_max = glm::vec4(maximum, 1.0f) - temp_min;
//    const float max_dimension = glm::max(temp_max.x, glm::max(temp_max.y, temp_max.z));

//    for (auto& vertex: vertex_data) {
//        vertex /= max_dimension;
//    }

////    for (auto& quadangle: model.quadangle_data) {
////    quadangle += quadangle_transformation;
////    }
//}



//void
//model_loader::get_center_of_mass(raw_model_data& model) {
//  const double one_over_n = 1.0 / model.vertex_data.size();

//  vertex center_of_mass(0.0f, 0.0f, 0.0f, 0.0f);

//  for (const auto& vertex: model.vertex_data) {
//    center_of_mass += vertex * one_over_n;
//  }

//  model.center_of_mass = center_of_mass;
//}

//        else if ("vn" == type && 4 <= split_vector_1.size()) {
//          auto vn1 = stof(split_vector_1[1]);
//          auto vn2 = stof(split_vector_1[2]);
//          auto vn3 = stof(split_vector_1[3]);

//          //vertex_normal_data.emplace_back(vn1, vn2, vn3, 1.0f);
//
//        } else if ("g" == type && 2 <= split_vector_1.size()) {
//          //groups.insert(next(begin(split_vector_1)), end(split_vector_1));
//        } else if ("o" == type && 2 == split_vector_1.size()) {
////          object_name = split_vector_1[1];
//        } else if ("usemtl" == type && 2 == split_vector_1.size()) {
////          material_name = split_vector_1[1];
//        }


