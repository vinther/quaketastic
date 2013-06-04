#ifndef BSP_MAP_LOADER_HPP
#define BSP_MAP_LOADER_HPP

#include <string>

#include "bsp.hpp"
#include "bsp_map.hpp"

namespace qts {

struct bsp_map_loader{
	typedef bsp::map type;

	bsp::map load(const std::string path);
};

}

#endif // BSP_MAP_LOADER_HPP
