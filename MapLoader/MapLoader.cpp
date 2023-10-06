#include "MapLoader.h"
#include <fstream>
#include <sstream>
#include <map>

MapLoader::MapLoader(const std::string &filePath) {
    this->filePath = new std::string(filePath);
}

MapLoader::~MapLoader() {
    delete filePath;
}

Map* MapLoader::loadMap() {
    std::ifstream file(*filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << *filePath << std::endl;
        return nullptr;
    }

    Map* map = new Map();
    std::string line;
    std::map<std::string, Territory*> territoryMap;  // To lookup territories by name

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        if (line.find("[continents]") != std::string::npos) {
            while (getline(file, line) && !line.empty()) {
                std::stringstream ss(line);
                std::string name, value;
                getline(ss, name, '=');
                getline(ss, value, '=');
                Continent* continent = new Continent(name);
                map->addContinent(continent);
            }
        } else if (line.find("[territories]") != std::string::npos) {
            while (getline(file, line) && !line.empty()) {
                std::stringstream ss(line);
                std::string name, x_coord, y_coord, continent_name;
                getline(ss, name, ',');
                getline(ss, x_coord, ',');
                getline(ss, y_coord, ',');
                getline(ss, continent_name, ',');
                
                Territory* territory = new Territory(name);
                map->addTerritory(territory);
                territoryMap[name] = territory;  // Store territory for later adjacency setup
                
                // Assuming continent names are unique and have been read already
                for (auto continent : map->getContinents()) {
                    if (*(continent->getName()) == continent_name) {
                        continent->addTerritory(territory);
                        break;
                    }
                }
            }
        } else if (line.find("[adjacencies]") != std::string::npos) {
            while (getline(file, line) && !line.empty()) {
                std::stringstream ss(line);
                std::string name, adj_name;
                getline(ss, name, ',');
                Territory* territory = territoryMap[name];  // Assuming territory names are unique
                
                while (getline(ss, adj_name, ',')) {
                    Territory* adj_territory = territoryMap[adj_name];  // Assuming adj_name exists
                    territory->addAdjacency(adj_territory);
                }
            }
        }
    }

    file.close();
    return map;
}

// Implementations for MapLoader methods. Handle file reading and parsing here.
