#pragma once
#include <string>
#include <vector>


namespace ee::tmx {


    struct TmxLayer {
 
        int m_id;

        int m_width;
        int m_height;


        std::string m_name;
        std::vector<int> m_tiles;

    };
}