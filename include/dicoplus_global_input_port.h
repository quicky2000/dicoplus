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
#ifndef _DICOPLUS_GLOBAL_INPUT_PORT_H_
#define _DICOPLUS_GLOBAL_INPUT_PORT_H_

#include "dicoplus_global_bus.h"
#include "dicoplus_types.h"

namespace dicoplus
{
  class dicoplus_global_input_port
  {
  public:
    inline dicoplus_global_input_port(const std::string & p_name);
    inline void operator () (dicoplus_global_bus  & p_bus);
  private:
    sc_in<bool> m_req;
    sc_out<bool> m_acq;
    sc_in<typename dicoplus_types::t_global_cmd_type> m_cmd;
    sc_in<typename dicoplus_types::t_global_data_type> m_data;
  };

  //----------------------------------------------------------------------------
  void dicoplus_global_input_port::operator () (dicoplus_global_bus  & p_bus)
  {
	m_req(p_bus.m_req);
	m_acq(p_bus.m_acq);
	m_cmd(p_bus.m_cmd);
	m_data(p_bus.m_data);    
  }

  //----------------------------------------------------------------------------
  dicoplus_global_input_port::dicoplus_global_input_port(const std::string & p_name):
    m_req((p_name+"_req").c_str()),
    m_acq((p_name+"_acq").c_str()),
    m_cmd((p_name+"_cmd").c_str()),
    m_data((p_name+"_data").c_str())
    {
    }

}
#endif // _DICOPLUS_GLOBAL_INPUT_PORT_H_
//EOF
