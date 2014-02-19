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
#ifndef _CHAR_ZONE_H_
#define _CHAR_ZONE_H_

#include "custom_field_zone.h"
namespace dicoplus
{
  class char_zone: synoptic::public custom_field_zone
  {
  public:
  private:
    //Inherited methods from custom_field_zone
    inline const uint64_t & get_content_representation(const uint32_t & p_content);
    //End of inherited methods from custom_field_zone
  };
}

#endif // _CHAR_ZONE_H_
//EOF
