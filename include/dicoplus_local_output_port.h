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
#ifndef _DICOPLUS_LOCAL_OUTPUT_PORT_H_
#define _DICOPLUS_LOCAL_OUTPUT_PORT_H_

#include "dicoplus_local_bus.h"

namespace dicoplus
{
  class dicoplus_local_output_port
  {
  public:
    inline dicoplus_local_output_port(const std::string & p_name);
    inline void operator () (dicoplus_local_bus  & p_bus);
    inline void send_data(bool p_data);
    inline void no_data(void);
  private:
    sc_out<bool> m_valid;
    sc_out<bool> m_data;
  };

  //----------------------------------------------------------------------------
  dicoplus_local_output_port::dicoplus_local_output_port(const std::string & p_name):
    m_valid((p_name+"_valid").c_str()),
    m_data((p_name+"_data").c_str())
      {
      }

    //----------------------------------------------------------------------------
    void dicoplus_local_output_port::send_data(bool p_data)
    {
      m_data = p_data;
      m_valid = true;;
    }

    //----------------------------------------------------------------------------
    void dicoplus_local_output_port::no_data(void)
    {
      m_valid = false;
    }

    //----------------------------------------------------------------------------
    void dicoplus_local_output_port::operator () (dicoplus_local_bus  & p_bus)
    {
      m_valid(p_bus.m_valid);
      m_data(p_bus.m_data);    
    }
}
#endif // _DICOPLUS_LOCAL_OUTPUT_PORT_H_
//EOF
