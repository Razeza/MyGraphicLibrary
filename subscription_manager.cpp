//
// Created by dunka on 15.11.2020.
//

#ifndef SUBSCRIPTION_MANAGER_CPP
#define SUBSCRIPTION_MANAGER_CPP

#include "subscription_manager.hpp"

std::unordered_map<Abstract_window*, std::unordered_set<Abstract_window*>> Subscription_manager::subscriptions;

void Subscription_manager::add_subscription (Abstract_window* sender, Abstract_window* subscriber) {
    subscriptions[sender].insert(subscriber);
}

void Subscription_manager::unsubscribe(Abstract_window* sender, Abstract_window* subscriber) {
    subscriptions[sender].erase(subscriber);
}

void Subscription_manager::send_event(Abstract_window* sender, Event* event) {
    for (auto& subscriber : subscriptions[sender]) {
        subscriber->process_event (event);
    }

    delete event;
}


#endif

