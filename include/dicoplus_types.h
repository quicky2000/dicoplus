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
#include "quicky_exception.h"
#include "systemc.h"
#include <sstream>

namespace dicoplus
{
  class dicoplus_types
  {
  public:
    typedef sc_bv<dicoplus_configuration::m_global_cmd_nb_bits> t_global_cmd_type;
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
	NOT_FIRST,
	CONFIRMED_FIRST,
	FIRST,
	CONFIRMED_MIDDLE,
	CONFIRMED_LAST,
        MATCHING_NOT_FIRST,
        MATCHED_NOT_FIRST,
        ATTACHED_NOT_FIRST
      } t_cell_FSM_state;

    inline static const std::string cell_FSM_state2string(const t_cell_FSM_state & p_state);

    typedef enum
      {
	LOCAL_MESSAGE_NOT_VALID,
	LOCAL_MESSAGE_VALID,
	LOCAL_MESSAGE_CANCEL
      } t_local_message_content;
    
    inline static const std::string local_message_content2string(const t_local_message_content & p_message);

    typedef enum
      {
        NORTH=0,
        EAST,
        SOUTH,
        WEST
      } t_orientation;

    inline static const std::string orientation2string(const t_orientation & p_state);

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
	case CONFIRMED_LAST:
          return "CONFIRMED_LAST";
	  break;
	case CONFIRMED_MIDDLE:
          return "CONFIRMED_MIDDLE";
	  break;
	case FIRST:
          return "FIRST";
	  break;
	case CONFIRMED_FIRST:
          return "CONFIRMED_FIRST";
	  break;
	case MATCHING_NOT_FIRST:
          return "MATCHING_NOT_FIRST";
	  break;
	case MATCHED_NOT_FIRST:
          return "MATCHING_NOT_FIRST";
	  break;
	case ATTACHED_NOT_FIRST:
          return "ATTACHED_NOT_FIRST";
	  break;
        default:
	  std::stringstream l_stream;
	  l_stream << p_state;
          throw quicky_exception::quicky_logic_exception("No string representation for cell FSM state \""+l_stream.str()+"\"",__LINE__,__FILE__);
          break;
        }
    }

  //----------------------------------------------------------------------------
  const std::string dicoplus_types::local_message_content2string(const t_local_message_content & p_message)
    {
      switch(p_message)
	{
	case LOCAL_MESSAGE_NOT_VALID:
	  return "LOCAL_MESSAGE_NOT_VALID";
	  break;
	case LOCAL_MESSAGE_VALID:
	  return "LOCAL_MESSAGE_VALID";
	  break;
	case LOCAL_MESSAGE_CANCEL:
	  return "LOCAL_MESSAGE_CANCEL";
	  break;
	default:
	  std::stringstream l_stream;
	  l_stream << p_message;
          throw quicky_exception::quicky_logic_exception("No string representation for local message content \""+l_stream.str()+"\"",__LINE__,__FILE__);
	  break;
	}
    }

  //----------------------------------------------------------------------------
  const std::string dicoplus_types::orientation2string(const t_orientation & p_orientation)
    {
      switch(p_orientation)
	{
	case NORTH:
	  return "NORTH";
	  break;
	case EAST:
	  return "EAST";
	  break;
	case SOUTH:
	  return "SOUTH";
	  break;
	case WEST:
	  return "WEST";
	  break;
	default:
	  std::stringstream l_stream;
	  l_stream << p_orientation;
	  throw quicky_exception::quicky_logic_exception("No string representation for orientation \""+l_stream.str()+"\"",__LINE__,__FILE__);
	  break;
	}
    }

}

#endif // _DICOPLUS_TYPES_H_
//EOF
