#pragma once

#include <cstdint>
#include <cstdio>
#include <chrono>
#include <system_error>
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
#	define DIR_SEP '\\'
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#else
#	define DIR_SEP '/'
#	include <dirent.h>
#endif

namespace cpfs {

class UnicodeEncodeError;
class UnicodeDecodeError;
enum class FileType;
class Path;
class Status;
class DirEntry;
class DirIter;


class UnicodeEncodeError : std::runtime_error {
public:
	UnicodeEncodeError() :
		std::runtime_error("Unicode encode error in CPFS library.")
	{}
};

class UnicodeDecodeError : std::runtime_error {
public:
	UnicodeDecodeError(const char *str) :
		std::runtime_error("Unicode decode error in CPFS library: " +
				std::string(str))
	{}
};

class FilesystemError : std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};


// Public emun of file types.  Not all are supported on all platforms.
enum class FileType {
	BlockDevice,
	CharDevice,
	Directory,
	Fifo,
	Symlink,
	Regular,
	Socket,
	NotFound,
	Unknown,
};


// Holds a filesystem path in an OS specific format.
class Path {
public:
#ifdef _WIN32
	typedef wchar_t NativeChar;
#else
	typedef char NativeChar;
#endif

	// Creates a new Path from a UTF-8 string.
	Path(const char *utf8);
	Path(const std::string &utf8) : Path(utf8.c_str()) {}
#ifdef _WIN32
	// Creates a new Path from a native string.
	Path(const NativeChar *native) : path(native) {}
#endif
	// Copies a path.
	Path(const Path &other) = default;
	// Copies a path and appends to the new copy.
	Path(const Path &other, const Path &to_append);

	// Returns the Path as a string in the platform's native format.
	const NativeChar* native() const { return path.data(); }

	// Returns the length of the path.
	size_t size() const { return path.size(); }

	// Converts a Path to a new UTF-8 string.
	std::string utf8() const;

	// Appends a path component, seperated by the system's directory seperator.
	void append(const Path &other);

private:
	friend class DirIter;

	Path(const NativeChar *native, size_t size) :
		path(native, size)
	{}

	std::basic_string<NativeChar> path;
};


// Holds information about a filesystem entry
class Status {
public:
	// Returns whether the file exists
	operator bool () { return type() != FileType::NotFound; }

	// Retrieves the file type.
	FileType type() const;

	// Retrieves the file size.
	uint64_t size() const { return st.st_size; }

	// Retrieves the access time.
	std::chrono::system_clock::time_point atime() const;

	// Retrieves the modification time.
	std::chrono::system_clock::time_point mtime() const;

	// Retrieves the creation time.
	std::chrono::system_clock::time_point ctime() const;

private:
	friend Status status(const Path &path, std::error_code &ec) noexcept;

#ifdef _WIN32
	struct _stat64 st;
#else
	struct stat st;
#endif
	bool not_found = false;
};


class DirEntry {
public:
	// Returns the name of the entry.
	Path name() const;

	// Returns the type of the entry pointed to by the directory iterator.
	// Note: This is not always supported well if at all.  If the type can
	// not be determined FileType::Unknown will be returned.  You can use
	// status() to try to find the entry type if this fails.
	FileType type() const;

private:
	friend class DirIter;

#ifdef _WIN32
	WIN32_FIND_DATAW data;
#else
	struct dirent *entry = nullptr;
#endif
};

// Holds directory iteration state
class DirIter {
public:
	// Initializes a directory iterator for the directory at the specified path.
	DirIter() noexcept {};
	DirIter(const Path &path);
	DirIter(const Path &path, std::error_code &ec) noexcept { init(path, ec); }
	DirIter(DirIter &&other) noexcept;

	// Releases resources used by a directory iterator.
	~DirIter();

	// Advances the directory iterator to the next item in the directory.
	void operator ++ ();
	bool operator == (const DirIter &other) const;
	bool operator != (const DirIter &other) const { return !(*this == other); }
	const DirEntry & operator * () const { return entry; }
	const DirEntry * operator -> () const { return &entry; }
	DirIter &&begin() { return std::move(*this); }
	DirIter end() const { return DirIter(); }

private:
	void init(const Path &path, std::error_code &ec) noexcept;

#ifdef _WIN32
	HANDLE handle = INVALID_HANDLE_VALUE;
#else
	DIR *dir = nullptr;
#endif
	DirEntry entry;
};


// Retreives information about a file.
Status status(const Path &path, std::error_code &ec) noexcept;
Status status(const Path &path);


// Checks if the path exists.
bool exists(const Path &path, std::error_code &ec) noexcept;
bool exists(const Path &path) noexcept;

// Checks if the paths points to an existing regular file.
bool is_file(const Path &path, std::error_code &ec) noexcept;
bool is_file(const Path &path) noexcept;

// Checks if the path points to an existing directory.
bool is_directory(const Path &path, std::error_code &ec) noexcept;
bool is_directory(const Path &path) noexcept;


// Attempts to creates a directory at the specified path.
// Returns whether the directory was successfully created.
bool create_directory(const Path &path, std::error_code &ec) noexcept;
bool create_directory(const Path &path);

// Opens a new file with the specified mode.
// Like fopen(), but supports wide chars on Windows.
FILE* file_open(const Path &path, const char *mode);

// Removes a file or empty directory.  Returns whether the operation succeeded.
bool remove(const Path &path, std::error_code &ec) noexcept;
bool remove(const Path &path);

// Recursively removes all files and directories in a directory tree.
// Returns whether the operation succeeded.
bool remove_recursive(const Path &path, std::error_code &ec) noexcept;
bool remove_recursive(const Path &path);

} // namespace cpfs
