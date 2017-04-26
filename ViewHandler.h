#ifndef VIEWHANDLER_H_INCLUDED
#define VIEWHANDLER_H_INCLUDED
//! \class ViewHandler
/*! \brief With this class implemented in your software you can; zoom in, zoom out
            and translate in x and y coordinates of the created window by generating a specific view.
*/
#define WALKSPEED 10

class ViewHandler{
public:
    ViewHandler(){
        viewCenter = sf::Vector2f(width/2,height/2);
        view.setSize(width,height);
        view.setCenter(viewCenter);
    }
    sf::View returnView(){
        return view;
    }
    void setView(const float i){
        view.zoom(i);
    };
    void alterView(){
        ///MOVEMENT HANDLER
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            // left key is pressed: move our character
            viewCenter -= {float(WALKSPEED), 0};
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            // left key is pressed: move our character
            viewCenter += {float(WALKSPEED), 0};
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            // left key is pressed: move our character
            viewCenter += {0, float(WALKSPEED)};
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            // left key is pressed: move our character
            viewCenter -= {0, float(WALKSPEED)};
        }
        ///ZOOM HANDLER
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
            // left key is pressed: move our character
            view.zoom(1.02);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
            // left key is pressed: move our character
            view.zoom(0.98);
        }
        view.setCenter(viewCenter);
    }
private:
    sf::View view;
    sf::Vector2f viewCenter;
};
#endif // VIEWHANDLER_H_INCLUDED
