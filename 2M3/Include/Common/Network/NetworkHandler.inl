//
// Created by jglrxavpok on 16/12/2020.
//
#include "NetworkHandler.h"

template<typename Packet, typename... Args>
std::unique_ptr<Packet> NetworkHandler::create(Args... args) {
    return std::move(std::make_unique<Packet>(generateNextIndex(), args...));
}