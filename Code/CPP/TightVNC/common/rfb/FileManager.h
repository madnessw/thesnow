/* Copyright (C) 2005 TightVNC Team.  All Rights Reserved.
 *
 * Developed by Dennis Syrovatsky.
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
 *
 * TightVNC distribution homepage on the Web: http://www.tightvnc.com/
 *
 */

// -=- FileManager.

#ifndef __RFB_FILEMANAGER_H__
#define __RFB_FILEMANAGER_H__

#include <rfb/fttypes.h>

namespace rfb {
  class FileManager {
  public:
    FileManager();
    ~FileManager();
    
    bool create(char *pFilename);
    bool close();
    
    bool isCreated();

    char *getFilename();

  protected:
    FILE *m_pFile;
    char m_szMode[4];
    char m_szFilename[FT_FILENAME_SIZE];
  };
}
#endif // __RFB_FILEMANAGER_H__
