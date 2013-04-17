#include "bsp_map_loader.hpp"

#include <fstream>
#include <iostream>

template<typename T>
void load_lump(std::ifstream& file_stream, const size_t index, const bsp::direntry* const direntries, std::vector<T>& container) {
    const bsp::direntry& direntry = direntries[index];

    container.resize(direntry.length / sizeof(T));

    file_stream.seekg(direntry.offset);
    file_stream.read(reinterpret_cast<char*>(container.data()), direntry.length);
}

void bsp::load_map(const std::string path, map &map) {
    std::ifstream file_stream(path, std::ifstream::binary);
    std::vector<char> buffer(2048);

    bsp::header header;

    if (!file_stream) {
        return;
    }

    file_stream.read(reinterpret_cast<char*>(&header), sizeof(bsp::header));

    map.entities.ents.reset(new char[header.direntries[0].length]);

    file_stream.seekg(header.direntries[0].offset);
    file_stream.read(reinterpret_cast<char*>(map.entities.ents.get()), header.direntries[0].length);

    load_lump(file_stream, 1, header.direntries, map.textures);
    load_lump(file_stream, 2, header.direntries, map.planes);
    load_lump(file_stream, 3, header.direntries, map.nodes);
    load_lump(file_stream, 4, header.direntries, map.leafs);
    load_lump(file_stream, 5, header.direntries, map.leaffaces);
    load_lump(file_stream, 6, header.direntries, map.leafbrushes);
    load_lump(file_stream, 7, header.direntries, map.models);
    load_lump(file_stream, 8, header.direntries, map.brushes);
    load_lump(file_stream, 9, header.direntries, map.brushsides);
    load_lump(file_stream, 10, header.direntries, map.vertexes);
    load_lump(file_stream, 11, header.direntries, map.meshverts);
    load_lump(file_stream, 12, header.direntries, map.effects);
    load_lump(file_stream, 13, header.direntries, map.faces);
    load_lump(file_stream, 14, header.direntries, map.lightmaps);
    load_lump(file_stream, 15, header.direntries, map.lightvols);

    file_stream.close();
}
