

#ifndef Foundation_File_UNIX_INCLUDED
#define Foundation_File_UNIX_INCLUDED


//#include "Poco/Foundation.h"

#include "timestamp.h"

#include <string>

namespace nanogear { namespace platform
{

class File
{
protected:
	typedef uint64_t FileSize;

	File();
	File(const std::string& path);
	virtual ~File();
	void swap(File& file);
	void setPath(const std::string& path);
	const std::string& getPath() const;
	bool exists() const;
	bool canRead() const;
	bool canWrite() const;
	bool canExecute() const;
	bool isFile() const;
	bool isDirectory() const;
	bool isLink() const;
	bool isDevice() const;
	bool isHidden() const;
   
	Timestamp created() const;
	Timestamp getLastModified() const;
	void setLastModified(const Timestamp& ts);
	
   FileSize getSize() const;
	void setSize(FileSize size);
	void setWriteable(bool flag = true);
	void setExecutable(bool flag = true);
	void copyTo(const std::string& path) const;
	void renameTo(const std::string& path);
	void remove();
	bool createFile();
	bool createDirectory();
	static void handleLastError(const std::string& path);
	
private:
	std::string _path;
	
	friend class DirectoryIteratorImpl;
	friend class LinuxDirectoryWatcherStrategy;
	friend class BSDDirectoryWatcherStrategy;
};


//
// inlines
//
inline const std::string& File::getPath() const
{
	return _path;
}

} // namespace platform
} // namespace nanogear


#endif // Foundation_File_UNIX_INCLUDED
