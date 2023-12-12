#define _DEFAULT_SOURCE  // For DT_*
#define _FILE_OFFSET_BITS 64

#include "cpfs.hpp"

#include <cstdarg>
#include <cassert>
#include <cstdlib>
#include <cstring>

#ifndef _WIN32
#	include <unistd.h>
#endif


static int (* const sys_remove)(const char *) = remove;

namespace cpfs {

static void remove_subitems(const Path &path, std::error_code &ec) noexcept;

#define NO_EC_WRAPPER(ret_type, func_name, err_msg) \
	ret_type func_name(const Path &path) \
	{ \
		std::error_code ec; \
		ret_type ret = func_name(path, ec); \
		if (ec) { \
			throw FilesystemError(err_msg \
					"\nPath: " + path.utf8() + \
					"\nError:" + ec.message()); \
		} \
		return ret; \
	}

#ifdef _WIN32

#ifndef MB_ERR_INVALID_CHARS
#define MB_ERR_INVALID_CHARS 0
#endif

#ifndef WC_ERR_INVALID_CHARS
#define WC_ERR_INVALID_CHARS 0
#endif


static std::wstring utf8_to_wide(const char *str)
{
	int wlen = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
			str, -1, NULL, 0);
	if (wlen == 0) {
		throw UnicodeDecodeError(str);
	}
	// wlen includes the NUL terminator, which wstring doesn't count.
	std::wstring wstr(wlen-1, L'\0');
	int res = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
			str, -1, &wstr[0], wlen);
	if (res == 0) {
		throw UnicodeDecodeError(str);
	}
	return wstr;
}

static std::string wide_to_utf8(const std::basic_string<wchar_t> &wstr)
{
	int len = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
			wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (len == 0) {
		throw UnicodeEncodeError();
	}
	std::string str(len, '\0');
	int res = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
			wstr.c_str(), -1, &str[0], len, NULL, NULL);
	if (res == 0) {
		throw UnicodeEncodeError();
	}
	return str;
}

#endif  // _WIN32


static inline void assign_syserror(std::error_code &ec)
{
#ifdef _WIN32
	ec.assign(GetLastError(), std::system_category());
#else
	ec.assign(errno, std::system_category());
#endif
}

Path::Path(const char *utf8)
{
#ifdef _WIN32
	path = utf8_to_wide(utf8);
#else
	path = utf8;
#endif
}

Path::Path(const Path &other, const Path &to_append) :
	Path(other)
{
	append(to_append);
}


std::string Path::utf8() const
{
#ifdef _WIN32
	return wide_to_utf8(path);
#else
	return path;
#endif
}

void Path::append(const Path &other)
{
	if (size() > 0 && other.size() > 0) {
		path.push_back(DIR_SEP);
	}
	path.append(other.path);
}


Status status(const Path &path, std::error_code &ec) noexcept
{
	ec.clear();
	Status st;
#ifdef _WIN32
	if (_wstat64(path.native(), &st.st) != 0) {
		st.not_found = true;
		if (errno == ENOENT) {
			ec.assign(ERROR_PATH_NOT_FOUND, std::system_category());
		} else {
			ec.assign(ERROR_INVALID_PARAMETER, std::system_category());
		}
	}
#else
	if (stat(path.native(), &st.st) == -1) {
		st.not_found = true;
		assign_syserror(ec);
	}
#endif
	return st;
}

NO_EC_WRAPPER(Status, status, "Failed to retreive file status.")

FileType Status::type() const
{
	if (not_found) {
		return FileType::NotFound;
	}
#ifdef _WIN32
	switch (st.st_mode & _S_IFMT) {
	case _S_IFDIR: return FileType::Directory;
	case _S_IFREG: return FileType::Regular;
	};
	return FileType::Unknown;
#else
	switch (st.st_mode & S_IFMT) {
	case S_IFBLK:  return FileType::BlockDevice;
	case S_IFCHR:  return FileType::CharDevice;
	case S_IFDIR:  return FileType::Directory;
	case S_IFIFO:  return FileType::Fifo;
	case S_IFLNK:  return FileType::Symlink;
	case S_IFREG:  return FileType::Regular;
	case S_IFSOCK: return FileType::Socket;
	};
	return FileType::Unknown;
#endif
}


#ifdef st_atime
// st_*time are alliased to st_*tim, grab the values directly
#define GET_STAT_TIME(t) std::chrono::system_clock::time_point( \
		std::chrono::seconds(st.st_##t##tim.tv_sec) + \
		std::chrono::nanoseconds(st.st_##t##tim.tv_nsec))
#else
#define GET_STAT_TIME(t) std::chrono::system_clock::time_point( \
		std::chrono::seconds(st.st_##t##time))
#endif


std::chrono::system_clock::time_point Status::atime() const { return GET_STAT_TIME(a); }
std::chrono::system_clock::time_point Status::mtime() const { return GET_STAT_TIME(m); }
std::chrono::system_clock::time_point Status::ctime() const { return GET_STAT_TIME(c); }


#define STATUS_WRAPPER(func_name, cond) \
	bool func_name(const Path &path, std::error_code &ec) noexcept \
	{ \
		return status(path, ec).cond; \
	} \
	bool func_name(const Path &path) noexcept \
	{ \
		std::error_code ec; \
		return status(path, ec).cond; \
	}

STATUS_WRAPPER(exists, type() != FileType::NotFound)
STATUS_WRAPPER(is_file, type() == FileType::Regular)
STATUS_WRAPPER(is_directory, type() == FileType::Directory)


bool create_directory(const Path &path, std::error_code &ec) noexcept
{
	ec.clear();
#ifdef _WIN32
	if (!CreateDirectoryW(path.native(), NULL)) {
		assign_syserror(ec);
		return false;
	}
#else
	if (mkdir(path.native(), 0777) != 0) {
		assign_syserror(ec);
		return false;
	}
#endif
	return true;
}

NO_EC_WRAPPER(bool, create_directory, "Failed to create directory.")


FILE* file_open(const Path &path, const char *mode)
{
#ifdef _WIN32
	// _wfopen requires the mode to also be a wide string, even though no
	// wide characters are supported.  We'll just do a quick assignment
	// instead of a full-blown encoding conversion.
	wchar_t wmode[4];
	for (unsigned i = 0; mode[i] != '\0'; ++i) {
		assert(i < sizeof(wmode) / sizeof(*wmode));
		wmode[i] = (wchar_t) mode[i];
	}

	return _wfopen(path.native(), wmode);
#else
	return fopen(path.native(), mode);
#endif
}


bool remove(const Path &path, std::error_code &ec) noexcept
{
	ec.clear();
	bool ok = true;
#ifdef _WIN32
	if (is_directory(path, ec)) {
		ok = RemoveDirectoryW(path.native());
	} else {
		if (ec) return false;
		ok = DeleteFileW(path.native());
	}
#else
	ok = sys_remove(path.native()) == 0;
#endif
	if (!ok) {
		assign_syserror(ec);
	}
	return ok;
}

NO_EC_WRAPPER(bool, remove, "Failed to remove file/directory.")

bool remove_recursive(const Path &path, std::error_code &ec) noexcept
{
	ec.clear();
	if (is_directory(path, ec)) {
		remove_subitems(path, ec);
		if (ec) return false;
	}
	if (ec) return false;
	return remove(path, ec);
}

NO_EC_WRAPPER(bool, remove_recursive, "Failed to recursively remove directory tree.")

// Helper for remove_recursive, assumes that the path points to a directory and
// only deletes its contents (not the directory itself).
static void remove_subitems(const Path &path, std::error_code &ec) noexcept
{
	for (const DirEntry &e : DirIter(path, ec)) {
		if (ec) return;
		Path entry_path(path, e.name());
		if (e.type() == FileType::Directory) {
			remove_subitems(entry_path, ec);
			if (ec) return;
		}
		remove(entry_path, ec);
		if (ec) return;
	}
}

DirIter::DirIter(const Path &path)
{
	std::error_code ec;
	init(path, ec);
	if (ec) {
		throw FilesystemError("Failed to create directory iterator."
			"\nPath: " + path.utf8() +
			"\nError:" + ec.message());
	}
}


void DirIter::init(const Path &path, std::error_code &ec) noexcept
{
	ec.clear();
#ifdef _WIN32
	wchar_t *match = new wchar_t[path.size() + 3];
	memcpy(match, path.native(), path.size() * sizeof(wchar_t));
	match[path.size()] = L'\\';
	match[path.size()+1] = L'*';
	match[path.size()+2] = L'\0';
	handle = FindFirstFileW(match, &entry.data);
	delete[] match;
	if (handle == INVALID_HANDLE_VALUE) {
		assign_syserror(ec);
	}
#else
	dir = opendir(path.native());
	if (dir == nullptr) {
		assign_syserror(ec);
	} else {
		errno = 0;
		entry.entry = readdir(dir);
		if (entry.entry == nullptr && errno != 0) {
			assign_syserror(ec);
		}
	}
#endif
}

DirIter::DirIter(DirIter &&other) noexcept
{
#ifdef _WIN32
	handle = other.handle;
	entry.data = other.entry.data;
	other.handle = INVALID_HANDLE_VALUE;
#else
	dir = other.dir;
	entry.entry = other.entry.entry;
	other.dir = nullptr;
	other.entry.entry = nullptr;
#endif
}

DirIter::~DirIter()
{
#ifdef _WIN32
	if (handle != INVALID_HANDLE_VALUE) {
		FindClose(handle);
	}
#else
	closedir(dir);
#endif
}

void DirIter::operator ++ ()
{
#ifdef _WIN32
	if (FindNextFileW(handle, &entry.data) == 0) {
		int err = GetLastError();
		FindClose(handle);
		handle = INVALID_HANDLE_VALUE;
		throw std::error_code(err, std::system_category());
	}
#else
	errno = 0;
	entry.entry = readdir(dir);
	if (entry.entry == nullptr && errno != 0) {
		throw std::error_code(errno, std::system_category());
	}
#endif
}

bool DirIter::operator == (const DirIter &other) const
{
#ifdef _WIN32
	return handle == other.handle;
#else
	return entry.entry == other.entry.entry;
#endif
}

Path DirEntry::name() const
{
#ifdef _WIN32
	return Path(data.cFileName);
#else
	return Path(entry->d_name);
#endif
}


FileType DirEntry::type() const
{
#ifdef _WIN32
	return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ?
		FileType::Directory : FileType::Regular;
#elif defined(_DIRENT_HAVE_D_TYPE)
	switch (entry->d_type) {
	case DT_BLK:  return FileType::BlockDevice;
	case DT_CHR:  return FileType::CharDevice;
	case DT_DIR:  return FileType::Directory;
	case DT_FIFO: return FileType::Fifo;
	case DT_LNK:  return FileType::Symlink;
	case DT_REG:  return FileType::Regular;
	case DT_SOCK: return FileType::Socket;
	}
	return FileType::Unknown;
#else
	return FileType::Unknown;
#endif
}

} // namespace cpfs
