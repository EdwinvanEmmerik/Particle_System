#ifndef MASSOBJECT_H_INCLUDED
#define MASSOBJECT_H_INCLUDED
//! \class MassObject
/*! \brief This class contains all the information to spawn one or more massobject(s).
*/
#include <SFML/Graphics.hpp>
using namespace std;

class MassObject{
    public:
    MassObject(sf::Vector2f _p, const float _m, const float _gf){
        position = _p;
        mass = _m;
        gravity_field = _gf;
    }
    void move_massObject(sf::Vector2f deltaPos){
        position +=deltaPos;
    }
    sf::Vector2f position;
    float gravity_field;
    //Setup this parameter for use:
    float max_gravity_force;
    float mass;
};
#endif // MASSOBJECT_H_INCLUDED
