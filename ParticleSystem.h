#ifndef PARTICLESYSTEM_H_INCLUDED
#define PARTICLESYSTEM_H_INCLUDED

#include <iostream>
#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>
#include "MassObject.h"

#define PI 3.14159265356
#define euler 2.7182818284590452353602875
#define sim_speed 0.000005
#define max_grav_force 100000
#define Gconst 1
#define height 600
#define width 800
/** \class ParticleSystem
    \brief This class contains all the information one needs to spawn one or more
            clusters of particles around one or more mass objects.
    * Creating instance of ParticleSystem by using:
        ParticleSystem particles(max_particles);
*/
class ParticleSystem : public sf::Drawable, public sf::Transformable{
public:
    //! default constructor
    /*!                             */
    ParticleSystem(unsigned int count) :
        m_particles(count),
        m_vertices(sf::Points, count),
        m_emitter(0, 0)
        {
        }
    //Add to privates:
    vector<MassObject> massobject;
    void create_massobject(const sf::Vector2f position, const float mass, const float gravity_field);
    void setEmitter(sf::Vector2f position);
    void update();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

    void get_radius_to_youranus(const unsigned i);          /**< Function that calculates the radius to a massobject */
    void get_force(const unsigned i);                       /**< Function that calculates the force of a massobject to a particle */
    void get_gravitational_pull(const unsigned i);          /**< Function that calculates the gravitational-pull of the massobject to a particle */
    void get_gravitational_angle(const unsigned i);         /**< Function that calculates the angle of the gravitational-pull of the massobject to a particle */
    void get_gravity_vector(const unsigned i);              /**< Function that calculates the gravity-vector from a massobject to a particle */
    void get_new_velocity(const unsigned i);                /**< Function that calculates the new velocity of the particle */
    void change_velocity(const unsigned i);                 /**< Function that stores the velocity to the new velocity */
    void update_position(const unsigned i);                 /**< Function that updates the position of a particle */
    void update_color(const unsigned i);                    /**< Function that updates the color of a particle */
    void create_particle_clusters(const unsigned cluster_count, const float compact,
                                  const float startvelocity, unsigned int spawnradius,
                                  float velocity_fase_offset, const unsigned s_velocity_type);  /**< Function that creates cluster of particles */
private:
    void resetParticle(std::size_t index);
    class Particle{
        /** \class Particle
            \brief This class contains all the information needed to spawn an particle.
            \relates ParticleSystem
        */
    public:
        void addMassobjectVector();
        sf::Vector2f velocity;              /**< The velocity vector of the particle */
        sf::Vector2f newvelocity;           /**< The new velocity vector of the particle */
        vector<sf::Vector2f> gravitypull;   /**< The gravity-pull vector from the massobjects */
        vector<float> gravityforce;         /**< The gravity-force vector from the massobjects */
        vector<float> gravityangle;         /**< The angle the gravity-force vector makes with the massobjects */
        vector<float> force;                /**< The force that the massobject pulls onto the particle */
        const float mass = 20;              /**< The mass of the particle */
        vector<float> radiustoyouranus;     /**< The radius from the particle to the massobjets */
        };
    vector<Particle> m_particles;           /**< The vector of particles of the ParticleSystem */
    sf::VertexArray m_vertices;             /**< The vertex array of the ParticleSystem */
    sf::Vector2f m_emitter;                 /**< The emitter(spawner) of the ParticleSystem */
};
#endif // PARTICLESYSTEM_H_INCLUDED
