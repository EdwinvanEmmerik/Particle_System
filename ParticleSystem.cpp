#include "ParticleSystem.h"
#include "Particles_defines.h"
void ParticleSystem::get_radius_to_youranus(const unsigned i){
    for(unsigned j=0; j< massobject.size(); j++){
        m_particles[i].radiustoyouranus[j] = sqrt(pow((m_vertices[i].position.x-massobject[j].position.x), 2) + pow(((m_vertices[i].position.y-massobject[j].position.y)), 2));
    }
}
void ParticleSystem::get_force(const unsigned i){
    for(unsigned j=0; j< massobject.size(); j++){
        m_particles[i].force[j] = ((Gconst * m_particles[i].mass * massobject[j].mass) / pow(m_particles[i].radiustoyouranus[j], massobject[j].gravity_field));
    }
}
void ParticleSystem::get_gravitational_pull(const unsigned i){
    for(unsigned j=0; j< massobject.size(); j++){
        m_particles[i].gravityforce[j] = (m_particles[i].force[j] / m_particles[i].mass);
    }
}
void ParticleSystem::get_gravitational_angle(const unsigned i){
    for(unsigned j=0; j< massobject.size(); j++){
        m_particles[i].gravityangle[j] = atan2((massobject[j].position.y - (float)m_vertices[i].position.y),
                                               (massobject[j].position.x - (float)m_vertices[i].position.x));
    }
}
void ParticleSystem::get_gravity_vector(const unsigned i){
    for(unsigned j=0; j< massobject.size(); j++){
        m_particles[i].gravitypull[j] = sf::Vector2f(cos(m_particles[i].gravityangle[j]) * m_particles[i].gravityforce[j],
                                                     sin(m_particles[i].gravityangle[j]) * m_particles[i].gravityforce[j] );
    }
}
void ParticleSystem::get_new_velocity(const unsigned i){
    //m_particles[i].newvelocity = m_particles[i].velocity;

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
void ParticleSystem::change_velocity(const unsigned i){
        get_radius_to_youranus(i);
        get_force(i);
        get_gravitational_pull(i);
        get_gravitational_angle(i);
        get_gravity_vector(i);
        get_new_velocity(i);

        m_particles[i].velocity = m_particles[i].newvelocity;
}
void ParticleSystem::update_position(const unsigned i){
    change_velocity(i);
            // update the position of the corresponding vertex
    m_vertices[i].position += sf::Vector2f(m_particles[i].velocity.x * sim_speed,m_particles[i].velocity.y * sim_speed);
}
void ParticleSystem::update_color(const unsigned i){
    //m_vertices[i].color = sf::Color::White;
    float relativeV = sqrt(pow(m_particles[i].velocity.x, 2) + pow(m_particles[i].velocity.y, 2));
    float ratio =  1/(1+pow(euler, relativeV*0.0000002));
    float ratio2 = 1/(1+pow(euler, m_particles[i].velocity.y*0.000003));

    float ratio3 = 1/(1+pow(euler,relativeV/75) );
            //cout << relativeV << endl;
            //m_vertices[i].color = sf::Color(255-(int)(ratio*500)%255, (int)(ratio*200)%255, (int)(ratio*500)%255, 100);

    m_vertices[i].color = sf::Color(255-(int)(ratio*500)%255, (int)(ratio*500)%255, (int)(sin((float)i/500)*255), 255-(int)(ratio3*500)%255+150);
    //Default
    //float ratio =  1/(1+pow(euler, relativeV*0.000002));
    //m_vertices[i].color = sf::Color(255-(int)(ratio*500)%255, 255-(int)(ratio*500)%255, (int)(ratio*255), 255-(int)(ratio3*500)%255);
}
void ParticleSystem::create_particle_clusters(const unsigned cluster_count, const float compact, const float startVelocity, unsigned int spawnradius, float velocity_fase_offset, const unsigned s_velocity_type){
 //Centerpoint where the clusters are spawned around
        for(unsigned clusterIndex=0; clusterIndex<cluster_count; clusterIndex++){
            //cout << "Break" << endl;
            m_emitter = sf::Vector2f(spawnradius *cos(2*PI/cluster_count*clusterIndex),
                                     spawnradius *sin(2*PI/cluster_count*clusterIndex));
            ///For some reason this centers the spawning of the clusters
            m_emitter -= {(viewheight/4),(viewwidth/4)};
            unsigned counter=0;

            for(unsigned i =     clusterIndex*(m_particles.size()/cluster_count);
                         i < (clusterIndex+1)*(m_particles.size()/cluster_count);
                         i++){
            counter++;
            unsigned index=i;
            //cout << index << endl;
            //m_particles[index].velocity = sf::Vector2f(startVelocity , //-sin(2*PI/cluster_count*clusterIndex),
            //                                           startVelocity );//cos(2*PI/cluster_count*clusterIndex));
            ///Rotational
            if(s_velocity_type ==0){
            m_particles[index].velocity = sf::Vector2f(startVelocity* -sin(2*PI/(float)cluster_count*(float)clusterIndex+velocity_fase_offset),
                                                       startVelocity*  cos(2*PI/(float)cluster_count*(float)clusterIndex+velocity_fase_offset));
            }else
            if(s_velocity_type ==1){
            m_particles[index].velocity = sf::Vector2f(startVelocity*  -cos(2*PI/(float)cluster_count*(float)clusterIndex+velocity_fase_offset),
                                                       startVelocity*   sin(2*PI/(float)cluster_count*(float)clusterIndex+velocity_fase_offset));
            }if(s_velocity_type ==2){
            m_particles[index].velocity = sf::Vector2f(startVelocity*  cos(velocity_fase_offset),
                                                       startVelocity*  sin(velocity_fase_offset));
            }
            m_vertices[index].position = m_emitter+(sf::Vector2f((cos(index)*compact*counter)+3*(float)viewheight/4,
                                                                 (sin(index)*compact*counter)+3*(float)viewwidth/4));

                                            ///ADD FUNCTION TO SET BEGIN SPEED
                                                   ///ADD FUNCTION TO UPDATE POSITION IN UPDATE()
            }
        }
    }
void ParticleSystem::create_massobject(const sf::Vector2f position, const float mass, const float gravity_field){
    massobject.push_back(MassObject(position,mass,gravity_field));
    for(unsigned i=0; i < m_particles.size(); i++){
        m_particles[i].addMassobjectVector();
    }
}
void ParticleSystem::setEmitter(sf::Vector2f position){
    m_emitter = position;
}
void ParticleSystem::update(){
    for (std::size_t i = 0; i < m_particles.size(); ++i){
        // update the particle lifetime
        // if the particle is dead, respawn i
        // update the position of the corresponding vertex
        update_position(i);
        update_color(i);
        // update the alpha (transparency) of the particle according to its lifetime
    }
}
void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const{
    // apply the transform
    states.transform *= getTransform();

    // our particles don't use a texture
    states.texture = NULL;

    // draw the vertex array
    target.draw(m_vertices, states);
}
void ParticleSystem::resetParticle(std::size_t index){
    // give a random velocity and lifetime to the particle
    float compact = 0.02;
    float angle = 0;
    float speed = 10;
    m_particles[index].velocity = sf::Vector2f( cos(angle) * speed,
                                                    sin(angle) * speed);
    //m_particles[index].lifetime = sf::milliseconds((rand() % 2000) + 1000);
    // reset the position of the corresponding vertex
    m_vertices[index].position = (sf::Vector2f((cos(index)*compact*index)+3*(float)viewwidth/4,
                                                   (sin(index)*compact*index)+3*(float)viewheight/4));
}
void ParticleSystem::Particle::addMassobjectVector(){
    gravitypull.emplace_back();
    gravityforce.push_back(0);
    gravityangle.push_back(0);
    force.push_back(0);
    radiustoyouranus.push_back(0);
}
