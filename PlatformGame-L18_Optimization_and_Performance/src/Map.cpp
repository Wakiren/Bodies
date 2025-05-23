
#include "Engine.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Log.h"
#include "Physics.h"

#include <math.h>
#include "tracy/Tracy.hpp"

using namespace std;

Map::Map() : Module(), mapLoaded(false)
{
    name = "map";
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake()
{
    name = "map";
    LOG("Loading Map Parser");

    return true;
}

bool Map::Start() {

    return true;
}

bool Map::Update(float dt)
{
    ZoneScoped;
    // Code you want to profile

    bool ret = true;

    if (mapLoaded) {

        //First lets lode the images for the BG
        for (const auto& Image : mapData.bgImages) {
            if (Image->properties.GetProperty("Draw") != NULL && Image->properties.GetProperty("Draw")->value == true)
            {
                Engine::GetInstance().render->DrawTexture(Image->texture, 0, 0);
            }
        }
        // L07 TODO 5: Prepare the loop to draw all tiles in a layer + DrawTexture()
        // iterate all tiles in a layer
        for (const auto& mapLayer : mapData.layers) {
            //Check if the property Draw exist get the value, if it's true draw the lawyer
            if (mapLayer->properties.GetProperty("Draw") != NULL && mapLayer->properties.GetProperty("Draw")->value == true) {

                Vector2D camPos = Vector2D(Engine::GetInstance().render->camera.x * -1 / 4, Engine::GetInstance().render->camera.y * -1 / 4 );
                if (camPos.getX() < 0) camPos.setX(0);
                if (camPos.getY() < 0) camPos.setY(0);
                Vector2D camPosTile = WorldToMap(camPos.getX(), camPos.getY());

                Vector2D camSize = Vector2D(Engine::GetInstance().render->camera.w, Engine::GetInstance().render->camera.h);
                Vector2D camSizeTile = WorldToMap(camSize.getX(), camSize.getY());

                Vector2D limits = Vector2D(camPosTile.getX() + camSizeTile.getX(), camPosTile.getY() + camSizeTile.getY());
                if (limits.getX() > mapData.width) limits.setX(mapData.width);
                if (limits.getY() > mapData.height) limits.setY(mapData.height);

                for (int i = camPosTile.getX(); i < limits.getX(); i++) {
                    for (int j = camPosTile.getY(); j < limits.getY(); j++) {

                        // L07 TODO 9: Complete the draw function

                        //Get the gid from tile
                        int gid = mapLayer->Get(i, j);
                        //Check if the gid is different from 0 - some tiles are empty
                        if (gid != 0) {
                            //L09: TODO 3: Obtain the tile set using GetTilesetFromTileId
                            TileSet* tileSet = GetTilesetFromTileId(gid);
                            if (tileSet != nullptr) {
                                //Get the Rect from the tileSetTexture;
                                SDL_Rect tileRect = tileSet->GetRect(gid);
                                //Get the screen coordinates from the tile coordinates
                                Vector2D mapCoord = MapToWorld(i, j);
                                //Draw the texture
                                Engine::GetInstance().render->DrawTexture(tileSet->texture, mapCoord.getX(), mapCoord.getY(), &tileRect);
                            }
                        }
                    }
                }
            }
        }

    }

    return ret;
}


// L09: TODO 2: Implement function to the Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
	TileSet* set = nullptr;

    for (const auto& tileset : mapData.tilesets) {
    	if (gid >= tileset->firstGid && gid < (tileset->firstGid + tileset->tileCount)) {
			set = tileset;
			break;
		}
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L06: TODO 2: Make sure you clean up any memory allocated from tilesets/map
    for (const auto& tileset : mapData.tilesets) {
        delete tileset;
    }
    mapData.tilesets.clear();

    // L07 TODO 2: clean up all layer data
    for (const auto& layer : mapData.layers)
    {
        delete layer;
    }
    mapData.layers.clear();

    return true;
}

// Load new map
bool Map::Load(std::string path, std::string fileName)
{
    bool ret = false;

    // Assigns the name of the map file and the path
    mapFileName = fileName;
    mapPath = path;
    std::string mapPathName = mapPath + mapFileName;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapPathName.c_str());

    if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", mapPathName.c_str(), result.description());
		ret = false;
    }
    else {

        // L06: TODO 3: Implement LoadMap to load the map properties
        // retrieve the paremeters of the <map> node and store the into the mapData struct
        mapData.width = mapFileXML.child("map").attribute("width").as_int();
        mapData.height = mapFileXML.child("map").attribute("height").as_int();
        mapData.tileWidth = mapFileXML.child("map").attribute("tilewidth").as_int();
        mapData.tileHeight = mapFileXML.child("map").attribute("tileheight").as_int();

        // L10: TODO 2: Define a property to store the MapType and Load it from the map
        std::string orientationStr = mapFileXML.child("map").attribute("orientation").as_string();
        if (orientationStr == "orthogonal") {
            mapData.orientation = MapOrientation::ORTOGRAPHIC;
        }
        else if (orientationStr == "isometric") {
            mapData.orientation = MapOrientation::ISOMETRIC;
        }
        else {
            LOG("Map orientation not found");
            ret = false;
        }

        // L06: TODO 4: Implement the LoadTileSet function to load the tileset properties
       
        //Iterate the Tileset
        for(pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode!=NULL; tilesetNode = tilesetNode.next_sibling("tileset"))
		{
            //Load Tileset attributes
			TileSet* tileSet = new TileSet();
            tileSet->firstGid = tilesetNode.attribute("firstgid").as_int();
            tileSet->name = tilesetNode.attribute("name").as_string();
            tileSet->tileWidth = tilesetNode.attribute("tilewidth").as_int();
            tileSet->tileHeight = tilesetNode.attribute("tileheight").as_int();
            tileSet->spacing = tilesetNode.attribute("spacing").as_int();
            tileSet->margin = tilesetNode.attribute("margin").as_int();
            tileSet->tileCount = tilesetNode.attribute("tilecount").as_int();
            tileSet->columns = tilesetNode.attribute("columns").as_int();

			//Load the tileset image
			std::string imgName = tilesetNode.child("image").attribute("source").as_string();
            tileSet->texture = Engine::GetInstance().textures->Load((mapPath+imgName).c_str());

			mapData.tilesets.push_back(tileSet);
		}

        // L07: TODO 3: Iterate all layers in the TMX and load each of them
        for (pugi::xml_node layerNode = mapFileXML.child("map").child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {

            // L07: TODO 4: Implement the load of a single layer 
            //Load the attributes and saved in a new MapLayer
            MapLayer* mapLayer = new MapLayer();
            mapLayer->id = layerNode.attribute("id").as_int();
            mapLayer->name = layerNode.attribute("name").as_string();
            mapLayer->width = layerNode.attribute("width").as_int();
            mapLayer->height = layerNode.attribute("height").as_int();

            //L09: TODO 6 Call Load Layer Properties
            LoadProperties(layerNode, mapLayer->properties);

            //Iterate over all the tiles and assign the values in the data array
            for (pugi::xml_node tileNode = layerNode.child("data").child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {
                mapLayer->tiles.push_back(tileNode.attribute("gid").as_int());
            }

            //add the layer to the map
            mapData.layers.push_back(mapLayer);
        }

        //Get the BG Images
        for (pugi::xml_node ImageNode = mapFileXML.child("map").child("imagelayer"); ImageNode != NULL; ImageNode = ImageNode.next_sibling("imagelayer")) {

            BGimage* Image = new BGimage();
            Image->id = ImageNode.attribute("id").as_int();
            Image->name = ImageNode.attribute("name").as_string();
            Image->width = ImageNode.child("image").attribute("width").as_int();
            Image->height = ImageNode.child("image").attribute("height").as_int();
			Image->source = ImageNode.child("image").attribute("source").as_string();

            //L09: TODO 6 Call Load Layer Properties
            LoadProperties(ImageNode, Image->properties);

			Image->texture = Engine::GetInstance().textures->Load((mapPath + Image->source).c_str());

            //add the layer to the map
            mapData.bgImages.push_back(Image);
        }

		for (pugi::xml_node objectLayerNode = mapFileXML.child("map").child("objectgroup"); objectLayerNode != NULL; objectLayerNode = objectLayerNode.next_sibling("objectgroup")) {
			MapObjectLayer* objectLayer = new MapObjectLayer();
			objectLayer->id = objectLayerNode.attribute("id").as_int();
			objectLayer->name = objectLayerNode.attribute("name").as_string();
			//L09: TODO 6 Call Load Layer Properties
			LoadProperties(objectLayerNode, objectLayer->properties);
			for (pugi::xml_node objectNode = objectLayerNode.child("object"); objectNode != NULL; objectNode = objectNode.next_sibling("object")) {
				Object* object = new Object();
				object->id = objectNode.attribute("id").as_int();
				object->x = objectNode.attribute("x").as_float();
				object->y = objectNode.attribute("y").as_float();
				object->width = objectNode.attribute("width").as_float();
				object->height = objectNode.attribute("height").as_float();
				objectLayer->object_list.push_back(*object);
			}
            for (pugi::xml_node objectNode = objectLayerNode.child("object").child("polygon"); objectNode != NULL; objectNode = objectLayerNode.child("object").next_sibling("polygon")) {
				std::string pointsStr = objectNode.attribute("points").as_string();
				objectLayer->points = ConvertStringToPoints(pointsStr);

                for (int i = 0; i < objectLayer->points.size(); i++)
                {
                    objectLayer->points[i].setX(objectLayer->points[i].getX() + objectNode.parent().attribute("x").as_int());
                    objectLayer->points[i].setY(objectLayer->points[i].getY() + objectNode.parent().attribute("y").as_int());
                }
            }

			mapData.objectLayers.push_back(objectLayer);
		}

        // L08 TODO 3: Create colliders
        // L08 TODO 7: Assign collider type
        // Later you can create a function here to load and create the colliders from the map

        for (const auto& mapObjectGroup : mapData.objectLayers) {
            if (mapObjectGroup->properties.GetProperty("Collisions_") != NULL && mapObjectGroup->properties.GetProperty("Collisions_")->value == true) {
                for (const auto& mapObject : mapObjectGroup->object_list) {
                    /*LOG("pos.x = %d, pos.y = %d, width = %d, height = %d", mapObject._x, mapObject._y, mapObject.width, mapObject.height);*/
                    PhysBody* collider = Engine::GetInstance().physics.get()->CreateRectangle(mapObject.x + mapObject.width / 2, mapObject.y + mapObject.height / 2, mapObject.width - 1, mapObject.height, STATIC);
                    collider->ctype = ColliderType::PLATFORM;
                    Colliders.push_back(collider);
                }
            }
        }

        ret = true;

        // L06: TODO 5: LOG all the data loaded iterate all tilesetsand LOG everything
        if (ret == true)
        {
            LOG("Successfully parsed map XML file :%s", fileName.c_str());
            LOG("width : %d height : %d", mapData.width, mapData.height);
            LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

            LOG("Tilesets----");

            //iterate the tilesets
            for (const auto& tileset : mapData.tilesets) {
                LOG("name : %s firstgid : %d", tileset->name.c_str(), tileset->firstGid);
                LOG("tile width : %d tile height : %d", tileset->tileWidth, tileset->tileHeight);
                LOG("spacing : %d margin : %d", tileset->spacing, tileset->margin);
            }
            			
            LOG("Layers----");

            for (const auto& layer : mapData.layers) {
                LOG("id : %d name : %s", layer->id, layer->name.c_str());
				LOG("Layer width : %d Layer height : %d", layer->width, layer->height);
            }   
        }
        else {
            LOG("Error while parsing map file: %s", mapPathName.c_str());
        }

        if (mapFileXML) mapFileXML.reset();

    }

    mapLoaded = ret;
    return ret;
}

// L07: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
Vector2D Map::MapToWorld(int x, int y) const
{
    Vector2D ret;

    // L09: TODO 3: Get the screen coordinates of tile positions for isometric maps 
    if (mapData.orientation == MapOrientation::ORTOGRAPHIC) {
        ret.setX(x * mapData.tileWidth);
        ret.setY(y * mapData.tileHeight);
    }
	else if (mapData.orientation == MapOrientation::ISOMETRIC) {
        ret.setX(x * mapData.tileWidth / 2 - y * mapData.tileWidth / 2);
        ret.setY(x * mapData.tileHeight / 2 + y * mapData.tileHeight / 2);
    }

    return ret;
}

// L10: TODO 5: Add method WorldToMap to obtain  map coordinates from screen coordinates 
Vector2D Map::WorldToMap(int x, int y) {

    Vector2D ret(0, 0);

    if (mapData.orientation == MapOrientation::ORTOGRAPHIC) {
        ret.setX(x / mapData.tileWidth);
        ret.setY(y / mapData.tileHeight);
    }

    if (mapData.orientation == MapOrientation::ISOMETRIC) {
        float half_width = mapData.tileWidth / 2;
        float half_height = mapData.tileHeight / 2;
        ret.setX(int((x / half_width + y / half_height) / 2));
        ret.setY(int((y / half_height - (x / half_width)) / 2));
    }

    return ret;
}

// L09: TODO 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.propertyList.push_back(p);
    }

    return ret;
}

MapLayer* Map::GetNavigationLayer() {
    for (const auto& layer : mapData.layers) {
		if (layer->properties.GetProperty("Navigation") != NULL && 
            layer->properties.GetProperty("Navigation")->value) {
			return layer;
		}
	}

	return nullptr;
}

// L09: TODO 7: Implement a method to get the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
    for (const auto& property : propertyList) {
        if (property->name == name) {
			return property;
		}
    }

    return nullptr;
}

MapLayer* Map::GetDataLayer() {

    for (const auto& layer : mapData.layers) {
        if (layer->name == "MapDataUpper" != NULL &&
            layer->name == "MapDataUpper") {
            return layer;
        }
    }

    return nullptr;
}

MapLayer* Map::GetDataLayerUnder() {

    for (const auto& layer : mapData.layers) {
        if (layer->name == "MapDataUnder" != NULL &&
            layer->name == "MapDataUnder") {
            return layer;
        }
    }
    return nullptr;
}

void Map::SwapUnderUpper()
{
    for (const auto& layer : mapData.layers) {
		if (layer->properties.GetProperty("Draw") != nullptr)
            layer->properties.GetProperty("Draw")->value = !layer->properties.GetProperty("Draw")->value;
    }
    for (const auto& Image : mapData.bgImages) {
        if (Image->properties.GetProperty("Draw") != nullptr)
            Image->properties.GetProperty("Draw")->value = !Image->properties.GetProperty("Draw")->value;
    }
    for (const auto& Image : mapData.objectLayers) {
        if (Image->properties.GetProperty("Collisions_") != nullptr)
            Image->properties.GetProperty("Collisions_")->value = !Image->properties.GetProperty("Collisions_")->value;
    }

    for (int i = 0; i < Colliders.size(); i++)
    {
		if (Colliders[i]->ctype == ColliderType::PLATFORM) {
			Engine::GetInstance().physics.get()->DeletePhysBody(Colliders[i]);

		}
    }

	Colliders.clear();

    for (const auto& mapObjectGroup : mapData.objectLayers) {
        if (mapObjectGroup->properties.GetProperty("Collisions_") != NULL && mapObjectGroup->properties.GetProperty("Collisions_")->value == true) {
            for (const auto& mapObject : mapObjectGroup->object_list) {
                /*LOG("pos.x = %d, pos.y = %d, width = %d, height = %d", mapObject._x, mapObject._y, mapObject.width, mapObject.height);*/
                PhysBody* collider = Engine::GetInstance().physics.get()->CreateRectangle(mapObject.x + mapObject.width / 2, mapObject.y + mapObject.height / 2, mapObject.width - 1, mapObject.height, STATIC);
                collider->ctype = ColliderType::PLATFORM;
				Colliders.push_back(collider);
            }
        }
    }
}

MapObjectLayer* Map::GetNarPath() {
	for (const auto& objectLayer : mapData.objectLayers) {
		if (objectLayer->name == "NarPath" != NULL && objectLayer->name == "NarPath") {
			return objectLayer;
		}
	}
	return nullptr;
}

vector<Vector2D> Map:: ConvertStringToPoints(std::string pointsStr) {
    vector<Vector2D> points;
    size_t start = 0;
    size_t end = pointsStr.find(' ');

    // Loop through the string, processing each "x,y" pair
    while (end != string::npos) {
        // Extract the current "x,y" pair
        string point = pointsStr.substr(start, end - start);

        // Find the comma separating x and y
        size_t commaPos = point.find(',');
        if (commaPos != string::npos) {
            // Extract x and y from the point
            float x = stof(point.substr(0, commaPos));
            float y = stof(point.substr(commaPos + 1));

            // Add the point to the list
            points.push_back(Vector2D(x, y));
        }

        // Move to the next point
        start = end + 1;
        end = pointsStr.find(' ', start);
    }

    // Handle the last point (after the final space)
    if (start < pointsStr.length()) {
        string point = pointsStr.substr(start);
        size_t commaPos = point.find(',');
        if (commaPos != string::npos) {
            float x = stof(point.substr(0, commaPos));
            float y = stof(point.substr(commaPos + 1));
            points.push_back(Vector2D(x, y));
        }
    }

    return points;
}
