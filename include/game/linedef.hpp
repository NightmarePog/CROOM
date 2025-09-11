#pragma once
#include <string>

/**
 * @brief Represents a wall segment.
 *
 * A Linedef defines the geometry, sector associations, textures, 
 * and collision properties of a wall between two vertices.
 */
struct Linedef {
    int id;                 
    ///< Unique identifier for this wall segment.

    int vertexStart;        
    ///< Index of the starting vertex in the global vertex array.

    int vertexEnd;          
    ///< Index of the ending vertex in the global vertex array.

    int frontSector;        
    ///< Index of the sector in front of the wall (the side facing the player).
    
    int backSector;         
    ///< Index of the sector behind the wall. Set to -1 if there is no back sector.

    std::string textureTop;     
    ///< Texture applied to the upper portion of the wall.
    
    std::string textureMiddle;  
    ///< Texture applied to the middle portion of the wall (main wall surface).
    
    std::string textureBottom;  
    ///< Texture applied to the lower portion of the wall.

    bool blocking;          
    ///< True if the wall blocks movement (collision), false for passable walls like doors.
};
