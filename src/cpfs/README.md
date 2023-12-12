CPFS
===

CPFS is a simple Cross-Platform FileSystem library for C++, loosly modeled after
C++17's std::filesystem (which is too new for some prople to use).

Documentation
---

See `src/cpfs.hpp` for documentation.

### Example

Here's a snippet of code to list all entries in a directory.  See `src/ls.cpp` for a full example.
```C++
for (const auto &entry : cpfs::DirIter("/tmp")) {
	std::cout << entry.name().utf8() << std::endl;
}
```
