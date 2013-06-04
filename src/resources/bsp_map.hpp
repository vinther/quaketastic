#ifndef BSP_MAP_HPP
#define BSP_MAP_HPP

#include <vector>

#include "bsp.hpp"

namespace qts {
namespace bsp {

struct map {
	entities map_entities;

	std::vector<texture> textures;
	std::vector<plane> planes;
	std::vector<node> nodes;
	std::vector<leaf> leafs;
	std::vector<leafface> leaffaces;
	std::vector<leafbrush> leafbrushes;
	std::vector<model> models;
	std::vector<brush> brushes;
	std::vector<brushside> brushsides;
	std::vector<vertex> vertexes;
	std::vector<meshvert> meshverts;
	std::vector<effect> effects;
	std::vector<face> faces;
	std::vector<lightmap> lightmaps;
	std::vector<lightvol> lightvols;
	std::vector<visdata> visdatas;
};

}
}

#endif // BSP_MAP_HPP
