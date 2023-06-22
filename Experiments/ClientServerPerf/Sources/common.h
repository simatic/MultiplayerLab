#pragma once
#include <string>
#include <chrono>
#include <cereal/archives/binary.hpp>
#include <cereal/types/chrono.hpp>

// Maximum length of a UDP packet
constexpr size_t max_length{65515};

// Packets originated in the server
enum class Server : unsigned char
{
    IdResponse,
    BroadcastMessage,
    AckDoneSendingMessages
};

struct ServerIdResponse
{
    unsigned char id;

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(id); // serialize things by passing them to the archive
    }
};

struct ServerBroadcastMessage
{
    unsigned char senderId{};
    unsigned int messageId{};
    std::chrono::time_point<std::chrono::system_clock> sendTime;

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(senderId, messageId, sendTime); // serialize things by passing them to the archive
    }
};


// Packets originated in the client
enum class Client : unsigned char
{
    IdRequest,
    MessageToBroadcast,
    DoneSendingMessages,
    DisconnectInfo // Only used for UDP client to tell server it is disconnecting
};

struct ClientMessageToBroadcast
{
    unsigned char senderId{};
    unsigned int messageId{};
    std::chrono::time_point<std::chrono::system_clock> sendTime;

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(senderId, messageId, sendTime); // serialize things by passing them to the archive
    }
};

struct ClientDisconnectInfo
{
    unsigned char id;

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(id); // serialize things by passing them to the archive
    }
};



