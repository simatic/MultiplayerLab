#pragma once
#include <string>
#include <chrono>
#include <cereal/archives/binary.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/types/string.hpp>

// Maximum length of a UDP packet
constexpr size_t maxLength{65515};

//---------------------------------------------------
// Packets sent by the server
//---------------------------------------------------
enum class ServerMsgId : unsigned char
{
    AckDoneSendingMessages = 65, // We start with a value which be displayed as a character in debugger
    AckDisconnectIntent,
    BroadcastBegin,
    BroadcastEnd,
    BroadcastMessage,
    IdResponse
};

struct GenericServerMsgWithoutData
{
    ServerMsgId msgId{};

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(msgId); // serialize things by passing them to the archive
    }
};

using ServerAckDoneSendingMessages = GenericServerMsgWithoutData;
using ServerAckDisconnectIntent = GenericServerMsgWithoutData;
using ServerBroadcastBegin = GenericServerMsgWithoutData;
using ServerBroadcastEnd = GenericServerMsgWithoutData;

struct ServerBroadcastMessage
{
    ServerMsgId msgId{};
    unsigned char senderId{};
    unsigned int msgNum{};
    std::chrono::time_point<std::chrono::system_clock> sendTime;
    std::string filler;

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(msgId, senderId, msgNum, sendTime, filler); // serialize things by passing them to the archive
    }
};

struct ServerIdResponse
{
    ServerMsgId msgId{};
    unsigned char id{};

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(msgId, id); // serialize things by passing them to the archive
    }
};

//---------------------------------------------------
// Packets sent by the client
//---------------------------------------------------
enum class ClientMsgId : unsigned char
{
    DisconnectIntent = 97, // We start with a value which be displayed as a character in debugger + the enum values are different from values in enum ServerMsgId
    DoneSendingMessages,
    IdRequest,
    MessageToBroadcast
};

struct GenericClientMsgWithId
{
    ClientMsgId msgId{};
    unsigned char id{};

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(msgId, id); // serialize things by passing them to the archive
    }
};

using ClientDisconnectIntent = GenericClientMsgWithId;
using ClientDoneSendingMessages = GenericClientMsgWithId;

struct ClientIdRequest
{
    ClientMsgId msgId{};
    int nbClients{};

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(msgId, nbClients); // serialize things by passing them to the archive
    }
};

struct ClientMessageToBroadcast
{
    ClientMsgId msgId{};
    unsigned char senderId{};
    unsigned int msgNum{};
    std::chrono::time_point<std::chrono::system_clock> sendTime;
    std::string filler;

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(msgId, senderId, msgNum, sendTime, filler); // serialize things by passing them to the archive
    }
};

//---------------------------------------------------
// Templates
//---------------------------------------------------

/// @brief Returns structured deserialized from msgStream
/// @details
/// Usage: Use <tt>auto data = deserializeStruct<StructName>(msgStream)</tt>
/// @tparam S Name of structure to be read
/// @args msgString String containing message
/// @return Data structure read
template <typename S>
S deserializeStruct(const std::string &msgString)
{
    std::istringstream msgStream{msgString};
    cereal::BinaryInputArchive archive(msgStream); // Create an input archive
    S structure{};
    archive(structure); // Read the structure from the archive
    return structure;
}

/// @brief Returns a string containing serialized structure
/// Usage: Use <tt>auto sv = prepareMsg<EnumName,DataStructName>(msgId, structure)</tt>
/// @tparam S Name of (Data) structure to be written in message
/// @args msgId to be stored in message
/// @args structure to be stored in message
/// @return string containing serialized structure
template <typename S>
std::string serializeStruct(S structure)
{
    std::stringstream o_stream;
    {
        cereal::BinaryOutputArchive archive(o_stream); // Create an output archive
        archive(structure); // Write the structure to the archive
    } // archive goes out of scope, ensuring all contents are flushed
    return o_stream.str();
}
