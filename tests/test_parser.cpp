#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "tmx/TmxParser.hpp"

TEST_CASE("On verifie sur load retourne bien qqchose")
{
    auto map = ee::tmx::TmxParser::load("tests/data/test.tmx");
    CHECK(map.has_value());
}

TEST_CASE("On verrifie si la map a les bonne dimensions")
{

    auto map = ee::tmx::TmxParser::load("tests/data/test.tmx");

    CHECK(map->m_width == 2);
    CHECK(map->m_height == 2);
}

TEST_CASE("On verifie si le nom des layer est bien recuperer")
{
    auto map = ee::tmx::TmxParser::load("tests/data/test.tmx");

    CHECK(map->m_layers.size() == 1); 
    CHECK(map->m_layers[0].m_name == "Ground");
}

TEST_CASE("On verifie si les data generale sont bien passer"){
    auto map = ee::tmx::TmxParser::load("tests/data/test.tmx");

    std::vector<int> tiles = {1, 2, 1, 2};
    CHECK(map->m_layers[0].m_tiles == tiles);
    CHECK(map->m_objectGroup.size() == 1);
    CHECK(map->m_objectGroup[0].m_objects[0].m_name == "Player");
}
