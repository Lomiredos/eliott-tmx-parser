#pragma once
#include <vector>

#include "TmxLayer.hpp"
#include "TmxObject.hpp"
#include "TmxTileset.hpp"

namespace ee::tmx {


    struct TmxMap {


        int m_width;
        int m_height;

        int m_tileWidth;
        int m_tileHeight;

        std::vector<TmxLayer> m_layers;
        std::vector<TmxObjectGroup> m_objectGroup;
        std::vector<TmxTileset> m_tileset;

    };
}