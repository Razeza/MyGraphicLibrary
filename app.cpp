//
// Created by danila on 09.12.2020.
//

#ifndef GRAPH_LIB_APP_H
#define GRAPH_LIB_APP_H

#include "app.hpp"

Window_manager* App::root_window = nullptr;

void App::init (Point size) {
    create_window (size);
}

void App::set_root_window(Window_manager* init_root_window) {
    App::root_window = init_root_window;
}

void App::run() {
    while (true)
    {
        if (Window_manager::CLOSE == App::root_window->manage_windows ()) {
            break;
        }
        App::root_window->render ();
        render_window ();
    }
}


#endif //GRAPH_LIB_APP_H

