#ifndef PARTICLESYSTEM_H_INCLUDED
#define PARTICLESYSTEM_H_INCLUDED
//! \class ParticleSystem
/*! \brief This class contains all the information one needs to spawn one or more
            clusters of particles around one or more mass objects.

    * Creating instance of ParticleSystem class
        ParticleSystem particles(max_particles);
    * Creating Mass Object
        particles.create_massobject(posx, posy, mass, gravity_field);
    * Creating Particle Clusters
        particles.create_particle_clusters(clustercount, compact, startvelocity, spawnradius, phase_offset_velocity, velocity_setting);
        velocity_setting: 0 = radial, 1 = radial mirrored, 2 = linear
*/
#include "MassObject.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <SFML/Graphics.hpp>


#define viewheight 4000
#define viewwidth 3000

#define PI 3.14159265356
#define euler 2.7182818284590452353602875
#define sim_speed 0.0000025
#define max_grav_force 100000
#define Gconst 1
#define height 600
#define width 800
class ParticleSystem : public sf::Drawable, public sf::Transformable{
public:
    //! default constructor
    /*!                             */
    ParticleSystem(unsigned int count) :
        m_particles(count),
        m_vertices(sf::Points, count),
        m_lifetime(sf::seconds(3)),
        m_emitter(0, 0)
        {
        }
    //Add to privates:
    vector<MassObject> massobject;
    void create_massobject(const sf::Vector2f position, const float mass, const float gravity_field);
    void setEmitter(sf::Vector2f position);
    void update();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const;

    void get_radius_to_youranus(const unsigned i);
    void get_force(const unsigned i);
    void get_gravitational_pull(const unsigned i);
    void get_gravitational_angle(const unsigned i);
    void get_gravity_vector(const unsigned i);
    void get_new_velocity(const unsigned i);
    void change_velocity(const unsigned i);
    void update_position(const unsigned i);
    void update_color(const unsigned i);
    void create_particle_clusters(const unsigned cluster_count, const float compact, const float startVelocity, unsigned int spawnradius, float velocity_fase_offset, const unsigned s_velocity_type);
    ///This function needs to set cluster location and relative particle speeds
private:
    void resetParticle(std::size_t index);
    class Particle{
        //! \class Particle
        /*! \brief This class contains all the information needed to spawn an particle.
        */
    public:
        void addMassobjectVector();
        sf::Vector2f velocity;
        sf::Vector2f newvelocity;
        vector<sf::Vector2f> gravitypull;
        vector<float> gravityforce;
        vector<float> gravityangle;
        vector<float> force;
        const float mass = 20;
        vector<float> radiustoyouranus;
        sf::Time lifetime;
    };
    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    sf::Time m_lifetime;
    sf::Vector2f m_emitter;
};
#endif // PARTICLESYSTEM_H_INCLUDED
