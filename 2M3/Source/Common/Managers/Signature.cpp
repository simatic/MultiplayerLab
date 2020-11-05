#include "Common/Managers/Signature.h"

std::size_t Signature::registeredComponentsCount = 0;
std::unordered_map<std::size_t, std::size_t> Signature::idToBitIndexMap;

Signature::Signature() :
	signature()
{}

std::ostream& operator<<(std::ostream& os, const Signature& signature)
{
	return os << signature.signature;
}