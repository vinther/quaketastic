#ifndef STATIC_OBJECT_HPP
#define STATIC_OBJECT_HPP

#include <iostream>

#include "../typedefs.hpp"
#include "../resources/mesh.hpp"
#include "../resources/mesh_loader.hpp"
#include "../resources/resource_cache.hpp"

namespace qts {
    struct static_object {
    	template <typename T>
        static_object(std::string object_path, resource_cache<T>& cache)
        	: object_mesh(cache.get_resource(object_path))
        	, transform(glm::mat4(1.0f))
        {
        }

        ~static_object() {
        }

      const mesh& object_mesh;
      glm::mat4 transform;
    };
}

#endif // STATIC_OBJECT_HPP
