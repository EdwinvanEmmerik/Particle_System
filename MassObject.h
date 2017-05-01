#ifndef MASSOBJECT_H_INCLUDED
#define MASSOBJECT_H_INCLUDED
#include <SFML/Graphics.hpp>

using namespace std;

/** \class MassObject
    \brief This class contains all the information to spawn one or more massobject(s).
*/
class MassObject{
    public:
    MassObject(sf::Vector2f _p, const float _m, const float _gf){
        position = _p;
        mass = _m;
        gravity_field = _gf;
    }
    /** \brief This function makes the massobject movable.
        * For example when you have programmed a solar system and you want to give the planets actual
            interaction with the particles that could me moving along the solar system.
        \param position
        \param deltaPos
    */
    void move_massObject(sf::Vector2f deltaPos){
        position +=deltaPos;    /**< Adjusting the position vector */
    }
    sf::Vector2f position;      /**< Position of the massobject */
    float gravity_field;        /**< The exponent on how strong the gravity field pulls onto the particle */
    float max_gravity_force;    /**< Maximum value the massobject can pull onto the particle */
    float mass;                 /**< Mass of the massobject */
};
#endif // MASSOBJECT_H_INCLUDED
