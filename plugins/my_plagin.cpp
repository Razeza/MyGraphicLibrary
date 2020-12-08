/*============================================================================*/

/*
 * $ g++ -c -fPIC fancy_plugin.cpp -o fancy_plugin.o
 * $ gcc -shared fancy_plugin.o -o fancy_plugin.so
 */

#include <iostream>
#include "my_plagin.cpp"

/*============================================================================*/

bool My_plagin::init() {
    std::cout << "I am plugin and I am ready to use\n";
    return true;
}

bool My_plagin::deinit() {
    std::cout << "Bye-bye! (plugin deinitialized)\n";
    return true;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void My_plagin::start_apply(PluginAPI::Canvas canvas,
                                PluginAPI::Position pos) {
    std::cout << "Plugin application started\n";
}

void My_plagin::stop_apply(PluginAPI::Canvas canvas,
                               PluginAPI::Position pos) {
    std::cout << "Plugin application finished\n";
}

void My_plagin::apply(PluginAPI::Canvas canvas,
                          PluginAPI::Position pos) {
    std::cout << "Applying...\n";
}

/*============================================================================*/

extern "C" PluginAPI::Plugin* get_plugin() {
    return PluginAPI::get_instance<My_plugin>();
}

/*============================================================================*/