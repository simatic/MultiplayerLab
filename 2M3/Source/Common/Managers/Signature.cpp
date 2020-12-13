#include "Common/Managers/Signature.h"

std::size_t Signature::registeredComponentsCount = 0;
std::unordered_map<std::size_t, std::size_t> Signature::idToBitIndexMap;

Signature::Signature() :
	bitset()
{}

Signature Signature::operator&(const Signature& signature) const
{
	Signature result;
	result.bitset = bitset & signature.bitset;
	return result;
}

bool Signature::operator==(const Signature& signature) const
{
	return bitset == signature.bitset;
}

std::ostream& operator<<(std::ostream& os, const Signature& signature)
{
	return os << signature.bitset;
}