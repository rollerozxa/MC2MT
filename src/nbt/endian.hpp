// Wrappers for cross-platform host<=>big-endian conversion functions

#ifndef NBT_ENDIAN_HEADER
#define NBT_ENDIAN_HEADER

#if defined(__linux__) || defined(__CYGWIN__)
	#include <endian.h>
#elif defined(__APPLE__)
	#include <libkern/OSByteOrder.h>
	#define htobe16(x) OSSwapHostToBigInt16(x)
	#define be16toh(x) OSSwapBigToHostInt16(x)
	#define htobe32(x) OSSwapHostToBigInt32(x)
	#define be32toh(x) OSSwapBigToHostInt32(x)
	#define htobe64(x) OSSwapHostToBigInt64(x)
	#define be64toh(x) OSSwapBigToHostInt64(x)
#elif defined(__OpenBSD__)
	#include <sys/endian.h>
#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)
	#include <sys/endian.h>

	#define be16toh(x) betoh16(x)
	#define be32toh(x) betoh32(x)
	#define be64toh(x) betoh64(x)
#elif defined(_WIN32)
	#include <winsock2.h>
	#include <sys/param.h>

	#define htobe16(x) htons(x)
	#define be16toh(x) ntohs(x)
	#define htobe32(x) htonl(x)
	#define be32toh(x) ntohl(x)

	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		#define htobe64(x) (((uint64_t)htonl((uint64_t)(x) & 0xFFFFFFFF) << 32) | htonl((uint64_t)(x) >> 32))
		#define be64toh(x) (((uint64_t)ntohl((uint64_t)(x) & 0xFFFFFFFF) << 32) | ntohl((uint64_t)(x) >> 32))
	#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		#define htobe64(x) x
		#define be64toh(x) x
	#else
		#error "Only big and little endian supported on Windows!"
	#endif
#else
	#error Platform not supported!
#endif

#endif // NBT_ENDIAN_HEADER
