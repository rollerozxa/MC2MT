#ifndef NBT_SERIALIZATION_HEADER
#define NBT_SERIALIZATION_HEADER

#include "endian.hpp"

namespace NBT {

#define NBT_WRITER(name, type, func) \
	inline void write##name(UByte * bytes, type i) \
		{ *reinterpret_cast<type *>(bytes) = func(i); }

NBT_WRITER(Short,  UShort, htobe16)
NBT_WRITER(Int,    UInt,   htobe32)
NBT_WRITER(Long,   ULong,  htobe64)
NBT_WRITER(Float,  float,  htobe32)
NBT_WRITER(Double, double, htobe64)


#define NBT_READER(name, type, func) \
	inline type read##name(const UByte * bytes) \
		{ return func(*reinterpret_cast<const type *>(bytes)); }

NBT_READER(Short,  UShort, be16toh)
NBT_READER(Int,    UInt,   be32toh)
NBT_READER(Long,   ULong,  be64toh)
NBT_READER(Float,  float,  be32toh)
NBT_READER(Double, double, be64toh)

// The following take a reference to the index becuase they can read a
// variable amount of data and have to update the main index appropriately.
extern ByteArray  readByteArray(const UByte * bytes, ULong & index);
extern String     readString   (const UByte * bytes, ULong & index);
extern List       readList     (const UByte * bytes, ULong & index);
extern Compound * readCompound (const UByte * bytes, ULong & index);
extern IntArray   readIntArray (const UByte * bytes, ULong & index);

} // namespace NBT

#endif // NBT_SERIALIZATION_HEADER
