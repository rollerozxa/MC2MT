
#include <cstring>
#include <sstream>

#include "nbt.hpp"
#include "serialization.hpp"


namespace NBT {

inline void writeByte(UByte * bytes, UByte b);
inline void writeBytes(UByte * bytes, const UByte * write, UInt size);
inline void writeString(UByte * bytes, const char * str, UShort size);
inline UByte readByte(const UByte * bytes);


/*****************
 * Serialization *
 *****************/

// Doesn't include size of tagid (always 1)
ULong Tag::getSerializedSize() const
{
	ULong size = 0;
	ULong i = 0;
	switch (type) {
	case TagType::End: return 0;
	case TagType::Byte: return sizeof(Byte);
	case TagType::Short: return sizeof(Short);
	case TagType::Int: return sizeof(Int);
	case TagType::Long: return sizeof(Long);
	case TagType::Float: return sizeof(float);
	case TagType::Double: return sizeof(double);
	case TagType::ByteArray:
		return sizeof(UInt) //Size field
			+ value.v_byte_array.size; // Array size
	case TagType::String:
		return sizeof(UShort) // Size field
			+ value.v_string.size; //String siza
	case TagType::List:
		for (; i < value.v_list.size; i++) {
			size += value.v_list.value[i].getSerializedSize();
		}
		return sizeof(UByte) // TagID
			+ sizeof(UInt) // Size
			+ size; // Items
	case TagType::Compound:
		for (auto &it : *value.v_compound) {
			size += 1 // Value type
				+ 2 // String size
				+ it.first.size() // String
				+ it.second.getSerializedSize(); // Value
		}
		return size
			+ sizeof(UByte); // End tag
	case TagType::IntArray:
		return sizeof(Short) // Size
			+ value.v_int_array.size * sizeof(Int); // Ints
	}
	return 0;
}


std::string Tag::write(bool write_type) const
{
	ULong index = 0;
	UInt i = 0;
	ULong size = getSerializedSize();

	if (write_type)
		size += sizeof(Byte);

	std::string byteStr;
	byteStr.resize(size);
	UByte *bytes = reinterpret_cast<UByte *>(&byteStr[0]);
	std::string str;

	if (write_type)
		writeByte((bytes + index++), (UByte) type);

	switch (type) {
	case TagType::End:
		break;
	case TagType::Byte:
		writeByte(bytes + index, value.v_byte);
		break;
	case TagType::Short:
		writeShort(bytes + index, value.v_short);
		break;
	case TagType::Int:
		writeInt(bytes + index, value.v_int);
		break;
	case TagType::Long:
		writeLong(bytes + index, value.v_long);
		break;
	case TagType::Float:
		writeFloat(bytes + index, value.v_float);
		break;
	case TagType::Double:
		writeDouble(bytes + index, value.v_double);
		break;
	case TagType::ByteArray:
		writeInt(bytes + index, value.v_byte_array.size);
		index += sizeof(Int);
		writeBytes(bytes + index, (UByte *) value.v_byte_array.value,
				value.v_byte_array.size);
		break;
	case TagType::String:
		writeString(bytes + index, value.v_string.value, value.v_string.size);
		break;
	case TagType::List:
		writeByte(bytes + index, (UByte) value.v_list.tagid);
		index += sizeof(Byte);
		writeInt(bytes + index, value.v_list.size);
		index += sizeof(Int);
		for (; i < value.v_list.size; i++) {
			str = value.v_list.value[i].write();
			writeBytes(bytes + index,
				reinterpret_cast<const UByte *>(str.data()),
				str.size());
			index += str.size();
		}
		break;
	case TagType::Compound:
		for (auto &it : *value.v_compound) {
			writeByte(bytes + index, (UByte) it.second.type);
			index += sizeof(Byte);
			writeString(bytes + index, it.first.data(), it.first.size());
			index += sizeof(Short) + it.first.size();
			str = it.second.write();
			writeBytes(bytes + index,
				reinterpret_cast<const UByte *>(str.data()),
				str.size());
			index += str.size();
		}
		writeByte(bytes + index, (UByte) TagType::End);
		break;
	case TagType::IntArray:
		writeInt(bytes + index, value.v_int_array.size);
		index += sizeof(Int);
		// Can't use memcpy, since you have to account for endianess
		for (; i < value.v_int_array.size; i++) {
			writeInt(bytes + index, value.v_int_array.value[i]);
			index += sizeof(Int);
		}
		break;
	}

	return byteStr;
}


std::string Tag::dump(const std::string &indent, UByte level) const
{
	bool first = true;
	std::ostringstream os;

	// String used to indent items in a list
	std::string idt_str;
	// String used to indent the close of a list
	std::string end_idt_str;
	// String used to seperate items in a list.
	// Placed after each item but before idt_str.
	const char *sep_str = ", ";

	if (!indent.empty()) {
		end_idt_str.reserve(1 + indent.size() * level);
		end_idt_str = '\n';
		for (UByte i = 0; i < level; ++i)
			end_idt_str += indent;
		idt_str = end_idt_str + indent;
		sep_str = ",";
	}

	switch (type) {
	case TagType::End:
		os << "<END>";
		break;
	case TagType::Byte:
		os << (Short) value.v_byte;
		break;
	case TagType::Short:
		os << value.v_short;
		break;
	case TagType::Int:
		os << value.v_int;
		break;
	case TagType::Long:
		os << value.v_long;
		break;
	case TagType::Float:
		os << value.v_float;
		break;
	case TagType::Double:
		os << value.v_double;
		break;
	case TagType::ByteArray:
		os << "byte[";
		for (UInt i = 0; i < value.v_byte_array.size; i++) {
			if (i != 0)
				os << sep_str;
			os << idt_str << (Short) value.v_byte_array.value[i];
		}
		os << end_idt_str << ']';
		break;
	case TagType::String:
		os << '"' << this->as<std::string>() << '"';
		break;
	case TagType::List:
		os << '[';
		for (UInt i = 0; i < value.v_list.size; i++) {
			if (i != 0)
				os << sep_str;
			os << idt_str << value.v_list.value[i].dump(indent, level+1);
		}
		os << end_idt_str << ']';
		break;
	case TagType::Compound:
		os << '{';
		for (auto &it : *value.v_compound) {
			if (!first)
				os << sep_str;
			first = false;
			os << idt_str << '"' << it.first << "\" = "
				<< it.second.dump(indent, level+1);
		}
		os << end_idt_str << '}';
		break;
	case TagType::IntArray:
		os << "int[";
		for (UInt i = 0; i < value.v_int_array.size; i++) {
			if (i != 0)
				os << sep_str;
			os << idt_str << value.v_int_array.value[i];
		}
		os << end_idt_str << ']';
		break;
	default:
		os << "<UNKNOWN TAG>";
	}
	return os.str();
}


inline void writeBytes(UByte * bytes, const UByte * write, UInt size)
{
	memcpy(bytes, write, size);
}


inline void writeByte(UByte * bytes, UByte b)
{
	bytes[0] = b;
}


inline void writeString(UByte * bytes, const char * str, UShort size)
{
	writeShort(bytes, size);
	writeBytes(bytes + sizeof(Short), (const UByte *) str, size);
}


/*******************
 * Deserialization *
 *******************/

void Tag::read(const UByte *bytes, bool compound)
{
	free();
	ULong index = 0;
	// Strictly, the root NBT tag must be Compound, but it's theoretically
	// possible to store other values directly as the root element.
	if (compound) {
		readTag(bytes, index, TagType::Compound);
	} else {
		TagType tag = (TagType) readByte(bytes);
		index += sizeof(Byte);
		readTag(bytes, index, tag);
	}
}


void Tag::readTag(const UByte *bytes, ULong &index, TagType tag)
{
	type = tag;
	switch (tag) {
	case TagType::End:
		break;
	case TagType::Byte:
		value.v_byte = readByte(bytes + index);
		index += sizeof(Byte);
		break;
	case TagType::Short:
		value.v_short = readShort(bytes + index);
		index += sizeof(Short);
		break;
	case TagType::Int:
		value.v_int = readInt(bytes + index);
		index += sizeof(Int);
		break;
	case TagType::Long:
		value.v_long = readLong(bytes + index);
		index += sizeof(Long);
		break;
	case TagType::Float:
		value.v_float = readFloat(bytes + index);
		index += sizeof(float);
		break;
	case TagType::Double:
		value.v_double = readDouble(bytes + index);
		index += sizeof(double);
		break;
	case TagType::ByteArray:
		value.v_byte_array = readByteArray(bytes, index);
		break;
	case TagType::String:
		value.v_string = readString(bytes, index);
		break;
	case TagType::List:
		value.v_list = readList(bytes, index);
		break;
	case TagType::Compound:
		value.v_compound = readCompound(bytes, index);
		break;
	case TagType::IntArray:
		value.v_int_array = readIntArray(bytes, index);
		break;
	default:
		throw std::runtime_error("Invalid tag type " +
			std::to_string((int)tag) +
			" at " + std::to_string(index));
	}
}


inline UByte readByte(const UByte * bytes)
{
	return bytes[0];
}


ByteArray readByteArray(const UByte *bytes, ULong &index)
{
	ByteArray x;
	x.size = readInt(bytes + index);
	index += sizeof(Int);
	x.value = new Byte[x.size];
	for (UInt i = 0; i < x.size; i++) {
		x.value[i] = readByte(bytes + index);
		index += sizeof(Byte);
	}
	return x;
}


String readString(const UByte *bytes, ULong &index)
{
	String x;
	x.size = readShort(bytes + index);
	index += sizeof(Short);
	if (!x.size)
		return x;
	x.value = new char[x.size];
	for (UShort i = 0; i < x.size; i++) {
		x.value[i] = readByte(bytes + index);
		index += sizeof(Byte);
	}
	return x;
}


List readList(const UByte *bytes, ULong &index)
{
	List x;
	x.tagid = (TagType) readByte(bytes + index);
	index += sizeof(Byte);
	x.size = readInt(bytes + index);
	index += sizeof(Int);
	if (x.size > 0) {
		x.value = new Tag[x.size];
	}
	for (UInt i = 0; i < x.size; i++) {
		x.value[i].readTag(bytes, index, x.tagid);
	}
	return x;
}


/*
 * TAG_Compound format:
 * TagType typeid = TagType::Compound
 * Repeat for each entry:
 *     TagType entrytype
 *     UShort keylen
 *     char key[keylen]
 *     TagType valtype
 *     Tag value
 * TagType entrytype = TagType::End
 */

Compound *readCompound(const UByte *bytes, ULong &index)
{
	Compound *x = new Compound;
	TagType tag;
	while (true) {
		tag = (TagType) readByte(bytes + index);
		index += sizeof(Byte);
		if (tag == TagType::End)
			break;

		String name = readString(bytes, index);
		std::string name_str;
		if (name.size)
			name_str.assign(name.value, name.size);

		(*x)[name_str].readTag(bytes, index, tag);

		if (name.size)
			delete [] name.value;
	}
	return x;
}


IntArray readIntArray(const UByte *bytes, ULong &index)
{
	IntArray x;
	x.size = readInt(bytes + index);
	index += sizeof(Int);
	if (x.size > 0) {
		x.value = new Int[x.size];
	}
	for (UInt i = 0; i < x.size; i++) {
		x.value[i] = readInt(bytes + index);
		index += sizeof(Int);
	}
	return x;
}

} // namespace NBT

