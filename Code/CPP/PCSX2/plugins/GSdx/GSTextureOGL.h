/* 
 *	Copyright (C) 2007-2009 Gabest
 *	http://www.gabest.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *   
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *   
 *  You should have received a copy of the GNU General Public License
 *  along with GNU Make; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA. 
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#pragma once

#include "GSTexture.h"

class GSTextureOGL : public GSTexture
{
	GLuint m_texture;
	GLuint m_pbo;

	int m_type;
	int m_format;

public:
	GSTextureOGL(GLuint texture, int type, int width, int height, int format = 0);
	virtual ~GSTextureOGL();

	bool Update(const GSVector4i& r, const void* data, int pitch);
	bool Map(GSMap& m, const GSVector4i* r);
	void Unmap();
	bool Save(const string& fn, bool dds = false);

	operator GLuint() {return m_texture;}
};