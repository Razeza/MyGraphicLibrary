#include <iostream>



#define DEB

#ifdef DEB
    #define DEBUG(exp) exp
    #define check() std::cout << "here" << std::endl;
#else
    #define DEBUG(exp)
    #define check() 
#endif



#include "research.cpp"
#include "menu.cpp"
#include "graphics.cpp"


void App ();

#include "application.cpp"

int main () {
    App ();
}




void App ()
{
    Application app;
   
    while (app.is_open()) {
           
        sf::Event event;
        while (app.poll_event (event))
        {
            if (event.type == sf::Event::Closed) {
                app.close ();
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

                const auto& mouse_coordinates = sf::Mouse::getPosition(app.get_window ()); 
                
                DEBUG (std::cout << mouse_coordinates.x << " " <<  mouse_coordinates.y << "\n";)
 
                switch (app.button_pressed (mouse_coordinates.x, mouse_coordinates.y)) {
                    case CLOSE:
                    {
                        app.close ();
                    }

                }
            }
        }

        app.draw_app ();
        app.display  ();

    }
}
