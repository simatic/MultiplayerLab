#include <iostream>
#include "tcp_communication.h"
#include "common.h"

struct ForLength
{
    size_t size{};

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(size); // serialize things by passing them to the archive
    }
};

void sendPacket(boost::asio::ip::tcp::socket *psock, std::string const& s)
{
    ForLength forLength{s.length()};
    std::stringstream oStream;
    {
        cereal::BinaryOutputArchive oarchive(oStream); // Create an output archive
        oarchive(forLength); // Write the data to the archive
    } // archive goes out of scope, ensuring all contents are flushed

    auto sWithLength = oStream.str() + s;

    boost::asio::write(*psock, boost::asio::buffer(sWithLength.data(), sWithLength.length()));
}

std::string receivePacket(boost::asio::ip::tcp::socket *psock)
{
    // We read the length of the message
    size_t len;
    boost::system::error_code error;
    auto length = boost::asio::read(*psock, boost::asio::buffer(&len, sizeof(len)), error);
    if (error)
        throw boost::system::system_error(error); // Some other error. boost::asio::error::eof est l'erreur pertinente à regardr
    assert(length == sizeof(len));
    // We read the message itself
    std::vector<char> v(len);
    auto msg_length = boost::asio::read(*psock,
                                        boost::asio::buffer(v));
    assert(msg_length == len);
    return std::string{v.data(), v.size()};
}