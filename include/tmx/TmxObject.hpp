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

        float m_rotation = 0.0f; // degres (Tiled) ; pivote autour du coin (x,y)


    };


    struct TmxObjectGroup {
        int m_id;
        std::string m_name;

        std::vector<TmxObject> m_objects;
    };
}