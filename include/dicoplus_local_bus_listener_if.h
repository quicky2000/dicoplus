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
#ifndef _DICOPLUS_LOCAL_BUS_LISTENER_IF_H_
#define _DICOPLUS_LOCAL_BUS_LISTENER_IF_H_

namespace dicoplus
{
  class dicoplus_local_bus_listener_if
  {
  public:
    virtual void no_activity(void)=0;
    virtual void data(bool p_data)=0;
    inline virtual ~dicoplus_local_bus_listener_if(void){}
  private:
  };
}

#endif // _DICOPLUS_LOCAL_BUS_LISTENER_H_
//EOF
