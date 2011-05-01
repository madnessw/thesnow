// Copyright (C) 2003 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#ifndef _LINEAR_DISKCACHE
#define _LINEAR_DISKCACHE

#include "Common.h"
#include <fstream>
#include <functional>

// Update this to the current SVN revision every time you change shader generation code.
// We don't automatically get this from SVN_REV because that would mean regenerating the
// shader cache for every revision, graphics-related or not, which is simply annoying.
enum
{
	LINEAR_DISKCACHE_VER = 6964
};

// On disk format:
//header{
// u32 'DCAC';
// u32 version;  // svn_rev
// u16 sizeof(key_type);
// u16 sizeof(value_type);
//}

//key_value_pair{
// u32 value_size;
// key_type   key;
// value_type[value_size]   value;
//}

// Example reader:
//
//class LinearDiskCacheReader
//{
//public:
//	template <typename F>
//	void operator()(const K& key, u32 value_size, F get_data) const
//	{...}
//};

// Dead simple unsorted key-value store with append functionality.
// No random read functionality, all reading is done in OpenAndRead.
// Keys and values can contain any characters, including \0.
//
// Suitable for caching generated shader bytecode between executions.
// Not tuned for extreme performance but should be reasonably fast.
// Does not support keys or values larger than 2GB, which should be reasonable.
// Keys must have non-zero length; values can have zero length.

// K and V are some POD type
// K : the key type
// V : value array type
template <typename K, typename V>
class LinearDiskCache
{
public:
	// return number of read entries
	template <typename LinearDiskCacheReader>
	u32 OpenAndRead(const char* filename, LinearDiskCacheReader& reader)
	{
		using std::ios;

		// close any currently opened file
		Close();

		// try opening for reading/writing
		m_file.open(filename, ios::in | ios::out | ios::binary | ios::app);
		
		if (m_file.is_open() && ValidateHeader())
		{
			// good header, read some key/value pairs
			u32 num_entries = 0;

			u32 value_size;
			K key;
			while (Read(&value_size) && Read(&key))
			{
				std::streamoff const pos = m_file.tellg();

				// pass key and value_size to reader with callback function to read the data
				reader(key, value_size, [this, &value_size](V* data){ Read(data, value_size); });

				// seek past data (in case reader didn't read it for whatever reason)
				m_file.seekg(pos + (value_size * sizeof(V)), ios::beg);

				++num_entries;
			}
			m_file.clear();

			return num_entries;
		}

		// failed to open file for reading or bad header
		// close and recreate file
		Close();
		m_file.open(filename, ios::out | ios::trunc | ios::binary);
		WriteHeader();
		return 0;
	}
	
	void Sync()
	{
		m_file.flush();
	}

	void Close()
	{
		if (m_file.is_open())
			m_file.close();
		// clear any error flags
		m_file.clear();
	}

	// Appends a key-value pair to the store.
	void Append(const K &key, const V *value, u32 value_size)
	{
		// TODO: Should do a check that we don't already have "key"?
		Write(&value_size);
		Write(&key);
		Write(value, value_size);
	}

private:
	void WriteHeader()
	{
		Write(&m_header);
	}

	bool ValidateHeader()
	{
		char file_header[sizeof(Header)];

		return (Read(file_header, sizeof(Header))
			&& !memcmp((const char*)&m_header, file_header, sizeof(Header)));
	}

	template <typename D>
	bool Write(const D *data, u32 count = 1)
	{
		return m_file.write((const char*)data, count * sizeof(D)).good();
	}

	template <typename D>
	bool Read(const D *data, u32 count = 1)
	{
		return m_file.read((char*)data, count * sizeof(D)).good();
	}

	struct Header
	{
		Header()
			: id(*(u32*)"DCAC")
			, ver(LINEAR_DISKCACHE_VER)
			, key_t_size(sizeof(K))
			, value_t_size(sizeof(V))
		{}

		const u32 id, ver;
		const u16 key_t_size, value_t_size;

	} m_header;

	std::fstream m_file;
};

#endif  // _LINEAR_DISKCACHE
