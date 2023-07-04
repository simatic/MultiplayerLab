#pragma once
#include <string>
#include <chrono>
#include <cereal/archives/binary.hpp>
#include <cereal/types/chrono.hpp>
#include <cereal/types/string.hpp>

// Maximum length of a UDP packet
constexpr size_t max_length{65515};

//---------------------------------------------------
// Generic message data
//---------------------------------------------------
struct MsgWithId
{
    unsigned char id{};

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(id); // serialize things by passing them to the archive
    }
};

struct MsgForBroadcastSample
{
    unsigned char senderId{};
    unsigned int messageId{};
    std::chrono::time_point<std::chrono::system_clock> sendTime;
    std::string filler;

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(senderId, messageId, sendTime, filler); // serialize things by passing them to the archive
    }
};

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

using ServerBroadcastMessage = MsgForBroadcastSample;
using ServerIdResponse = MsgWithId;

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

using ClientDisconnectIntent = MsgWithId;
using ClientDoneSendingMessages = MsgWithId;

struct ClientIdRequest
{
    int nbClients{};

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(nbClients); // serialize things by passing them to the archive
    }
};

using ClientMessageToBroadcast = MsgForBroadcastSample;

//---------------------------------------------------
// Templates
//---------------------------------------------------

/// @brief Returns data of message contained in msgStream (Note: msg_id has already been read in msgStream)
/// @details
/// Usage: Use <tt>auto data = readDataInMsgStream<DataStructName>(msgStream)</tt>
/// @tparam D Name of (Data) structure to be read
/// @args msgStream string stream containing message
/// @return Data structure read
template <typename D>
D readDataInMsgStream(std::istringstream &msgStream)
{
    D data{};
    cereal::BinaryInputArchive iarchive(msgStream); // Create an input archive
    iarchive(data); // Read the data from the archive
    return data;
}

/// @brief Returns a string containing a message with msgId and no data
/// Usage: Use <tt>auto sv = prepareMsgWithNoData<EnumName>(msgId)</tt>
/// @tparam E Name of msgId Enum
/// @args msgId to be stored in message
/// @return string containing message
template <typename E>
std::string prepareMsgWithNoData(E msgId)
{
    std::stringstream o_stream;
    o_stream << static_cast<unsigned char>(msgId);
    return o_stream.str();
}

/// @brief Returns a string containing a message with msgId and data
/// Usage: Use <tt>auto sv = prepareMsg<EnumName,DataStructName>(msgId, data)</tt>
/// @tparam E Name of msgId Enum
/// @tparam D Name of (Data) structure to be written in message
/// @args msgId to be stored in message
/// @args data to be stored in message
/// @return string containing message
template <typename E, typename D>
std::string prepareMsg(E msgId, D data)
{
    std::stringstream o_stream;
    o_stream << static_cast<unsigned char>(msgId);
    {
        cereal::BinaryOutputArchive oarchive(o_stream); // Create an output archive
        oarchive(data); // Write the data to the archive
    } // archive goes out of scope, ensuring all contents are flushed
    return o_stream.str();
}
