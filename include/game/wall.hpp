#pragma once
#include "game/entity.hpp"
#include <string>

class Wall: public Entity {
    private:
    int id;                 
    double point_x;        
    double point_y;                  
    bool collisions;   
    std::string texture;        
};
