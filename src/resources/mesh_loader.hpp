#ifndef MESH_LOADER_HPP
#define MESH_LOADER_HPP

#include "mesh.hpp"

#include <vector>
#include <string>

namespace qts {

struct mesh_loader {
	typedef mesh type;

    mesh load(const std::string& path);
};

}

#endif /* MESH_LOADER_HPP */
