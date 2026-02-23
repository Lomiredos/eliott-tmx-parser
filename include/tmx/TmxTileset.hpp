#pragma once
#include <string>



namespace ee::tmx {


    struct TmxTileset {
 

    int m_firstGid;
    std::string m_path;

    int m_tileWidth;
    int m_tileHeight;
    int m_columns;
 
    };
}