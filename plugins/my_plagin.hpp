/*============================================================================*/

#include "plugins/api.hpp"

/*============================================================================*/

class My_plugin : public PluginAPI::Plugin {

    bool init   () override;
    bool deinit () override;

    void start_apply (PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
    void apply       (PluginAPI::Canvas canvas, PluginAPI::Position pos) override;
    void stop_apply  (PluginAPI::Canvas canvas, PluginAPI::Position pos) override;

};

/*============================================================================*/