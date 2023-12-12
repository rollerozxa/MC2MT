
#include <cstdint>
#include <cassert>

#include <zlib.h>

#include "compression.hpp"

namespace NBT {

constexpr std::size_t cmp_buf_size = 128 * 1024;

bool compress(std::string * out, const char * in, size_t size, int level,
		CompressionFormat format)
{
	int res = 0;
	unsigned char temp_buffer[cmp_buf_size];

	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.next_in = reinterpret_cast<unsigned char *>(const_cast<char *>(in));
	strm.avail_in = size;

	int ws = 15;
	if (format == CompressionFormat::GZip)
		ws += 16;

	if ((res = deflateInit2(&strm, level, Z_DEFLATED, ws, 8,
			Z_DEFAULT_STRATEGY)) != Z_OK) {
		*out = "Error initializing stream: ";
		*out += zError(res);
		return false;
	}

	do {
		strm.avail_out = sizeof(temp_buffer);
		strm.next_out = temp_buffer;

		res = deflate(&strm, Z_FINISH);

		if (res != Z_OK && res != Z_STREAM_END)
			break;

		// avail_out is amount of *unused* space in next_out
		size_t count = sizeof(temp_buffer) - strm.avail_out;
		auto signed_buf = reinterpret_cast<const char *>(temp_buffer);
		out->append(signed_buf, count);
	} while (res == Z_OK);

	if (res != Z_STREAM_END) {
		*out = "Deflation error: ";
		*out += zError(res);
		return false;
	}

	(void) deflateEnd(&strm);

	return true;
}


bool decompress(std::string * out, const char * in, size_t size)
{
	int res = 0;
	unsigned char temp_buffer[cmp_buf_size];

	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.next_in = reinterpret_cast<unsigned char *>(const_cast<char *>(in));
	strm.avail_in = size;

	if ((res = inflateInit2(&strm, 15 + 32)) != Z_OK) {
		*out = "Error initializing stream: ";
		*out += zError(res);
		return false;
	}

	do {
		strm.next_out = temp_buffer;
		strm.avail_out = sizeof(temp_buffer);

		res = inflate(&strm, Z_NO_FLUSH);

		if (res != Z_OK && res != Z_STREAM_END)
			break;

		// avail_out is amount of *unused* space in next_out
		size_t count = sizeof(temp_buffer) - strm.avail_out;
		auto signed_buf = reinterpret_cast<const char *>(temp_buffer);
		out->append(signed_buf, count);
	} while (res == Z_OK);

	if (res != Z_STREAM_END) {
		*out = "Inflation error: ";
		*out += zError(res);
		return false;
	}

	(void) inflateEnd(&strm);

	return true;
}

} // namespace NBT

