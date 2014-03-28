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
#ifndef _DICOPLUS_LOCAL_BUS_H_
#define _DICOPLUS_LOCAL_BUS_H_

#include "systemc.h"
#include <string>

namespace dicoplus
{
  class dicoplus_local_bus
  {
  public:
    inline dicoplus_local_bus(const std::string & p_name);
    sc_signal<bool> m_valid;
    sc_signal<bool> m_data;
  private:
  };

  //----------------------------------------------------------------------------
  dicoplus_local_bus::dicoplus_local_bus(const std::string & p_name):
    m_valid((p_name+"_valid").c_str()),
    m_data((p_name+"_data").c_str())
    {
    }
}
#endif // _DICOPLUS_LOCAL_BUS_H_
