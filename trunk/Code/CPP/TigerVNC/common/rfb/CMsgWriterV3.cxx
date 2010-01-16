/* Copyright (C) 2002-2005 RealVNC Ltd.  All Rights Reserved.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */
#include <rdr/OutStream.h>
#include <rfb/msgTypes.h>
#include <rfb/Exception.h>
#include <rfb/ConnParams.h>
#include <rfb/CMsgWriterV3.h>

using namespace rfb;

CMsgWriterV3::CMsgWriterV3(ConnParams* cp, rdr::OutStream* os)
  : CMsgWriter(cp, os)
{
}

CMsgWriterV3::~CMsgWriterV3()
{
}

void CMsgWriterV3::writeClientInit(bool shared)
{
  os->writeU8(shared);
  endMsg();
}

void CMsgWriterV3::startMsg(int type)
{
  os->writeU8(type);
}

void CMsgWriterV3::endMsg()
{
  os->flush();
}

void CMsgWriterV3::writeSetDesktopSize(int width, int height,
                                       const ScreenSet& layout)
{
  if (!cp->supportsSetDesktopSize)
    throw Exception("Server does not support SetDesktopSize");

  startMsg(msgTypeSetDesktopSize);
  os->pad(1);

  os->writeU16(width);
  os->writeU16(height);

  os->writeU8(layout.num_screens());
  os->pad(1);

  ScreenSet::const_iterator iter;
  for (iter = layout.begin();iter != layout.end();++iter) {
    os->writeU32(iter->id);
    os->writeU16(iter->dimensions.tl.x);
    os->writeU16(iter->dimensions.tl.y);
    os->writeU16(iter->dimensions.width());
    os->writeU16(iter->dimensions.height());
    os->writeU32(iter->flags);
  }

  endMsg();
}
