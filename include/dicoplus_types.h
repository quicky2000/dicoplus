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
#ifndef _DICOPLUS_TYPES_H_
#define _DICOPLUS_TYPES_H_

#include "dicoplus_configuration.h"
#include "systemc.h"
#include <sstream>

namespace dicoplus
{
  class dicoplus_types
  {
  public:
    typedef sc_bv<dicoplus_configuration::m_global_cmd_nb_bits > t_global_cmd_type;
    typedef sc_bv<dicoplus_configuration::m_characters_nb_bits> t_global_data_type;
    typedef enum 
      {
	UNINITIALIZED=0,
	INITIALIZED,
	READY2START,
	POTENTIAL_FIRST,
	START_POTENTIAL_NOT_FIRST,
	POTENTIAL_NOT_FIRST,
	POTENTIAL_FIRST_NOT_FIRST,
	NOT_FIRST
      } t_cell_FSM_state;
    inline static const std::string cell_FSM_state2string(const t_cell_FSM_state & p_state);
  private:
  };

  //----------------------------------------------------------------------------
  const std::string dicoplus_types::cell_FSM_state2string(const t_cell_FSM_state & p_state)
    {
      switch(p_state)
        {
        case UNINITIALIZED:
          return "UNINITIALIZED";
          break;
        case INITIALIZED:
          return "INITIALIZED";
          break;
        case READY2START:
          return "READY2START";
          break;
	case POTENTIAL_FIRST:
          return "POTENTIAL_FIRST";
	  break;
	case START_POTENTIAL_NOT_FIRST:
          return "START_POTENTIAL_NOT_FIRST";
	  break;
	case POTENTIAL_NOT_FIRST:
          return "POTENTIAL_NOT_FIRST";
	  break;
	case POTENTIAL_FIRST_NOT_FIRST:
          return "POTENTIAL_FIRST_NOT_FIRST";
	  break;
	case NOT_FIRST:
          return "NOT_FIRST";
	  break;
        default:
	  std::stringstream l_stream;
	  l_stream << p_state;
          throw quicky_exception::quicky_logic_exception("No string representation for cell FSM state \""+l_stream.str()+"\"",__LINE__,__FILE__);
          break;
        }
    }
}
#endif // _DICOPLUS_TYPES_H_
//EOF
