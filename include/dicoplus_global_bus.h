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
#ifndef _DICOPLUS_GLOBAL_BUS_H_
#define _DICOPLUS_GLOBAL_BUS_H_

#include "systemc.h"
#include "dicoplus_types.h"
#include <string>

namespace dicoplus
{
  class dicoplus_global_bus
  {
  public:
    inline dicoplus_global_bus(const std::string & p_name);
    sc_signal<bool> m_req;
    sc_signal<bool> m_acq;
    sc_signal<typename dicoplus_types::t_global_cmd_type> m_cmd;
    sc_signal<typename dicoplus_types::t_global_data_type> m_data;
  private:
  };

  //----------------------------------------------------------------------------
  dicoplus_global_bus::dicoplus_global_bus(const std::string & p_name):
    m_req((p_name+"_req").c_str()),
    m_acq((p_name+"_acq").c_str()),
    m_cmd((p_name+"_cmd").c_str()),
    m_data((p_name+"_data").c_str())
    {
    }
}
#endif // _DICOPLUS_GLOBAL_BUS_H_
