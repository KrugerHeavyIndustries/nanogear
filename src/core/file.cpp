/*
 * Nanogear - C++ web development framework
 *
 * This library is based on Restlet (R) <http://www.restlet.org> by Noelios Technologies
 * Copyright (C) 2005-2008 by Noelios Technologies <http://www.noelios.com>
 * Restlet is a registered trademark of Noelios Technologies. All other marks and
 * trademarks are property of their respective owners.
 *
 * Copyright (C) 2008-2009 Chris Kruger.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "file.h"
#include "path.h"
#include "directoryiterator.h"

#if defined(POCO_OS_FAMILY_WINDOWS) && defined(POCO_WIN32_UTF8)
#if defined(_WIN32_WCE)
#include "File_WINCE.cpp"
#else
#include "File_WIN32U.cpp"
#endif
#elif defined(POCO_OS_FAMILY_WINDOWS)
#include "File_WIN32.cpp"
#elif defined(POCO_VXWORKS)
#include "File_VX.cpp"
#elif defined(__APPLE__)
#include "fileunix.cpp"
#else
#include "File_VMS.cpp"
#endif

namespace nanogear
{

File::File()
{
}

File::File(const std::string& path) : platform::File(path)
{
}

File::File(const char* path): platform::File(std::string(path))
{
}

File::File(const Path& path): platform::File(path.toString())
{
}

File::File(const File& file): platform::File(file.getPath())
{
}

File::~File()
{
}

File& File::operator=(const File& file)
{
	setPath(file.getPath());
	return *this;
}

File& File::operator=(const std::string& path)
{
	setPath(path);
	return *this;
}

File& File::operator=(const char* path)
{
	//poco_check_ptr (path);
	setPath(path);
	return *this;
}

File& File::operator=(const Path& path)
{
	setPath(path.toString());
	return *this;
}

void File::swap(File& file)
{
	swap(file);
}

bool File::exists() const
{
   return platform::File::exists();
}

bool File::canRead() const
{
	return platform::File::canRead();
}

bool File::canWrite() const
{
	return platform::File::canWrite();
}

bool File::canExecute() const
{
	return platform::File::canExecute();
}

bool File::isFile() const
{
	return platform::File::isFile();
}

bool File::isDirectory() const
{
	return platform::File::isDirectory();
}

bool File::isLink() const
{
	return platform::File::isLink();
}

bool File::isDevice() const
{
	return platform::File::isDevice();
}

bool File::isHidden() const
{
	return platform::File::isHidden();
}

Timestamp File::created() const
{
	return platform::File::created();
}

Timestamp File::getLastModified() const
{
	return platform::File::getLastModified();
}
	
File& File::setLastModified(const Timestamp& ts)
{
	platform::File::setLastModified(ts);
	return *this;
}

File::FileSize File::getSize() const
{
	return platform::File::getSize();
}

File& File::setSize(FileSize size)
{
	platform::File::setSize(size);
	return *this;
}

File& File::setWriteable(bool flag)
{
	platform::File::setWriteable(flag);
	return *this;
}

File& File::setReadOnly(bool flag)
{
	platform::File::setWriteable(!flag);
	return *this;
}

File& File::setExecutable(bool flag)
{
	platform::File::setExecutable(flag);
	return *this;
}

void File::copyTo(const std::string& path) const
{
	Path src(getPath());
	Path dest(path);
	File destFile(path);
	if ((destFile.exists() && destFile.isDirectory()) || dest.isDirectory())
	{
		dest.makeDirectory();
		dest.setFileName(src.getFileName());
	}
	if (isDirectory())
		copyDirectory(dest.toString());
	else
      platform::File::copyTo(dest.toString());
}

void File::copyDirectory(const std::string& path) const
{
	File target(path);
	target.createDirectories();

	Path src(platform::File::getPath());
	src.makeFile();
	DirectoryIterator it(src);
	DirectoryIterator end;
	for (; it != end; ++it)
	{
		it->copyTo(path);
	}
}

void File::moveTo(const std::string& path)
{
	copyTo(path);
	remove(true);
	platform::File::setPath(path);
}

void File::renameTo(const std::string& path)
{
	platform::File::renameTo(path);
	platform::File::setPath(path);
}

void File::remove(bool recursive)
{
	if (recursive && !isLink() && isDirectory())
	{
		std::vector<File> files;
		list(files);
		for (std::vector<File>::iterator it = files.begin(); it != files.end(); ++it)
		{
			it->remove(true);
		}
	}
	platform::File::remove();
}

bool File::createFile()
{
   return platform::File::createFile();
}

bool File::createDirectory()
{
	return platform::File::createDirectory();
}

void File::createDirectories()
{
	if (!exists())
	{
		Path p(platform::File::getPath());
		p.makeDirectory();
		if (p.depth() > 1)
		{
			p.makeParent();
			File f(p);
			f.createDirectories();
		}
		platform::File::createDirectory();
	}
}

void File::list(std::vector<std::string>& files) const
{
	files.clear();
	DirectoryIterator it(*this);
	DirectoryIterator end;
	while (it != end)
	{
		files.push_back(it.name());
		++it;
	}
}

void File::list(std::vector<File>& files) const
{
	files.clear();
	DirectoryIterator it(*this);
	DirectoryIterator end;
	while (it != end)
	{
		files.push_back(*it);
		++it;
	}
}

void File::handleLastError(const std::string& path)
{
	platform::File::handleLastError(path);
}

} // namespace nanogear
