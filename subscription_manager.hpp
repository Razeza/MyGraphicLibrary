//
// Created by dunka on 15.11.2020.
//

#ifndef GRAPH_LIB_SUBSCRIPTION_MANAGER_HPP
#define GRAPH_LIB_SUBSCRIPTION_MANAGER_HPP

#include <unordered_set>
#include <unordered_map>

class Subscription_manager
{
private:
    static std::unordered_map<Abstract_window*, std::unordered_set<Abstract_window*>> subscriptions;

public:
    Subscription_manager () = delete;

    static void add_subscription (Abstract_window* sender, Abstract_window* subscriber);
    static void unsubscribe      (Abstract_window* sender, Abstract_window* subscriber);

    static void send_event       (Abstract_window* sender, Event* event);
};


#endif //GRAPH_LIB_SUBSCRIPTION_MANAGER_HPP
