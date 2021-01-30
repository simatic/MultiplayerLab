#include "Common/Network/TemplatePacket.h"

// Sending side constructor, easiest to write: simply fill your fields with your arguments
TemplatePacket::TemplatePacket(PacketSequenceIndex index, std::string someArgument, std::string someOtherArgument):
        Packet(index), // call the superclass constructor and store the packet sequence index
        someArgument(someArgument), // store an argument
        someOtherArgument(someOtherArgument) {} // store other argument

// Receiving side constructor, slightly harder to write: you need to extract the arguments from the SFML packet
TemplatePacket::TemplatePacket(PacketSequenceIndex index, sf::Packet& source): Packet(index) {
    // read from the SFML packet
    // Make sure to read in the same order that you write inside the `write` method
    source >> someArgument;
    source >> someOtherArgument;

    // Hint: for vectors, you can start by writing/reading its length before having a for-loop to read all elements
    //  WorldStatePacket.cpp uses it, you can take a look at it as an example
}

std::unique_ptr<Packet> TemplatePacket::handle(INetworkModule* iNetworkModule) const {
    // What should you do immediately when receiving this packets?
    //  If you have anything to do, put it here. But make sure a NetworkSystem is not more appropriate beforehand.

    // If you want to reply to this packet directly (eg. a pong packet for a ping packet), create and return it right here:
    return nullptr;
}

void TemplatePacket::write(sf::Packet &destination) const {
    // write into the SFML packet.
    // Make sure to write in the same order that you read inside the receiving-side constructor
    destination << someArgument;
    destination << someOtherArgument;
}

std::string TemplatePacket::getSomeArgument() const {
    return someArgument;
}

std::string TemplatePacket::getSomeOtherArgument() const {
    return someOtherArgument;
}
