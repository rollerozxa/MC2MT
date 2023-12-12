
#ifndef NBT_HEADER
#define NBT_HEADER

#include <cstdint>
#include <cassert>
#include <exception>
#include <string>
#include <map>
#include <limits>

// Require C++11
#if __cplusplus < 201103L
	#error NBT-CPP requires C++11
#endif

static_assert(std::numeric_limits<float>::is_iec559 && sizeof(float) == 4,
		"NBT depends on IEEE-754 32-bit floats");
static_assert(std::numeric_limits<double>::is_iec559 && sizeof(double) == 8,
		"NBT depends on IEEE-754 64-bit doubles");


namespace NBT {

/*********
 * Types *
 *********/

class Tag;

typedef int8_t  Byte;
typedef int16_t Short;
typedef int32_t Int;
typedef int64_t Long;
typedef uint8_t  UByte;
typedef uint16_t UShort;
typedef uint32_t UInt;
typedef uint64_t ULong;

enum class TagType : UByte {
	End,
	Byte,
	Short,
	Int,
	Long,
	Float,
	Double,
	ByteArray,
	String,
	List,
	Compound,
	IntArray,
};

struct ByteArray {
	UInt size;
	Byte *value;
};

struct String {
	UShort size;
	char *value;
};

struct List {
	TagType tagid;
	UInt size;
	Tag *value;
};

typedef std::map<std::string, Tag> Compound;

struct IntArray {
	UInt size;
	Int *value;
};

union Value {
	Byte      v_byte;
	Short     v_short;
	Int       v_int;
	Long      v_long;
	float     v_float;
	double    v_double;
	ByteArray v_byte_array;
	String    v_string;
	List      v_list;
	Compound *v_compound;
	IntArray  v_int_array;
};

class Tag {
public:
	Tag() : type(TagType::End), value() {}
	Tag(const UByte *bytes, bool compound=true);
	Tag(const char *bytes) = delete;
	//Tag(const std::string &bytes) : Tag(bytes.data()) {}
	//Tag(const std::basic_string<unsigned char> &bytes) : Tag(bytes.data()) {}
	Tag(const TagType tag, UInt size = 0, TagType subtype = TagType::End);

	Tag(Byte x)   : type(TagType::Byte)   { value.v_byte = x; }
	Tag(Short x)  : type(TagType::Short)  { value.v_short = x; }
	Tag(Int x)    : type(TagType::Int)    { value.v_int = x; }
	Tag(Long x)   : type(TagType::Long)   { value.v_long = x; }
	Tag(float x)  : type(TagType::Float)  { value.v_float = x; }
	Tag(double x) : type(TagType::Double) { value.v_double = x; }
	Tag(const std::string &x);

	Tag(const Tag &t) : type(TagType::End) { copy(t); }
	Tag(Tag &&t) : type(t.type), value(t.value)
		{ t.type = TagType::End; }

	~Tag() { free(); }

	Tag & operator = (const Tag &t);
	Tag & operator = (Tag &&t);
	Tag & operator [] (const Int &k);
	Tag & operator [] (const Int &k) const;
	Tag & operator [] (const std::string &k) { assert(type == TagType::Compound); return (*value.v_compound)[k]; }
	Tag & operator [] (const std::string &k) const { assert(type == TagType::Compound); return value.v_compound->at(k); }
	Tag & operator [] (const char *k) { assert(type == TagType::Compound); return (*value.v_compound)[k]; }
	Tag & operator [] (const char *k) const { assert(type == TagType::Compound); return value.v_compound->at(k); }

	Tag & operator += (const Byte &t);
	Tag & operator += (const Int &t);
	Tag & operator += (const Tag &t);
	Tag & operator += (Tag &&t);

	operator Byte      () const { assert(type == TagType::Byte);      return value.v_byte; }
	operator Short     () const { assert(type == TagType::Short);     return value.v_short; }
	operator Int       () const { assert(type == TagType::Int);       return value.v_int; }
	operator Long      () const { assert(type == TagType::Long);      return value.v_long; }
	operator float     () const { assert(type == TagType::Float);     return value.v_float; }
	operator double    () const { assert(type == TagType::Double);    return value.v_double; }
	operator ByteArray () const { assert(type == TagType::ByteArray); return value.v_byte_array; }
	operator String    () const { assert(type == TagType::String);    return value.v_string; }
	operator List      () const { assert(type == TagType::List);      return value.v_list; }
	operator Compound& () const { assert(type == TagType::Compound);  return *value.v_compound; }
	operator IntArray  () const { assert(type == TagType::IntArray);  return value.v_int_array; }

	operator std::string () const {
		assert(type == TagType::String);
		if (value.v_string.size)
			return std::string(value.v_string.value, value.v_string.size);
		return std::string();
	}

	template <typename T> T as() const { return static_cast<T>(*this); }

	void copy(const Tag &t);
	void free();
	void setTag(const TagType tag, UInt size = 0, TagType subtype = TagType::End);

	void read(const UByte *bytes, bool compound=true);
	std::string write(bool write_type=false) const;
	std::string dump(const std::string &indent="\t", UByte level=0) const;

	void insert(const Int k, const Byte b);
	void insert(const Int k, const Int i);
	void insert(const Int k, const Tag &t);
	void insert(const std::string &k, const Tag &t);

	TagType type;

protected:
	void readTag(const UByte *bytes, ULong &index, TagType tag);

	friend List      readList    (const UByte *bytes, ULong &index);
	friend Compound *readCompound(const UByte *bytes, ULong &index);

	ULong getSerializedSize() const;
	template <typename container, typename contained>
		void ensureSize(container *field, UInt size);

	Value value;
};

} // namespace NBT

#endif

