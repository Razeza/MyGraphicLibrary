#define SFML

#include "engine.cpp"
#include "graphic_library.cpp"
#include "cnavas.cpp"


class App {
    static Window_manager* root_window;
public:
    static void init (Point size);
    static void set_root_window (Window_manager* root_window);
    static void run ();
};




