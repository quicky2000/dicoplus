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
#ifndef _DICOPLUS_LOCAL_INPUT_PORT_H_
#define _DICOPLUS_LOCAL_INPUT_PORT_H_

#include "dicoplus_local_bus.h"

namespace dicoplus
{
  class dicoplus_local_input_port
  {
  public:
    inline dicoplus_local_input_port(const std::string & p_name);
    inline void operator () (dicoplus_local_bus  & p_bus);
    inline bool is_valid(void)const;
    inline sc_bv<2> get_data(void)const;
  private:
    sc_in<bool> m_valid;
    sc_in<sc_bv<2> > m_data;
  };

  //----------------------------------------------------------------------------
  dicoplus_local_input_port::dicoplus_local_input_port(const std::string & p_name):
    m_valid((p_name+"_valid").c_str()),
    m_data((p_name+"_data").c_str())
      {
      }

    //----------------------------------------------------------------------------
    bool dicoplus_local_input_port::is_valid(void)const
      {
	return m_valid.read();
      }

    //----------------------------------------------------------------------------
    sc_bv<2> dicoplus_local_input_port::get_data(void)const
      {
	return m_data.read();
      }

    //----------------------------------------------------------------------------
    void dicoplus_local_input_port::operator () (dicoplus_local_bus  & p_bus)
    {
      m_valid(p_bus.m_valid);
      m_data(p_bus.m_data);    
    }

}
#endif // _DICOPLUS_LOCAL_INPUT_PORT_H_
//EOF
