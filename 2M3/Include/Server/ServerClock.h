#include <SFML/System.hpp>

class ServerClock {
private:
    static ServerClock* instance;
    static sf::Mutex instanceMutex;
    sf::Clock internalClock;

public:
    static const ServerClock& getInstance();
    float asSeconds() const;
    sf::Int32 asMilliseconds() const;
    sf::Time get() const;
};