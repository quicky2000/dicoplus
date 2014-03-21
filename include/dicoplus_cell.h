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
#ifndef _DICOPLUS_CELL_H_
#define _DICOPLUS_CELL_H_

#include "systemc.h"
#include "dicoplus_global_port_manager.h"
#include "dicoplus_global_port_binding_if.h"
#include "cell_listener_if.h"
#include "dicoplus_global_message_analyzer_if.h"
namespace dicoplus
{
  class dicoplus_cell:public sc_module, public dicoplus_global_port_binding_if,public dicoplus_global_message_analyzer_if
  {
  public:
    SC_HAS_PROCESS(dicoplus_cell);
    inline dicoplus_cell(sc_module_name name);

    // Methods inherited from dicoplus_global_port_binding_if
    inline void bind_input_port(dicoplus_global_bus & p_bus);
    inline void bind_output_port(dicoplus_global_bus & p_bus);
    // End of methods inherited from dicoplus_global_port_binding_if

    // Methods inherited from dicoplus_global_message_analyzer_if
    inline void treat(const dicoplus_global_message_char & p_message);
    inline void treat(const dicoplus_global_message_separator & p_message);
    // End of methods inherited from dicoplus_global_message_analyzer_if

    inline void set_listener(cell_listener_if & p_listener);

    sc_in<bool> m_clk;
  private:
    inline void set_internal_state(const dicoplus_types::t_cell_FSM_state & p_state);


    dicoplus_global_port_manager m_global_port_manager;
    cell_listener_if * m_listener;
    dicoplus_types::t_global_data_type m_content;
    dicoplus_types::t_cell_FSM_state m_internal_state;
  };

  //----------------------------------------------------------------------------
  dicoplus_cell::dicoplus_cell(sc_module_name name):
    sc_module(name),
    m_clk("clk"),
    m_global_port_manager("global_port_manager",*this),
    m_listener(NULL),
    m_content(0),
    m_internal_state(dicoplus_types::UNINITIALIZED)
      {
	m_global_port_manager.m_clk(m_clk);
      }

  //----------------------------------------------------------------------------
    void dicoplus_cell::set_internal_state(const dicoplus_types::t_cell_FSM_state & p_state)
    {
      m_internal_state = p_state;
      if(m_listener) m_listener->set_state(m_internal_state);
   }
  //----------------------------------------------------------------------------
    void dicoplus_cell::set_listener(cell_listener_if & p_listener)
    {
      m_listener = &p_listener;
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_input_port(dicoplus_global_bus & p_bus)
    {
      m_global_port_manager.bind_input_port(p_bus);
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_output_port(dicoplus_global_bus & p_bus)
    {
      m_global_port_manager.bind_output_port(p_bus);
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::treat(const dicoplus_global_message_char & p_message)
    {
#ifdef DEBUG_DICOPLUS_CELL
      std::cout << name() << " : Treat char message @ " << sc_time_stamp() << std::endl ;
#endif // DEBUG_DICOPLUS_CELL
      switch(m_internal_state)
	{
	case dicoplus_types::UNINITIALIZED:
	  {
	    m_content = p_message.get_data();
	    set_internal_state(dicoplus_types::INITIALIZED);
	    if(m_listener) m_listener->set_content(p_message.get_data().to_uint());
	  }
	  break;
	case dicoplus_types::INITIALIZED:
	  {
	    m_global_port_manager.post_message(p_message);
	  }
	default:
	  break;
	}
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::treat(const dicoplus_global_message_separator & p_message)
    {
#ifdef DEBUG_DICOPLUS_CELL
      std::cout << name() << " : Treat separator message @ " << sc_time_stamp() << std::endl ;
#endif // DEBUG_DICOPLUS_CELL
      m_global_port_manager.post_message(p_message);
    }

  
}
#endif // _DICOPLUS_CELL_H_
//EOF
