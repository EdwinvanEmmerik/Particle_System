#include "ParticleSystem.h"
#include "Particles_defines.h"

/** \brief This function calculates the radius from a particle to every massobject.
    * Formula: radius = sqrt((Particle.x-Massobject.x)^2+(Particle.y-Massobject.y)^2)
    \param m_vertices[i].position.x
    \param m_vertices[i].position.y
    \param massobject[j].possition.x
    \param massobject[j].possition.y
    \return radiustoyouranus
*/
void ParticleSystem::get_radius_to_youranus(const unsigned i){
    for(unsigned j=0; j< massobject.size(); j++){
        m_particles[i].radiustoyouranus[j] = sqrt(pow((m_vertices[i].position.x-massobject[j].position.x), 2) + pow(((m_vertices[i].position.y-massobject[j].position.y)), 2));
    }
}
/** \brief This function calculates the magnitude of the force-vector every massobject pulls onto a particle.
    *   Formula: force = (Gconst * Particle.mass * Massobject.mass) /
                            (radiustoyouranus^gravity_field)
    \param m_particles[i].mass
    \param m_particles[i].radiustoyouranus
    \param massobject[j].mass
    \param massobject[j].gravity_field
    \return m_particles[i].force[j]
*/
void ParticleSystem::get_force(const unsigned i){
    for(unsigned j=0; j< massobject.size(); j++){
        m_particles[i].force[j] = ((Gconst * m_particles[i].mass * massobject[j].mass) / pow(m_particles[i].radiustoyouranus[j], massobject[j].gravity_field));
    }
}
/** \brief This function calculates the magnitude of the actual force-vector every massobject pulls onto a particle.
    * Formula: gravityforce = force / particle.mass
    \param m_particles[i].force
    \param m_particles[i].mass
    \return m_particles[i].gravityforce[j]
*/
void ParticleSystem::get_gravitational_pull(const unsigned i){
    for(unsigned j=0; j< massobject.size(); j++){
        m_particles[i].gravityforce[j] = (m_particles[i].force[j] / m_particles[i].mass);
    }
}
/** \brief This function calculates the angle of the force-vector from a particle to every massobject.
    * Formula: angle = atan2(Massobject.y - Particle.y) /
                                (Massobject.x - Particle.x)
    \param massobject[j].position.x
    \param massobject[j].position.y
    \param m_vertices[i].position.x
    \param m_vertices[i].position.y
    \return m_particles[i].gravityangle[j]
*/
void ParticleSystem::get_gravitational_angle(const unsigned i){
    for(unsigned j=0; j< massobject.size(); j++){
        m_particles[i].gravityangle[j] = atan2((massobject[j].position.y - (float)m_vertices[i].position.y),
                                               (massobject[j].position.x - (float)m_vertices[i].position.x));
    }
}
/** \brief This function calculates the gravitypull-vector from a particle to every massobject.
    \param m_particles[i].gravityangle[j]
    \param m_particles[i].gravityforce[j]
    \return m_particles[i].gravitypull[j]
*/
void ParticleSystem::get_gravity_vector(const unsigned i){
    for(unsigned j=0; j< massobject.size(); j++){
        m_particles[i].gravitypull[j] = sf::Vector2f(cos(m_particles[i].gravityangle[j]) * m_particles[i].gravityforce[j],
                                                     sin(m_particles[i].gravityangle[j]) * m_particles[i].gravityforce[j] );
    }
}
/** \brief This function makes sure the maximum gravity force does not get passed. Also the calculated gravitypull-vector
            gets added to the m_particles[i].newvelocity-vector.
    \param m_particles[i].gravityforce[j]
    \param m_particles[i].gravitypull[j]
    \param m_particles[i].gravityangle[j]
    \return m_particles[i].newvelocity
*/
void ParticleSystem::get_new_velocity(const unsigned i){
    m_particles[i].newvelocity = m_particles[i].velocity;
    for(unsigned j=0; j< massobject.size(); j++){ //Sum of all gravity vectors
        ///Maximum gravity pull limiter
        if(m_particles[i].gravityforce[j] > max_grav_force){
            m_particles[i].gravitypull[j] = sf::Vector2f( max_grav_force*cos(m_particles[i].gravityangle[j]) ,
                                                          max_grav_force*sin(m_particles[i].gravityangle[j]) );
        }
        m_particles[i].newvelocity += m_particles[i].gravitypull[j];
    }
}
/** \brief This function changes the newly calculated velocity to the current velocity of the particle.
    * It does this by using the following functions:
    -   get_radius_to_youranus(i)
    -   get_force(i)
    -   get_gravitational_pull(i)
    -   get_gravitational_angle(i)
    -   get_gravity_vector(i)
    -   get_new_velocity(i)
    \return m_particles[i].velocity
*/
void ParticleSystem::change_velocity(const unsigned i){
        get_radius_to_youranus(i);
        get_force(i);
        get_gravitational_pull(i);
        get_gravitational_angle(i);
        get_gravity_vector(i);
        get_new_velocity(i);
        m_particles[i].velocity = m_particles[i].newvelocity;   // Sets newly calculated velocity to current velocity
}
/** \brief This function updates the position of a particle.
    * First it runs the change_velocity function, then it will add a vector that is the product of m_particle[i].velocity
        and simulation speed to the m_vertices[i].position vector.
    \param m_particles[i].velocity
    \return m_vertives[i].position
*/
void ParticleSystem::update_position(const unsigned i){
    change_velocity(i);                                                                                                     // updating the velocity of the particle
    m_vertices[i].position += sf::Vector2f(m_particles[i].velocity.x * sim_speed,m_particles[i].velocity.y * sim_speed);    // update the position of the corresponding vertex
}
/** \brief This function updates the color of the particle.
    * One can play alot with the color of the particle, for example; one can use the velocity
        of the particle to give it a color ratio depending on how fast or slow it goes.
    \return m_vertices[i].color
*/
void ParticleSystem::update_color(const unsigned i){
    //m_vertices[i].color = sf::Color::White;
    float relativeV = sqrt(pow(m_particles[i].velocity.x, 2) + pow(m_particles[i].velocity.y, 2));
    float ratio =  1/(1+pow(euler, relativeV*0.0000002));
    float ratio2 = 1/(1+pow(euler, m_particles[i].velocity.y*0.000003));
    float ratio3 = 1/(1+pow(euler,relativeV/75));
    //m_vertices[i].color = sf::Color(255-(int)(ratio*500)%255, (int)(ratio*200)%255, (int)(ratio*500)%255, 100); //transparant blue
    //m_vertices[i].color = sf::Color(255-(int)(ratio*500)%255, (int)(ratio*500)%255, (int)(sin((float)i)*255), 255-(int)(ratio3*500)%255+150);
    //Default
    //m_vertices[i].color = sf::Color(255-(int)(ratio*500)%255, 255-(int)(ratio*500)%255, (int)(ratio*255), 255-(int)(ratio3*500)%255);
    m_vertices[i].color = sf::Color(pow((int)(ratio*500)%255,1), pow(255-(int)(ratio*500)%255,3), pow(255-(int)(ratio*500)%255,2) ,150);
}
/** \brief This function gives a starting position for each particle divided over the chosen number of clusters. It also makes it able to change
            the reaction to the velocity setting to a particle.
    * Function format: particles.create_particle_clusters(clustercount, compact, startvelocity, spawnradius, phase_offset_velocity, velocity_setting);
    * Velocity_setting options: 0 = radial, 1 = radial mirrored, 2 = linear.
    \param cluster_count
    \param compact
    \param startvelocity
    \param spawnradius
    \param velocity_fase_offset
*/
void ParticleSystem::create_particle_clusters(const unsigned cluster_count, const float compact, const float startvelocity, unsigned int spawnradius, float velocity_fase_offset, const unsigned s_velocity_type){
    for(unsigned clusterIndex=0; clusterIndex<cluster_count; clusterIndex++){
        m_emitter = sf::Vector2f(spawnradius *cos(2*PI/cluster_count*clusterIndex),
                                    spawnradius *sin(2*PI/cluster_count*clusterIndex));
        m_emitter -= {(viewwidth/2),(viewheight/2)};    // This centers the spawning of the clusters
        unsigned counter = 0;                           // Making a counter and set to 0
        for(unsigned i = clusterIndex*(m_particles.size()/cluster_count);
                     i < (clusterIndex+1)*(m_particles.size()/cluster_count);
                     i++){
            counter++;
            unsigned index=i;
            if(s_velocity_type ==0){// radial
            m_particles[index].velocity = sf::Vector2f(startvelocity* -sin(2*PI/(float)cluster_count*(float)clusterIndex+velocity_fase_offset),
                                                       startvelocity*  cos(2*PI/(float)cluster_count*(float)clusterIndex+velocity_fase_offset));
            }else
            if(s_velocity_type ==1){// radial mirrored
            m_particles[index].velocity = sf::Vector2f(startvelocity*  -cos(2*PI/(float)cluster_count*(float)clusterIndex+velocity_fase_offset),
                                                       startvelocity*   sin(2*PI/(float)cluster_count*(float)clusterIndex+velocity_fase_offset));
            }if(s_velocity_type ==2){// linear
            m_particles[index].velocity = sf::Vector2f(startvelocity*  cos(velocity_fase_offset+ i),
                                                       startvelocity*  sin(velocity_fase_offset+ i));
            }
            m_vertices[index].position = m_emitter+(sf::Vector2f((cos(index)*compact*counter)+viewwidth,
                                                                 (sin(index)*compact*counter)+viewheight));
            /**TODO:
            ADD FUNCTION TO SET BEGIN SPEED
            ADD FUNCTION TO UPDATE POSITION IN UPDATE()*/
            }
        }
}
/** \brief This function makes it able to create a massobject.
    * Function format: particles.create_massobject(posx, posy, mass, gravity_field);
    \param sf::Vector2f position
    \param mass
    \param gravity_field
    \return void
*/
void ParticleSystem::create_massobject(const sf::Vector2f position, const float mass, const float gravity_field){
    massobject.push_back(MassObject(position,mass,gravity_field));
    for(unsigned i=0; i < m_particles.size(); i++){
        m_particles[i].addMassobjectVector();
    }
}
/** \brief This function sets a position to the m_emitter
    \param sf::Vector2f position
    \return void
*/
void ParticleSystem::setEmitter(sf::Vector2f position){
    m_emitter = position;
}
/** \brief This function updates the position and the color for all particles. It runs the update_position
            function and the update_color function to do so.
*/
void ParticleSystem::update(){
    for (std::size_t i = 0; i < m_particles.size(); ++i){
        update_position(i);
        update_color(i);
    }
}
/** \brief This function draws the vertex array that the m_vertices(particles) are stored in.
*/
void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    states.transform *= getTransform();     // apply the transform
    states.texture = NULL;                  // our particles don't use a texture
    target.draw(m_vertices, states);        // draw the vertex array
}
/** \brief This function makes it able to let the particles react on the massobjects by storing their properties into a vector.
*/
void ParticleSystem::Particle::addMassobjectVector(){
    gravitypull.emplace_back();
    gravityforce.push_back(0);
    gravityangle.push_back(0);
    force.push_back(0);
    radiustoyouranus.push_back(0);
}
