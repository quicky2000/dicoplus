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
#ifndef _DICOPLUS_GLOBAL_PORT_MANAGER_H_
#define _DICOPLUS_GLOBAL_PORT_MANAGER_H_

#include "dicoplus_global_input_port.h"
#include "dicoplus_global_output_port.h"

namespace dicoplus
{
  class dicoplus_global_port_manager:sc_module
  {
  public:
    inline dicoplus_global_port_manager(sc_module_name p_name);
    inline void bind_input_port(dicoplus_global_bus & p_bus);
    inline void bind_output_port(dicoplus_global_bus & p_bus);
  private:
    dicoplus_global_input_port m_global_input_port;
    dicoplus_global_output_port m_global_output_port;
  };

  //----------------------------------------------------------------------------
  dicoplus_global_port_manager::dicoplus_global_port_manager(sc_module_name p_name):
    sc_module(p_name),
    m_global_input_port("gi_port"),
    m_global_output_port("go_port")
   {
   }
    //----------------------------------------------------------------------------
    void dicoplus_global_port_manager::bind_input_port(dicoplus_global_bus & p_bus)
    {
      m_global_input_port(p_bus);
    }

    //----------------------------------------------------------------------------
    void dicoplus_global_port_manager::bind_output_port(dicoplus_global_bus & p_bus)
    {
      m_global_output_port(p_bus);
    }
}
#endif // _DICOPLUS_GLOBAL_PORT_MANAGER_H_
//EOF
