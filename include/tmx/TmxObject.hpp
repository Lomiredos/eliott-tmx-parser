#pragma once
#include <string>
#include <vector>


namespace ee::tmx {

    struct TmxObject {


        int m_id;
        std::string m_name;
        float m_x;
        float m_y;
    
        float m_width;
        float m_height;
    
    
    };


    struct TmxObjectGroup {
        int m_id;
        std::string m_name;

        std::vector<TmxObject> m_objects;
    };
}