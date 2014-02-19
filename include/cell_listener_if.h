/*    This file is part of dicoplus
      The aim of this software is to solve dicoplus game
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

#ifndef CELL_LISTENER_IF_H
#define CELL_LISTENER_IF_H

#include <inttypes.h>

namespace dicoplus
{
  class cell_listener_if
  {
  public:
    virtual void set_content(const uint32_t & p_content)=0;
    inline virtual ~cell_listener_if(void){}
  private:
  };
}
#endif // CELL_LISTENER_IF_H
//EOF
