#include "tmx/TmxParser.hpp"
#include <filesystem>
#include <sstream>
#include <iostream>

using namespace ee::tmx;

static std::vector<int> parseCSV(std::string entry)
{
    std::vector<int> result;
    std::stringstream ss(entry);
    std::string token;
    while (std::getline(ss, token, ','))
    {
        if (token.empty())
            continue;

        bool hasDigit = false;
        for (char c : token)
        {
            if (std::isdigit(c))
            {
                hasDigit = true;
                break;
            }
        }

        if (hasDigit)
        {

            try
            {
                int value = std::stoi(token);
                result.push_back(value);
            }
            catch (const std::exception &e)
            {
                std::cerr << "[TiledParser] error parsing number:" << token << "\n";
            }
        }
    }

    return result;
}

std::optional<TmxMap> ee::tmx::TmxParser::load(const std::string &_path)
{
    // on prend le parent l'extension
    std::filesystem::path basePath = std::filesystem::path(_path).parent_path();

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(_path.c_str()) != tinyxml2::XML_SUCCESS)
        return std::nullopt;

    tinyxml2::XMLElement *map = doc.FirstChildElement("map");

    if (!map)
        return std::nullopt;
    TmxMap tmxMap;
    tmxMap.m_height = map->IntAttribute("height");
    tmxMap.m_width = map->IntAttribute("width");
    tmxMap.m_tileHeight = map->IntAttribute("tileheight");
    tmxMap.m_tileWidth = map->IntAttribute("tilewidth");

    // on gere les tileset
    tinyxml2::XMLElement *currentTileset = map->FirstChildElement("tileset");
    while (currentTileset)
    {
        auto tileset = loadTileSet(basePath.string(), currentTileset);
        if (tileset)
            tmxMap.m_tileset.push_back(*tileset);
        currentTileset = currentTileset->NextSiblingElement("tileset");
    }

    // on gere les layers (c'est pareil)
    tinyxml2::XMLElement *currentLayer = map->FirstChildElement("layer");
    while (currentLayer)
    {
        auto layer = loadLayer(currentLayer);
        if (layer)
            tmxMap.m_layers.push_back(*layer);
        currentLayer = currentLayer->NextSiblingElement("layer");
    }

    // obj groupe (pareil)
    tinyxml2::XMLElement *currentObjectGroup = map->FirstChildElement("objectgroup");
    while (currentObjectGroup)
    {
        TmxObjectGroup objGroup = loadObjectGroup(currentObjectGroup);
        
        tmxMap.m_objectGroup.push_back(objGroup);
        currentObjectGroup = currentObjectGroup->NextSiblingElement("objectgroup");
    }

    return tmxMap;
}

std::optional<TmxTileset> ee::tmx::TmxParser::loadTileSet(const std::string &_path, tinyxml2::XMLElement *_elem)
{
    TmxTileset tileset;
    tileset.m_firstGid = _elem->IntAttribute("firstgid");
    if (const char *tsxSource = _elem->Attribute("source"))
    {

        std::string fullPath = (std::filesystem::path(_path) / tsxSource).string();
        tinyxml2::XMLDocument tsxDoc;

        if (tsxDoc.LoadFile(fullPath.c_str()) != tinyxml2::XML_SUCCESS)
            return std::nullopt;

        tinyxml2::XMLElement *tsxTileset = tsxDoc.FirstChildElement("tileset");
        if (!tsxTileset)
            return std::nullopt;

        tileset.m_tileWidth = tsxTileset->IntAttribute("tilewidth");
        tileset.m_tileHeight = tsxTileset->IntAttribute("tileheight");
        tileset.m_columns = tsxTileset->IntAttribute("columns");

        tinyxml2::XMLElement *image = tsxTileset->FirstChildElement("image");
        if (!image)
            return tileset;

        const char *imageSource = image->Attribute("source");
        if (!imageSource)
            return tileset;

        tileset.m_path = (std::filesystem::path(_path) / imageSource).string();
    }
    else
    {

        tileset.m_tileWidth = _elem->IntAttribute("tilewidth");
        tileset.m_tileHeight = _elem->IntAttribute("tileheight");
        tileset.m_columns = _elem->IntAttribute("columns");

        tinyxml2::XMLElement *image = _elem->FirstChildElement("image");
        if (!image)
            return std::nullopt;

        const char *imageSource = image->Attribute("source");
        if (!imageSource)
            return std::nullopt;

        tileset.m_path = (std::filesystem::path(_path) / imageSource).string();
    }

    return tileset;
}

std::optional<TmxLayer> ee::tmx::TmxParser::loadLayer(tinyxml2::XMLElement *_elem)
{
    TmxLayer layer;

    if (const char *name = _elem->Attribute("name"))
        layer.m_name = name;

    layer.m_width = _elem->IntAttribute("width");
    layer.m_height = _elem->IntAttribute("height");
    layer.m_id = _elem->IntAttribute("id");

    tinyxml2::XMLElement *data = _elem->FirstChildElement("data");
    if (!data)
        return std::nullopt;

    const char *encoding = data->Attribute("encoding");
    if (!encoding || std::string(encoding) != "csv")
        return std::nullopt;

    std::string intData = data->GetText();

    layer.m_tiles = parseCSV(intData);

    if (layer.m_tiles.size() == 0)
        return std::nullopt;

    return layer;
}

TmxObject ee::tmx::TmxParser::loadObject(tinyxml2::XMLElement *_elem)
{

    TmxObject obj;
    if (_elem->Attribute("name"))
        obj.m_name = _elem->Attribute("name");
    obj.m_x = _elem->FloatAttribute("x");
    obj.m_y = _elem->FloatAttribute("y");
    obj.m_width = _elem->FloatAttribute("width");
    obj.m_height = _elem->FloatAttribute("height");
    obj.m_id = _elem->IntAttribute("id");


    return obj;
}

TmxObjectGroup ee::tmx::TmxParser::loadObjectGroup(tinyxml2::XMLElement *_elem)
{
    TmxObjectGroup objGroup;
    if (_elem->Attribute("name"))
        objGroup.m_name = _elem->Attribute("name");

    objGroup.m_id = _elem->IntAttribute("id");
    tinyxml2::XMLElement *obj = _elem->FirstChildElement("object");
    while (obj)
    {
        TmxObject object = loadObject(obj);
        objGroup.m_objects.push_back(object);
        obj = obj->NextSiblingElement("object");
    }

    return objGroup;
}
