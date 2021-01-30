#include "Common/Managers/Signature.h"

thread_local std::size_t Signature::registeredComponentsCount = 0;
thread_local std::unordered_map<std::size_t, std::size_t> Signature::idToBitIndexMap;

/**
 * Constructs an empty signature (00000...).
 */
Signature::Signature() :
	bitset()
{}

/**
 * Operator overload to perform a logic AND operation between two signatures.
 * @param signature The second signature to perform the operation on.
 */
Signature Signature::operator&(const Signature& signature) const
{
	Signature result;
	result.bitset = bitset & signature.bitset;
	return result;
}

/**
 * Operator overload to check if two signatures are the same.
 * @param signature The second signature to perform the operation on.
 */
bool Signature::operator==(const Signature& signature) const
{
	return bitset == signature.bitset;
}

/**
 * Operator overload to extract the bitset in an ostream object.
 */
std::ostream& operator<<(std::ostream& os, const Signature& signature)
{
	return os << signature.bitset;
}