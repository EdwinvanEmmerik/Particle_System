#include "ParticleSystem.h"
#include "ViewHandler.h"
int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(width, height), "Particles");
    window.setFramerateLimit(30);
    ViewHandler viewH;
    viewH.setView(1);
    // create the particle system
    ParticleSystem particles(20000);

    //Creation of a Mass Object in the center of the screen
    particles.create_massobject(sf::Vector2f((float)width      /2,
                                             (float)height     /2),
                                                 20000000,
                                                 1.5);
    // Creation of particle clusters
    particles.create_particle_clusters(10, 0.05, 100000, 200, PI, 0);

    // run the main loop
    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed)
                viewH.alterView();
        }
        particles.update();
        // draw it
        window.clear();
        window.draw(particles);

        window.setView(viewH.returnView());
        window.display();
    }
    return 0;
}
