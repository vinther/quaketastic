#ifndef BSP_MAP_LOADER_HPP
#define BSP_MAP_LOADER_HPP

#include <string>

#include "bsp.hpp"
#include "bsp_map.hpp"

namespace bsp {
    void load_map(const std::string path, map& map);
}

#endif // BSP_MAP_LOADER_HPP
