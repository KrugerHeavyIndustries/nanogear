//
// File_UNIX.cpp
//
// $Id: //poco/1.4/Foundation/src/File_UNIX.cpp#1 $
//
// Library: Foundation
// Package: Filesystem
// Module:  File
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "fileunix.h"
#include "path.h"
#include "timestamp.h"
//#include "Poco/Buffer.h"
//#include "Poco/Exception.h"
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <utime.h>
#include <cstring>

namespace nanogear { namespace platform
{

File::File()
{
}

File::File(const std::string& path): _path(path)
{
	std::string::size_type n = _path.size();
	if (n > 1 && _path[n - 1] == '/')
		_path.resize(n - 1);
}

File::~File()
{
}

void File::swap(File& file)
{
	std::swap(_path, file._path);
}

void File::setPath(const std::string& path)
{
	_path = path;
	std::string::size_type n = _path.size();
	if (n > 1 && _path[n - 1] == '/')
		_path.resize(n - 1);
}

bool File::exists() const
{
	//poco_assert (!_path.empty());

	struct stat st;
	return stat(_path.c_str(), &st) == 0;
}

bool File::canRead() const
{
	//poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid())
			return (st.st_mode & S_IRUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IRGRP) != 0;
		else
			return (st.st_mode & S_IROTH) != 0 || geteuid() == 0;
	}
	else handleLastError(_path);
	return false;
}

bool File::canWrite() const
{
	//poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid())
			return (st.st_mode & S_IWUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IWGRP) != 0;
		else
			return (st.st_mode & S_IWOTH) != 0 || geteuid() == 0;
	}
	else handleLastError(_path);
	return false;
}

bool File::canExecute() const
{
	//poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
	{
		if (st.st_uid == geteuid() || geteuid() == 0)
			return (st.st_mode & S_IXUSR) != 0;
		else if (st.st_gid == getegid())
			return (st.st_mode & S_IXGRP) != 0;
		else
			return (st.st_mode & S_IXOTH) != 0;
	}
	else handleLastError(_path);
	return false;
}

bool File::isFile() const
{
	//poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISREG(st.st_mode);
	else
		handleLastError(_path);
	return false;
}

bool File::isDirectory() const
{
	//poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISDIR(st.st_mode);
	else
		handleLastError(_path);
	return false;
}

bool File::isLink() const
{
	//poco_assert (!_path.empty());

	struct stat st;
	if (lstat(_path.c_str(), &st) == 0)
		return S_ISLNK(st.st_mode);
	else
		handleLastError(_path);
	return false;
}

bool File::isDevice() const
{
	//poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode);
	else
		handleLastError(_path);
	return false;
}

bool File::isHidden() const
{
	//poco_assert (!_path.empty());
	Path p(_path);
	p.makeFile();

	return p.getFileName()[0] == '.';
}

Timestamp File::created() const
{
	//poco_assert (!_path.empty());

#if defined(__APPLE__) && defined(st_birthtime) && !defined(POCO_NO_STAT64) // st_birthtime is available only on 10.5
	struct stat64 st;
	if (stat64(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_birthtime);
#elif defined(__FreeBSD__)
	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_birthtime);
#else
	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_ctime);
#endif 
	else
		handleLastError(_path);
	return 0;
}

Timestamp File::getLastModified() const
{
	//poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return Timestamp::fromEpochTime(st.st_mtime);
	else
		handleLastError(_path);
	return 0;
}

void File::setLastModified(const Timestamp& ts)
{
	//poco_assert (!_path.empty());

	struct utimbuf tb;
	tb.actime  = ts.epochTime();
	tb.modtime = ts.epochTime();
	if (utime(_path.c_str(), &tb) != 0)
		handleLastError(_path);
}

File::FileSize File::getSize() const
{
	//poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) == 0)
		return st.st_size;
	else
		handleLastError(_path);
	return 0;
}

void File::setSize(FileSize size)
{
	//poco_assert (!_path.empty());

	if (truncate(_path.c_str(), size) != 0)
		handleLastError(_path);
}

void File::setWriteable(bool flag)
{
	//poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) != 0) 
		handleLastError(_path);
	mode_t mode;
	if (flag)
	{
		mode = st.st_mode | S_IWUSR;
	}
	else
	{
		mode_t wmask = S_IWUSR | S_IWGRP | S_IWOTH;
		mode = st.st_mode & ~wmask;
	}
	if (chmod(_path.c_str(), mode) != 0) 
		handleLastError(_path);
}

void File::setExecutable(bool flag)
{
	//poco_assert (!_path.empty());

	struct stat st;
	if (stat(_path.c_str(), &st) != 0) 
		handleLastError(_path);
	mode_t mode;
	if (flag)
	{
		mode = st.st_mode | S_IXUSR;
	}
	else
	{
		mode_t wmask = S_IXUSR | S_IXGRP | S_IXOTH;
		mode = st.st_mode & ~wmask;
	}
	if (chmod(_path.c_str(), mode) != 0) 
		handleLastError(_path);
}

void File::copyTo(const std::string& path) const
{
	//poco_assert (!_path.empty());

	int sd = open(_path.c_str(), O_RDONLY);
	if (sd == -1) handleLastError(_path);

	struct stat st;
	if (fstat(sd, &st) != 0) 
	{
		close(sd);
		handleLastError(_path);
	}
	const long blockSize = st.st_blksize;

	int dd = open(path.c_str(), O_CREAT | O_TRUNC | O_WRONLY, st.st_mode);
	if (dd == -1)
	{
		close(sd);
		handleLastError(path);
	}
   std::vector<char> buffer(blockSize);
   try
	{
		int n;
      while ((n = read(sd, buffer.data(), blockSize)) > 0)
		{
			if (write(dd, buffer.data(), n) != n)
				handleLastError(path);
		}
		if (n < 0)
			handleLastError(_path);
	}
	catch (...)
	{
		close(sd);
		close(dd);
		throw;
	}
	close(sd);
	if (fsync(dd) != 0) 
	{
		close(dd);
		handleLastError(path);
	}
	if (close(dd) != 0)
		handleLastError(path);
}

void File::renameTo(const std::string& path)
{
	//poco_assert (!_path.empty());

	if (rename(_path.c_str(), path.c_str()) != 0)
		handleLastError(_path);
}

void File::remove()
{
	//poco_assert (!_path.empty());

	int rc;
	if (!isLink() && isDirectory())
		rc = rmdir(_path.c_str());
	else
		rc = unlink(_path.c_str());
	if (rc) handleLastError(_path);
}

bool File::createFile()
{
	//poco_assert (!_path.empty());
	
	int n = open(_path.c_str(), O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (n != -1)
	{
		close(n);
		return true;
	}
	if (n == -1 && errno == EEXIST)
		return false;
	else
		handleLastError(_path);
	return false;
}

bool File::createDirectory()
{
	//poco_assert (!_path.empty());

	if (exists() && isDirectory())
		return false;
	if (mkdir(_path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0) 
		handleLastError(_path);
	return true;
}

void File::handleLastError(const std::string& path)
{
   /*
	switch (errno)
	{
	case EIO:
		throw IOException(path, errno);
	case EPERM:
		throw FileAccessDeniedException("insufficient permissions", path, errno);
	case EACCES:
		throw FileAccessDeniedException(path, errno);
	case ENOENT:
		throw FileNotFoundException(path, errno);
	case ENOTDIR:
		throw OpenFileException("not a directory", path, errno);
	case EISDIR:
		throw OpenFileException("not a file", path, errno);
	case EROFS:
		throw FileReadOnlyException(path, errno);
	case EEXIST:
		throw FileExistsException(path, errno);
	case ENOSPC:
		throw FileException("no space left on device", path, errno);
	case EDQUOT:
		throw FileException("disk quota exceeded", path, errno);
#if !defined(_AIX)
	case ENOTEMPTY:
		throw FileException("directory not empty", path, errno);
#endif
	case ENAMETOOLONG:
		throw PathSyntaxException(path, errno);
	case ENFILE:
	case EMFILE:
		throw FileException("too many open files", path, errno);
	default:
		throw FileException(std::strerror(errno), path, errno);
	}
    */
}

} // namespace platform
} // namespace nanogear
