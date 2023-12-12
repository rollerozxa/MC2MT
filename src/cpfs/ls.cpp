#include "cpfs.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <ctime>


#define CHECK_EC(ec) do { \
	if (ec) { \
		printf("error %d: %s", ec.value(), ec.message().c_str()); \
		return ec.value(); \
	} \
} while (0)


void print_size(size_t size)
{
	if (size < 10000) {
		printf("%4u", (unsigned)size);
		return;
	}
	static const char postfixes[] = {'K', 'M', 'G', 'T', 'P', 'E'};
	unsigned pidx = 0;
	size /= 1000;
	while (pidx < sizeof(postfixes) && size >= 1000) {
		++pidx;
		size /= 1000;
	}
	printf("%3u%c", (unsigned)size, postfixes[pidx]);
}

int main(int argc, char *argv[])
{
	cpfs::Path path(argc > 1 ? argv[1] : ".");

#ifdef WIN32
	printf("T|Size|   Last Mod Time    | Filename\n");
#else
	printf("T|Size|      Last Modification Time      | Filename\n");
#endif

	std::error_code ec;
	for (const auto &e : cpfs::DirIter(path, ec)) {
		CHECK_EC(ec);
		cpfs::Path entry_name = e.name();
		std::string entry_utf8 = entry_name.utf8();

		if (entry_utf8 == "." || entry_utf8 == "..") {
			continue;
		}

		char tp = e.type() == cpfs::FileType::Directory ? 'd' : '-';

		cpfs::Path entry_path(path, entry_name);
		cpfs::Status st = cpfs::status(entry_path, ec);
		CHECK_EC(ec);
		if (st.type() == cpfs::FileType::NotFound) {
			return 1;
		}

		auto mtime = st.mtime().time_since_epoch();
		time_t mtime_time = std::chrono::duration_cast<std::chrono::seconds    >(mtime).count();
#ifdef WIN32
		// Windows's %z format specifier is unpredictable, and it doesn't support nanosecond timestamps.
		struct tm *mtime_tm = gmtime(&mtime_time);
		char mtime_str[sizeof("YYYY-MM-DDTHH:MM:SSZ")];
		strftime(mtime_str, sizeof(mtime_str), "%Y-%m-%dT%H:%M:%SZ", mtime_tm);
#else
		time_t mtime_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(mtime).count() % 1000000000;

		struct tm *mtime_tm = localtime(&mtime_time);
		char mtime_str[sizeof("YYYY-MM-DDTHH:MM:SS.000000000-0000")];
		strftime(mtime_str, sizeof(mtime_str), "%Y-%m-%dT%H:%M:%S.*********%z", mtime_tm);

		char mnano_str[10];
		snprintf(mnano_str, sizeof(mnano_str), "%09" PRIu64, mtime_nano);
		memcpy(mtime_str + sizeof("YYYY-MM-DDTHH:MM:SS"), mnano_str, 9);
#endif

		printf("%c ", tp);
		print_size(st.size());
		printf(" %s %s\n", mtime_str, entry_utf8.c_str());
	}
	return 0;
}
