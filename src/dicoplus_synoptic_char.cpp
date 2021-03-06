/*    This file is part of dicoplus
      The aim of this software is to solvde dicoplus game
      Copyright (C) 2014  Julien Thevenon ( julien_thevenon at yahoo.fr )

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 3 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include "dicoplus_synoptic_char.h"

namespace dicoplus
{
  std::map<uint32_t,uint64_t> dicoplus_synoptic_char::m_content_representation;
  const uint64_t dicoplus_synoptic_char::m_default_representation = 0x0;
  const uint32_t dicoplus_synoptic_char::m_width = 5;
  const uint32_t dicoplus_synoptic_char::m_height = 10;
}

//EOF
