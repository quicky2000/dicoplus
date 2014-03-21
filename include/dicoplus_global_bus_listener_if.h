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
#ifndef _DICOPLUS_GLOBAL_BUS_LISTENER_IF_H
#define _DICOPLUS_GLOBAL_BUS_LISTENER_IF_H

#include "dicoplus_global_message_analyzer_if.h"

namespace dicoplus
{
  class dicoplus_global_bus_listener_if: public dicoplus_global_message_analyzer_if
  {
  public:
    virtual void no_activity(void)=0;
  private:
  };
}

#endif // _DICOPLUS_GLOBAL_BUS_LISTENER_IF_H
//EOF
