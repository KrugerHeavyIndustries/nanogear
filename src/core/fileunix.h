

#ifndef Foundation_File_UNIX_INCLUDED
#define Foundation_File_UNIX_INCLUDED


//#include "Poco/Foundation.h"

#include "timestamp.h"

#include <string>

namespace nanogear
{

class FileImpl
{
protected:
	typedef uint64_t FileSizeImpl;

	FileImpl();
	FileImpl(const std::string& path);
	virtual ~FileImpl();
	void swapImpl(FileImpl& file);
	void setPathImpl(const std::string& path);
	const std::string& getPathImpl() const;
	bool existsImpl() const;
	bool canReadImpl() const;
	bool canWriteImpl() const;
	bool canExecuteImpl() const;
	bool isFileImpl() const;
	bool isDirectoryImpl() const;
	bool isLinkImpl() const;
	bool isDeviceImpl() const;
	bool isHiddenImpl() const;
   
	Timestamp createdImpl() const;
	Timestamp getLastModifiedImpl() const;
	void setLastModifiedImpl(const Timestamp& ts);
	
   FileSizeImpl getSizeImpl() const;
	void setSizeImpl(FileSizeImpl size);
	void setWriteableImpl(bool flag = true);		
	void setExecutableImpl(bool flag = true);		
	void copyToImpl(const std::string& path) const;
	void renameToImpl(const std::string& path);
	void removeImpl();
	bool createFileImpl();
	bool createDirectoryImpl();
	static void handleLastErrorImpl(const std::string& path);
	
private:
	std::string _path;
	
	friend class DirectoryIteratorImpl;
	friend class LinuxDirectoryWatcherStrategy;
	friend class BSDDirectoryWatcherStrategy;
};


//
// inlines
//
inline const std::string& FileImpl::getPathImpl() const
{
	return _path;
}


} // namespace Poco


#endif // Foundation_File_UNIX_INCLUDED
