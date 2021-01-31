#include "Server/Network/ServerClock.h"

ServerClock* ServerClock::instance = nullptr;
sf::Mutex ServerClock::instanceMutex{};

const ServerClock &ServerClock::getInstance() {
    if(instance == nullptr) { // thread-safe singleton lazy-initialisation
        instanceMutex.lock();
        if(instance == nullptr) {
            instance = new ServerClock();
        }
        instanceMutex.unlock();
    }
    return *instance;
}

float ServerClock::asSeconds() const {
    return get().asSeconds();
}

sf::Int32 ServerClock::asMilliseconds() const {
    return get().asMilliseconds();
}

sf::Time ServerClock::get() const {
    return internalClock.getElapsedTime();
}


