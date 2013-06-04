#ifndef MESH_HPP_
#define MESH_HPP_

#include <utility>
#include <vector>
#include <string>

#include "../typedefs.hpp"

namespace qts {

struct mesh {
	mesh()
	{
	}

	mesh (mesh&& other)
        : vertices(std::move(other.vertices))
		, vertex_normals(std::move(other.vertex_normals))
        , triangles(std::move(other.triangles))
    {
    }

	mesh& operator=(const mesh& other) = default;

	std::vector<vertex> vertices;
    std::vector<vertex_normal> vertex_normals;
    std::vector<unsigned int> triangles;

    size_t hash;
};

}


#endif /* MESH_HPP_ */
