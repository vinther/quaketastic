#ifndef BSP_MAP_HPP
#define BSP_MAP_HPP

#include <vector>

#include "bsp.hpp"

namespace bsp {
    struct map {
        bsp::entities entities;

        std::vector<bsp::texture> textures;
        std::vector<bsp::plane> planes;
        std::vector<bsp::node> nodes;
        std::vector<bsp::leaf> leafs;
        std::vector<bsp::leafface> leaffaces;
        std::vector<bsp::leafbrush> leafbrushes;
        std::vector<bsp::model> models;
        std::vector<bsp::brush> brushes;
        std::vector<bsp::brushside> brushsides;
        std::vector<bsp::vertex> vertexes;
        std::vector<bsp::meshvert> meshverts;
        std::vector<bsp::effect> effects;
        std::vector<bsp::face> faces;
        std::vector<bsp::lightmap> lightmaps;
        std::vector<bsp::lightvol> lightvols;
        std::vector<bsp::visdata> visdata;
    };
}

#endif // BSP_MAP_HPP
