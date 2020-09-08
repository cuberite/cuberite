#pragma once

#include <initializer_list>





/** Represents the state of a single block (previously known as "block meta").
The state consists of a map of string -> string, plus a mechanism for fast equality checks between two BlockState instances.
Once a BlockState instance is created, it is then immutable - there's no way of changing it, only by creating a (modified) copy.
A BlockState instance can be created from hard-coded data or from dynamic data:
	BlockState bs({{"key1", "value1"}, {key2", "value2"}});  // Hard-coded
	- or -
	std::map<AString, AString> map({{"key1", "value1"}, {key2", "value2"}});
	map["key3"] = "value3";
	BlockState bs(map);  // From dynamic data
*/
class BlockState
{
public:

	/** Creates a new instance with an empty map. */
	BlockState();

	/** Creates a new instance consisting of a single key-value pair.
	If the value is empty, it is not stored wihin the map. */
	BlockState(const AString & aKey, const AString & aValue);

	/** Creates a new instance initialized with several (hard-coded) key-value pairs.
	Any key with an empty value is not stored within the map. */
	BlockState(std::initializer_list<std::pair<const AString, AString>> aKeysAndValues);

	/** Creates a new instance initialized with several (dynamic) key-value pairs.
	Makes a copy of aKeysAndValues for this object.
	Any key with an empty value is not stored within the map. */
	BlockState(const std::map<AString, AString> & aKeysAndValues);

	/** Creates a new instance initialized with several (dynamic) key-value pairs.
	Any key with an empty value is not stored within the map. */
	BlockState(std::map<AString, AString> && aKeysAndValues);

	/** Creates a copy of the specified BlockState with the (hard-coded) additional keys and values added to it.
	Any key in aAdditionalKeysAndValues that is already present in aCopyFrom is overwritten with the aAdditionalKeysAndValues' one.
	Any key with an empty value is not stored in the map.
	(it's possible to erase a key from aCopyFrom by setting it to empty string in aAdditionalKeysAndValues). */
	BlockState(const BlockState & aCopyFrom, std::initializer_list<std::pair<const AString, AString>> aAdditionalKeysAndValues);

	/** Creates a copy of the specified BlockState with the (dynamic) additional keys and values added to it.
	Any key in aAdditionalKeysAndValues that is already present in aCopyFrom is overwritten with the aAdditionalKeysAndValues' one.
	Any key with an empty value is not stored in the map.
	(it's possible to erase a key from aCopyFrom by setting it to empty string in aAdditionalKeysAndValues). */
	BlockState(const BlockState & aCopyFrom, const std::map<AString, AString> & aAdditionalKeysAndValues);

	/** Less-than comparison. */
	bool operator <(const BlockState & aOther) const;

	/** Fast equality check. */
	bool operator ==(const BlockState & aOther) const;

	/** Fast inequality check. */
	bool operator !=(const BlockState & aOther) const
	{
		return !(operator ==(aOther));
	}

	/** Returns the value at the specified key.
	If the key is not present, returns an empty string. */
	const AString & value(const AString & aKey) const;


protected:

	/** The state, represented as a string->string map. */
	std::map<AString, AString> mState;

	/** The checksum used for the fast equality check.
	This is calculated upon creation. */
	UInt32 mChecksum;


	/** Normalizes mState and calculates the checksum from it.
	Removes all the empty values from mState.
	Used only from constructors. */
	UInt32 initializeChecksum();

	/** Removes all the keys from mState that have an empty value. */
	void removeEmptyKeys();

	/** Calculates the partial checksum of a single string.
	Used from within initializeChecksum(). */
	UInt32 partialChecksum(const AString & aString);
};
