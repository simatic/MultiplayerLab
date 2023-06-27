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
    IdResponse,
    BroadcastMessage,
    AckDoneSendingMessages
};

using ServerIdResponse = MsgWithId;
using ServerBroadcastMessage = MsgForBroadcastSample;

//---------------------------------------------------
// Packets sent by the client
//---------------------------------------------------
enum class ClientMsgId : unsigned char
{
    IdRequest,
    MessageToBroadcast,
    DoneSendingMessages
};

using ClientMessageToBroadcast = MsgForBroadcastSample;
using ClientDoneSendingMessages = MsgWithId;

//---------------------------------------------------
// Templates
//---------------------------------------------------

/// @brief Returns data of message contained in msg_stream (Note: msg_id has already been read in msg_stream)
/// @details
/// Usage: Use <tt>auto data = read_data_in_msg_stream<DataStructName>(msg_stream)</tt>
/// @tparam D Name of (Data) structure to be read
/// @args msg_stream string stream containing message
/// @return Data structure read
template <typename D>
D read_data_in_msg_stream(std::istringstream &msg_stream)
{
    D data{};
    cereal::BinaryInputArchive iarchive(msg_stream); // Create an input archive
    iarchive(data); // Read the data from the archive
    return data;
}

/// @brief Returns a string containing a message with msg_id and no data
/// Usage: Use <tt>auto sv = prepare_msg_with_no_data<EnumName>(msg_id)</tt>
/// @tparam E Name of msg_id Enum
/// @args msg_id to be stored in message
/// @return string containing message
template <typename E>
std::string prepare_msg_with_no_data(E msg_id)
{
    std::stringstream o_stream;
    o_stream << static_cast<unsigned char>(msg_id);
    return o_stream.str();
}

/// @brief Returns a string containing a message with msg_id and data
/// Usage: Use <tt>auto sv = prepare_msg<EnumName,DataStructName>(msg_id, data)</tt>
/// @tparam E Name of msg_id Enum
/// @tparam D Name of (Data) structure to be written in message
/// @args msg_id to be stored in message
/// @args data to be stored in message
/// @return string containing message
template <typename E, typename D>
std::string prepare_msg(E msg_id, D data)
{
    std::stringstream o_stream;
    o_stream << static_cast<unsigned char>(msg_id);
    {
        cereal::BinaryOutputArchive oarchive(o_stream); // Create an output archive
        oarchive(data); // Write the data to the archive
    } // archive goes out of scope, ensuring all contents are flushed
    return o_stream.str();
}
