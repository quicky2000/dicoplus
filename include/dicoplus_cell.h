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
#include "dicoplus_local_input_port.h"
#include "dicoplus_local_output_port.h"

#define DEBUG_DICOPLUS_CELL

namespace dicoplus
{
  class dicoplus_cell:public sc_module, public dicoplus_global_port_binding_if,public dicoplus_global_message_analyzer_if
  {
  public:
    SC_HAS_PROCESS(dicoplus_cell);
    inline dicoplus_cell(sc_module_name name, 
                         const unsigned int & p_nb_neighbour);

    // Methods inherited from dicoplus_global_port_binding_if
    inline void bind_input_port(dicoplus_global_bus & p_bus);
    inline void bind_output_port(dicoplus_global_bus & p_bus);
    // End of methods inherited from dicoplus_global_port_binding_if

    // Methods inherited from dicoplus_global_message_analyzer_if
    inline void treat(const dicoplus_global_message_char & p_message);
    inline void treat(const dicoplus_global_message_separator & p_message);
    // End of methods inherited from dicoplus_global_message_analyzer_if

    inline void set_listener(cell_listener_if & p_listener);
    inline void bind_north_port(dicoplus_local_bus & p_bus);
    inline void bind_east_port(dicoplus_local_bus & p_bus);
    inline void bind_south_port(dicoplus_local_bus & p_bus);
    inline void bind_west_port(dicoplus_local_bus & p_bus);
    inline void bind_output_port(dicoplus_local_bus & p_bus);

    sc_in<bool> m_clk;
  private:
    inline void run(void);
    inline void set_internal_state(const dicoplus_types::t_cell_FSM_state & p_state);


    dicoplus_global_port_manager m_global_port_manager;
    cell_listener_if * m_listener;
    dicoplus_types::t_global_data_type m_content;
    dicoplus_types::t_cell_FSM_state m_internal_state;
    dicoplus_local_input_port m_local_north_input_port;
    dicoplus_local_input_port m_local_east_input_port;
    dicoplus_local_input_port m_local_south_input_port;
    dicoplus_local_input_port m_local_west_input_port;
    dicoplus_local_output_port m_local_output_port;
    unsigned int m_nb_valid;
    unsigned int m_nb_invalid;
    const unsigned int m_nb_neighbour;
    unsigned int m_remaining_locals;
    // Variable to set to true by the debugger to activate some messages
    bool m_gdb_debug;
  };

  //----------------------------------------------------------------------------
  dicoplus_cell::dicoplus_cell(sc_module_name name, 
                               const unsigned int & p_nb_neighbour):
    sc_module(name),
    m_clk("clk"),
    m_global_port_manager("global_port_manager",*this),
    m_listener(NULL),
    m_content(0),
    m_internal_state(dicoplus_types::UNINITIALIZED),
    m_local_north_input_port("north"),
    m_local_east_input_port("east"),
    m_local_south_input_port("south"),
    m_local_west_input_port("west"),
    m_local_output_port("output"),
    m_nb_valid(0),
    m_nb_invalid(0),
    m_nb_neighbour(p_nb_neighbour),
    m_remaining_locals(0),
    m_gdb_debug(false)
      {
	m_global_port_manager.m_clk(m_clk);
#ifdef DEBUG_DICOPLUS_CELL
        std::cout << "Constructor of \"" << this->name() <<  "\"" << std::endl ;
#endif //  DEBUG_DICOPLUS_CELL

        SC_THREAD(run);
        sensitive << m_clk.pos();
      }

    //----------------------------------------------------------------------------
    void dicoplus_cell::set_internal_state(const dicoplus_types::t_cell_FSM_state & p_state)
    {
      if(dicoplus_types::POTENTIAL == p_state || dicoplus_types::POTENTIAL_NORTH == p_state || dicoplus_types::POTENTIAL_WEST == p_state || dicoplus_types::FIRST == p_state || dicoplus_types::ATTACHED == p_state)
        {
#ifdef DEBUG_DICOPLUS_CELL
          if(m_gdb_debug)
            {
              std::cout << "Set remaining locals of \"" << name() << "\"" << std::endl ;
            }
#endif // DEBUG_DICOPLUS_CELL
          m_remaining_locals = m_nb_neighbour;
        }
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
      if(m_gdb_debug)
        {
          char l_char_array[5] = {0x0,0x0,0x0,0x0,0x0};
          uint32_t l_cp = dicoplus_char::get_code_point(p_message.get_data().to_uint());
          utf8::append(l_cp,l_char_array);
          std::cout << name() << " : Treat char message '" << l_char_array << "'@ " << sc_time_stamp() << std::endl ;
          std::cout << "\tInternal state : " << dicoplus_types::cell_FSM_state2string(m_internal_state) << std::endl ;
        }
#endif // DEBUG_DICOPLUS_CELL
      if(dicoplus_types::UNINITIALIZED != m_internal_state && dicoplus_types::INITIALIZED != m_internal_state  && dicoplus_types::SKIP_NEXT_WEST != m_internal_state  && dicoplus_types::SKIP_NEXT_NORTH != m_internal_state  && p_message.get_data() != m_content)
        {
          m_local_output_port.send_data(false);
        }
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
	  break;
        case dicoplus_types::POTENTIAL:
        case dicoplus_types::POTENTIAL_WEST:
        case dicoplus_types::POTENTIAL_NORTH:
          if(p_message.get_data() == m_content)
            {
              set_internal_state(dicoplus_types::ATTACHED);
              m_local_output_port.send_data(true);
            }
          else
            {
              set_internal_state( dicoplus_types::NOT_FIRST);
            }
          m_global_port_manager.post_message(p_message);
          break;
	case dicoplus_types::READY2START:
          if(p_message.get_data() == m_content)
            {
              set_internal_state(dicoplus_types::FIRST);
              m_local_output_port.send_data(true);
            }
          else
            {
              set_internal_state(dicoplus_types::NOT_FIRST);
            }
	  m_global_port_manager.post_message(p_message);
	  break;
        case dicoplus_types::SKIP_NEXT_WEST:
          set_internal_state(dicoplus_types::POTENTIAL_WEST);
          m_global_port_manager.post_message(p_message);
          break;
        case dicoplus_types::SKIP_NEXT_NORTH:
          set_internal_state(dicoplus_types::POTENTIAL_NORTH);
          m_global_port_manager.post_message(p_message);
          break;
	case dicoplus_types::NOT_FIRST:
	case dicoplus_types::FIRST:
        case dicoplus_types::ATTACHED:
        case dicoplus_types::CONFIRMED:
	  m_global_port_manager.post_message(p_message);
	  break;
	default:
	  throw quicky_exception::quicky_logic_exception("Unhandled state to receive a char message : \""+dicoplus_types::cell_FSM_state2string(m_internal_state),__LINE__,__FILE__);
	  break;
	}
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::treat(const dicoplus_global_message_separator & p_message)
    {
      
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
        {
          std::cout << name() << " : Treat separator message @ " << sc_time_stamp() << std::endl ;
          std::cout << "\tInternal state : " << dicoplus_types::cell_FSM_state2string(m_internal_state) << std::endl ;
        }
#endif // DEBUG_DICOPLUS_CELL
      switch(m_internal_state)
	{
	case dicoplus_types::NOT_FIRST:
	case dicoplus_types::INITIALIZED:
	case dicoplus_types::POTENTIAL:
	case dicoplus_types::POTENTIAL_WEST:
	case dicoplus_types::POTENTIAL_NORTH:
	  set_internal_state(dicoplus_types::READY2START);
	  m_global_port_manager.post_message(p_message);
	  break;
	case dicoplus_types::ATTACHED:
	  set_internal_state(dicoplus_types::CONFIRMED);
	  m_global_port_manager.post_message(p_message);
	  break;
	case dicoplus_types::CONFIRMED:
	  m_global_port_manager.post_message(p_message);
	  break;
	default:
	  throw quicky_exception::quicky_logic_exception("Unhandled state to receive a separator message : \""+dicoplus_types::cell_FSM_state2string(m_internal_state)+"\" in cell \""+name()+"\"",__LINE__,__FILE__);
	  break;
	}
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::run(void)
    {
      while(1)
	{
          m_nb_valid = 0;
          m_nb_invalid = 0;
          unsigned int l_nb_local_msg = 0;
          if(m_local_north_input_port.is_valid())
            {
              ++l_nb_local_msg;
#ifdef DEBUG_DICOPLUS_CELL
              if(m_gdb_debug)
                {
                  std::cout << name() << " received " << m_local_north_input_port.get_data() << " from north" << std::endl ;
                  std::cout << "\tInternal state : " << dicoplus_types::cell_FSM_state2string(m_internal_state) << std::endl ;
                }
#endif // DEBUG_DICOPLUS_CELL
              if(m_local_north_input_port.get_data())
                {
                  if(dicoplus_types::READY2START == m_internal_state || dicoplus_types::NOT_FIRST == m_internal_state || dicoplus_types::POTENTIAL == m_internal_state)set_internal_state(dicoplus_types::SKIP_NEXT_NORTH); 
                  ++m_nb_valid;
                }
              else if(dicoplus_types::POTENTIAL_NORTH != m_internal_state)
                {
                  ++m_nb_invalid;
                }
            }
          if(m_local_east_input_port.is_valid())
            {
              ++l_nb_local_msg;
#ifdef DEBUG_DICOPLUS_CELL
              if(m_gdb_debug)
                {
                  std::cout << name() << " received " << m_local_east_input_port.get_data() << " from east" << std::endl ;
                  std::cout << "\tInternal state : " << dicoplus_types::cell_FSM_state2string(m_internal_state) << std::endl ;
                }
#endif // DEBUG_DICOPLUS_CELL
              if(m_local_east_input_port.get_data())
                {
                  ++m_nb_valid;
                }
              else
                {
                  ++m_nb_invalid;
                }
            }
          if(m_local_south_input_port.is_valid())
            {
              ++l_nb_local_msg;
#ifdef DEBUG_DICOPLUS_CELL
              if(m_gdb_debug)
                {
                  std::cout << name() << " received " << m_local_south_input_port.get_data() << " from south" << std::endl ;
                  std::cout << "\tInternal state : " << dicoplus_types::cell_FSM_state2string(m_internal_state) << std::endl ;
                }
#endif // DEBUG_DICOPLUS_CELL
              if(m_local_south_input_port.get_data())
                {
                  ++m_nb_valid;
                }
              else
                {
                  ++m_nb_invalid;
                }
            }
          if(m_local_west_input_port.is_valid())
            {
              ++l_nb_local_msg;
#ifdef DEBUG_DICOPLUS_CELL
              if(m_gdb_debug)
                {
                  std::cout << name() << " received " << m_local_west_input_port.get_data() << " from west" << std::endl ;
                  std::cout << "\tInternal state : " << dicoplus_types::cell_FSM_state2string(m_internal_state) << std::endl ;
                }
#endif // DEBUG_DICOPLUS_CELL
              if(m_local_west_input_port.get_data())
                {
                  if(dicoplus_types::READY2START == m_internal_state || dicoplus_types::NOT_FIRST == m_internal_state || dicoplus_types::POTENTIAL == m_internal_state)set_internal_state(dicoplus_types::SKIP_NEXT_WEST); 
                  ++m_nb_valid;
                }
              else if(dicoplus_types::POTENTIAL_WEST != m_internal_state)
                {
                  ++m_nb_invalid;
                }
            }

          if(l_nb_local_msg)
            {
              switch(m_internal_state)
                {
                case dicoplus_types::INITIALIZED:
                case dicoplus_types::UNINITIALIZED:
                case dicoplus_types::SKIP_NEXT_WEST :
                case dicoplus_types::SKIP_NEXT_NORTH :
                  break;
                case dicoplus_types::POTENTIAL:
                case dicoplus_types::POTENTIAL_WEST:
                case dicoplus_types::POTENTIAL_NORTH:
                  if(m_nb_invalid)
                    {
#ifdef DEBUG_DICOPLUS_CELL
                      if(m_gdb_debug)
                        {
                          std::cout << "Compute remaining locals of " << name() << std::endl ;
                        }
#endif // DEBUG_DICOPLUS_CELL
                      m_remaining_locals = m_remaining_locals - m_nb_invalid;
                      assert(m_remaining_locals < m_nb_neighbour);
                      if(!m_remaining_locals)
                        {
                          set_internal_state(dicoplus_types::NOT_FIRST);
                        }
                    }
                  break;
                case dicoplus_types::ATTACHED:
                case dicoplus_types::FIRST:
                  if(m_nb_valid)
                    {
#ifdef DEBUG_DICOPLUS_CELL
                      if(m_gdb_debug)
                        {
                          std::cout << "Confirm " << name() << std::endl ;
                        }
#endif // DEBUG_DICOPLUS_CELL
                      set_internal_state(dicoplus_types::CONFIRMED);
                    }
                  else
                    {
#ifdef DEBUG_DICOPLUS_CELL
                      if(m_gdb_debug)
                        {
                          std::cout << "Compute remaining locals of " << name() << std::endl ;
                        }
#endif // DEBUG_DICOPLUS_CELL
                      m_remaining_locals = m_remaining_locals - m_nb_invalid;
                      assert(m_remaining_locals < m_nb_neighbour);
                      if(!m_remaining_locals)
                        {
                          set_internal_state(dicoplus_types::NOT_FIRST);
                        }
                    }
                  break;
                case dicoplus_types::NOT_FIRST :
                  if(m_nb_valid) 
                    {
                      set_internal_state(dicoplus_types::POTENTIAL);
                    }
                  break;
                case dicoplus_types::READY2START:
                  if(m_nb_valid) 
                    {
                      set_internal_state(dicoplus_types::POTENTIAL);
                    }
                  break;
                case dicoplus_types::CONFIRMED:
                  break;
                default:
                  throw quicky_exception::quicky_logic_exception("Unhandled state to receive a local message : \""+dicoplus_types::cell_FSM_state2string(m_internal_state)+"\" in cell \""+name()+"\"",__LINE__,__FILE__);
                  break;
                }
            }
          m_local_output_port.no_data();
	  m_global_port_manager.listen();
	  wait();
	}
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_north_port(dicoplus_local_bus & p_bus)
    {
      m_local_north_input_port(p_bus);      
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_east_port(dicoplus_local_bus & p_bus)
    {
      m_local_east_input_port(p_bus);      
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_south_port(dicoplus_local_bus & p_bus)
    {
      m_local_south_input_port(p_bus);      
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_west_port(dicoplus_local_bus & p_bus)
    {
      m_local_west_input_port(p_bus);      
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_output_port(dicoplus_local_bus & p_bus)
    {
      m_local_output_port(p_bus);      
    }

  
}
#endif // _DICOPLUS_CELL_H_
//EOF
