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
#include "dicoplus_char.h"
#include "dicoplus_link_state.h"
#include <map>

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

    inline ~dicoplus_cell(void);

    inline static void init(void);
    inline static void terminate(void);
  private:
    inline void run(void);

    // Enumerated type to represent local message content
    typedef enum
      {
        NO_MESSAGE,
        NOT_VALID,
        VALID,
	CANCEL
      } t_local_message;

    // Possible states for FSM of letters who can be the first letter of a word
    typedef enum
      {
        UNKNOWN_FIRST,
        POTENTIAL_FIRST,
        FIRST
      } t_first_FSM_state;

    // Possible states for FSM of letters who cannot be the first letter of a 
    // word
    typedef enum
      {
        UNKNOWN_NOT_FIRST,
        POTENTIAL_NOT_FIRST,
        POTENTIAL_NOT_FIRST_RELOADED,
        MATCHING_NOT_FIRST,
        MATCHED_NOT_FIRST,
        ATTACHED_NOT_FIRST,
        NOT_FIRST
      } t_not_first_FSM_state;

    // Possible states of common internal FSM
    typedef enum
      {
	COMMON_UNINITIALIZED=0,
	COMMON_INITIALIZED,
	COMMON_READY2START,
	COMMON_POTENTIAL_FIRST,
	COMMON_START_POTENTIAL_NOT_FIRST,
	COMMON_POTENTIAL_NOT_FIRST,
	COMMON_POTENTIAL_FIRST_NOT_FIRST,
	COMMON_NOT_FIRST,
        COMMON_CONFIRMED
      } t_common_FSM_state;

    // Enumerated type to represent the logical link between with other letters
    // for first candidate
    typedef enum
      {
        FIRST_NONE,
	FIRST_NEXT
      } t_first_local_link_state;

    // Enumerated type to represent the logical link between with other letters
    // for first candidate
    typedef enum
      {
        NOT_FIRST_NONE,
	NOT_FIRST_NEXT,
	NOT_FIRST_PREVIOUS,
	NOT_FIRST_PREVIOUS_RELOADED
      } t_not_first_local_link_state;

    inline void set_internal_state(const t_common_FSM_state & p_state);

    inline void set_state_first_FSM(const t_first_FSM_state & p_state);
    inline void set_state_not_first_FSM(const t_not_first_FSM_state & p_state);
    
    inline static const std::string not_first_local_link_state2string(const t_not_first_local_link_state & p_state);
    inline static const std::string first_local_link_state2string(const t_first_local_link_state & p_state);
    inline static const std::string state_first_candidate2string(const t_first_FSM_state & p_state);
    inline static const std::string state_not_first_candidate2string(const t_not_first_FSM_state & p_state);
    inline static const std::string common_FSM_state2string(const t_common_FSM_state & p_state);

    inline void print_internal_state(std::ostream & p_stream = std::cout)const;
    inline void align_internal_state(void);

    inline static void add_representation(const t_common_FSM_state & p_common_state,
                                          const t_first_FSM_state & p_first_state,
                                          const t_not_first_FSM_state & p_not_first_FSM_state,
                                          const dicoplus_types::t_cell_FSM_state & p_representation_state);
    inline void first_treat_cancel_message(const t_local_message (&p_messages)[4]);
    inline void not_first_treat_cancel_message(const t_local_message (&p_messages)[4]);
    inline void treat_local_message(const t_local_message (&p_messages)[4],
                                    const unsigned int & p_nb_valid);
    inline void first_treat_local_message(const t_local_message (&p_messages)[4],
                                          const unsigned int & p_nb_valid);
    inline void not_first_treat_local_message(const t_local_message (&p_messages)[4],
                                              const unsigned int & p_nb_valid);

    inline void first_treat_message_completion(void);
    inline void not_first_treat_message_completion(void);
    // Conenctivity
    dicoplus_global_port_manager m_global_port_manager;
    dicoplus_local_input_port* m_local_input_ports[4];
    dicoplus_local_output_port m_local_output_port;

    // Outside listener communication
    cell_listener_if * m_listener;
    dicoplus_types::t_cell_FSM_state m_representation_state;

    // Content
    dicoplus_types::t_global_data_type m_content;

    // FSMs
    t_common_FSM_state m_internal_state;
    t_first_FSM_state m_state_first_candidate;
    t_not_first_FSM_state m_state_not_first_candidate;

    t_first_local_link_state m_first_local_link_states[4];
    t_not_first_local_link_state m_not_first_local_link_states[4];

    // Messages related variables
    const unsigned int m_nb_neighbour;
    unsigned int m_remaining_locals;

    // Variable to set to true by the debugger to activate some messages
    bool m_gdb_debug;

    typedef std::map<std::pair<t_common_FSM_state,std::pair<t_first_FSM_state,t_not_first_FSM_state> >,dicoplus_types::t_cell_FSM_state> t_rep_map;
    static t_rep_map m_representations;
    static ofstream m_status_file;
  };

  //----------------------------------------------------------------------------
  dicoplus_cell::dicoplus_cell(sc_module_name name, 
                               const unsigned int & p_nb_neighbour):
    sc_module(name),
    // Connectivity
    m_clk("clk"),
    m_global_port_manager("global_port_manager",*this),
    m_local_output_port("output"),
    // Outside listener communication
    m_listener(NULL),
    m_representation_state(dicoplus_types::UNINITIALIZED),
    // Content
    m_content(0),
    // FSMs
    m_internal_state(COMMON_UNINITIALIZED),
    m_state_first_candidate(UNKNOWN_FIRST),
    m_state_not_first_candidate(UNKNOWN_NOT_FIRST),
    // Messages related variables
    m_nb_neighbour(p_nb_neighbour),
    m_remaining_locals(m_nb_neighbour),
    m_gdb_debug(false)
      {

	m_global_port_manager.m_clk(m_clk);

        for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
          {
            m_local_input_ports[l_orientation] = new dicoplus_local_input_port(dicoplus_types::orientation2string((dicoplus_types::t_orientation)l_orientation));
            m_first_local_link_states[l_orientation] = FIRST_NONE;
            m_not_first_local_link_states[l_orientation] = NOT_FIRST_NONE;
          }

#ifdef DEBUG_DICOPLUS_CELL
        std::cout << "Constructor of \"" << this->name() <<  "\"" << std::endl ;
#endif //  DEBUG_DICOPLUS_CELL

        SC_THREAD(run);
        sensitive << m_clk.pos();
      }

    //----------------------------------------------------------------------------
    void dicoplus_cell::add_representation(const t_common_FSM_state & p_common_FSM_state,
                                           const t_first_FSM_state & p_first_FSM_state,
                                           const t_not_first_FSM_state & p_not_first_FSM_state,
                                           const dicoplus_types::t_cell_FSM_state & p_representation_state)
    {
      m_representations.insert(
                               t_rep_map::value_type(
                                                     std::pair<t_common_FSM_state,std::pair<t_first_FSM_state,t_not_first_FSM_state> >(
                                                                                                                                       p_common_FSM_state,
                                                                                                                                       std::pair<t_first_FSM_state,t_not_first_FSM_state>(
                                                                                                                                                                                          p_first_FSM_state,
                                                                                                                                                                                          p_not_first_FSM_state
                                                                                                                                                                                          )
                                                                                                                                       )
                                                     ,p_representation_state
                                                     )
                               );
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::set_internal_state(const t_common_FSM_state & p_state)
    {
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
	{
	  std::cout << name() << " : set internal state to \"" << common_FSM_state2string(p_state) << "\"" << std::endl ;
	}
#endif //  DEBUG_DICOPLUS_CELL

      m_internal_state = p_state;
      align_internal_state();
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::set_state_first_FSM(const t_first_FSM_state & p_state)
    {
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
	{
	  std::cout << name() << " : set state first candidate to \"" << state_first_candidate2string(p_state) << "\"" << std::endl ;
	}
#endif //  DEBUG_DICOPLUS_CELL

      m_state_first_candidate = p_state;
      align_internal_state();
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::set_state_not_first_FSM(const t_not_first_FSM_state & p_state)
    {
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
	{
	  std::cout << name() << " : set state not first candidate to \"" << state_not_first_candidate2string(p_state) << "\"" << std::endl ;
	}
#endif //  DEBUG_DICOPLUS_CELL

      m_state_not_first_candidate = p_state;
      align_internal_state();
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::print_internal_state(std::ostream & p_stream)const
    {
      p_stream << std::string(name()) << std::endl ;
      p_stream << "\tInternal state : \t\t" << common_FSM_state2string(m_internal_state) << std::endl ;
      p_stream << "\tFirst candidate state :\t\t" << state_first_candidate2string(m_state_first_candidate) << std::endl ;
      p_stream << "\tNot first candidate state :\t" << state_not_first_candidate2string(m_state_not_first_candidate) << std::endl ;
      for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
	{
	  p_stream << "\t" << dicoplus_types::orientation2string((dicoplus_types::t_orientation)l_orientation) << "\t: " << first_local_link_state2string(m_first_local_link_states[l_orientation]) << "\t" << not_first_local_link_state2string(m_not_first_local_link_states[l_orientation]) << std::endl;
	}
      
      p_stream << std::endl ;
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
      if(m_gdb_debug && COMMON_UNINITIALIZED != m_internal_state && COMMON_INITIALIZED != m_internal_state)
        {
          char l_char_array[5] = {0x0,0x0,0x0,0x0,0x0};
          uint32_t l_cp = dicoplus_char::get_code_point(p_message.get_data().to_uint());
          utf8::append(l_cp,l_char_array);
          std::cout << name() << " : Treat char message '" << l_char_array << "'@ " << sc_time_stamp() << std::endl ;
          print_internal_state();
        }
#endif // DEBUG_DICOPLUS_CELL

      switch(m_internal_state)
	{
	case COMMON_UNINITIALIZED:
          // Use message data to initialise itself
          m_content = p_message.get_data();
          set_internal_state(COMMON_INITIALIZED);
          // Notify the listener of content change for GUI display
          if(m_listener) m_listener->set_content(p_message.get_data().to_uint());
          // Don`t forward the message to following cell !!!!
	  break;
	case COMMON_INITIALIZED:
          // Still in Grid inialisation phase so just forward the message for 
          // following cell initialisations
          m_global_port_manager.post_message(p_message);
	  break;
	case COMMON_READY2START:
          // Check content of message
          if(p_message.get_data() == m_content)
            {
              // We are potentially the first letter of the word as this is the
              // first character after separator message
              set_internal_state(COMMON_POTENTIAL_FIRST);

              // Notify neighbour cells that content is matching
              m_local_output_port.send_data(true);
            }
          else
            {
              // The first message doesn't match our content so we are not the first letter
              set_internal_state(COMMON_NOT_FIRST);
              // Notify neighbour cells that content is not matching
              m_local_output_port.send_data(false);
            }
	  m_global_port_manager.post_message(p_message);
	  break;
	case COMMON_START_POTENTIAL_NOT_FIRST:
          // Check content of message
          if(p_message.get_data() == m_content)
            {
              // We are potentially the first letter of the word as this is the
              // first character after separator message
              set_internal_state(COMMON_POTENTIAL_FIRST_NOT_FIRST);
              set_state_not_first_FSM(POTENTIAL_NOT_FIRST);

              // Notify neighbour cells that content is matching
              m_local_output_port.send_data(true);
            }
          else
            {
              // We are not the first letter of the word as this is the
              // first character after separator message
              set_internal_state(COMMON_POTENTIAL_NOT_FIRST);
              
              // Notify neighbour cells that content is not matching
              m_local_output_port.send_data(false);
            }
	  m_global_port_manager.post_message(p_message);
	  break;
        case COMMON_POTENTIAL_FIRST_NOT_FIRST:
        case COMMON_POTENTIAL_NOT_FIRST:
          if(p_message.get_data() == m_content && (POTENTIAL_NOT_FIRST == m_state_not_first_candidate || POTENTIAL_NOT_FIRST_RELOADED == m_state_not_first_candidate || ATTACHED_NOT_FIRST == m_state_not_first_candidate))
            {
              if(ATTACHED_NOT_FIRST != m_state_not_first_candidate) set_state_not_first_FSM(MATCHING_NOT_FIRST);
              m_local_output_port.send_data(true);
            }
          else
            {
              m_local_output_port.send_data(false);
            }
          // Forward the message
	  m_global_port_manager.post_message(p_message);
          break;
        case COMMON_CONFIRMED:
        case COMMON_POTENTIAL_FIRST:
        case COMMON_NOT_FIRST:
          // Don't care about message content
          m_local_output_port.send_data(false);
          // Forward the message
          m_global_port_manager.post_message(p_message);
          break ;
	default:
	  throw quicky_exception::quicky_logic_exception(std::string(name())+" : Unhandled state to receive a char message : \""+common_FSM_state2string(m_internal_state),__LINE__,__FILE__);
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
          print_internal_state();
        }
#endif // DEBUG_DICOPLUS_CELL

      switch(m_internal_state)
	{
	case COMMON_INITIALIZED :
          // Separator message indicate the end of initialisation phase so cell becomes ready to start
	  set_internal_state(COMMON_READY2START);
          // Information is transmitted to following cells
	  m_global_port_manager.post_message(p_message);
	  break;
	case COMMON_POTENTIAL_FIRST_NOT_FIRST:
	case COMMON_POTENTIAL_NOT_FIRST :
          if(ATTACHED_NOT_FIRST == m_state_not_first_candidate || MATCHED_NOT_FIRST == m_state_not_first_candidate || FIRST == m_state_first_candidate)
            {
              set_internal_state(COMMON_CONFIRMED);
            }
          else if(POTENTIAL_NOT_FIRST == m_state_not_first_candidate)
            {
              set_internal_state(COMMON_READY2START);
              set_state_first_FSM(UNKNOWN_FIRST);
              set_state_not_first_FSM(UNKNOWN_NOT_FIRST);
            }
          // Information is transmitted to following cells
	  m_global_port_manager.post_message(p_message);
	  break;
	case COMMON_NOT_FIRST :
	  // Separator message indicate the end of the current word so cell becomes ready to start
	  set_internal_state(COMMON_READY2START);
          set_state_first_FSM(UNKNOWN_FIRST);
          set_state_not_first_FSM(UNKNOWN_NOT_FIRST);
          // Information is transmitted to following cells
	  m_global_port_manager.post_message(p_message);
	  break;
        case COMMON_POTENTIAL_FIRST:
          set_internal_state(COMMON_CONFIRMED);
          // Information is transmitted to following cells
	  m_global_port_manager.post_message(p_message);
          break;
        case COMMON_CONFIRMED:
          // Forward the message
          m_global_port_manager.post_message(p_message);
          break;
	default:
	  throw quicky_exception::quicky_logic_exception("Unhandled state to receive a separator message : \""+common_FSM_state2string(m_internal_state)+"\" in cell \""+name()+"\"",__LINE__,__FILE__);
	  break;
	}
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::run(void)
    {
      while(1)
	{
          unsigned int l_nb_valid = 0;
          unsigned int l_nb_invalid = 0;
          unsigned int l_nb_local_msg = 0;
	  unsigned int l_nb_cancel = 0;

	  t_local_message l_local_messages[4];

          // Collect local messages
          for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
            {
              if(m_local_input_ports[l_orientation]->is_valid())
                {
#ifdef DEBUG_DICOPLUS_CELL
                  if(m_gdb_debug)
                    {
                      std::cout << name() << " received " << dicoplus_types::local_message_content2string((dicoplus_types::t_local_message_content)m_local_input_ports[l_orientation]->get_data().to_uint()) << " from " << dicoplus_types::orientation2string((dicoplus_types::t_orientation)l_orientation) << std::endl ;
                    }
#endif // DEBUG_DICOPLUS_CELL
		  switch(m_local_input_ports[l_orientation]->get_data().to_uint())
		    {
		    case dicoplus_types::LOCAL_MESSAGE_VALID :
		      ++l_nb_valid;
                      ++l_nb_local_msg;
		      l_local_messages[l_orientation] = VALID;
		      break;
		    case dicoplus_types::LOCAL_MESSAGE_NOT_VALID :
		      ++l_nb_invalid;
                      ++l_nb_local_msg;
		      l_local_messages[l_orientation] = NOT_VALID;
		      break;
		    case dicoplus_types::LOCAL_MESSAGE_CANCEL :
		      ++l_nb_cancel;
		      l_local_messages[l_orientation] = CANCEL;
                      break;
		    default:
		      quicky_exception::quicky_logic_exception("Unhandled local message \""+dicoplus_types::local_message_content2string((dicoplus_types::t_local_message_content)m_local_input_ports[l_orientation]->get_data().to_uint())+"\" in cell \""+std::string(name())+"\"",__LINE__,__FILE__);
		      break;
		    }

                }
              else
                {
                  l_local_messages[l_orientation] = NO_MESSAGE;
                }
            }

          m_local_output_port.no_data();

          // Check number of local messages
	  if(1 < l_nb_local_msg) 
	    {
	      std::stringstream l_stream;
	      l_stream << l_nb_local_msg;
	      throw quicky_exception::quicky_logic_exception(std::string(name())+": Only one local message is possible at a time : "+l_stream.str(),__LINE__,__FILE__);
	    }

          
          if(l_nb_cancel && COMMON_CONFIRMED != m_internal_state)
            {
#ifdef DEBUG_DICOPLUS_CELL
              if(m_gdb_debug)
                {
                  std::cout << name() << " : Treat cancel message @ " << sc_time_stamp() << std::endl ;
                  print_internal_state();
                }
#endif // DEBUG_DICOPLUS_CELL
              not_first_treat_cancel_message(l_local_messages);
              first_treat_cancel_message(l_local_messages);
            }

          // Modify state in function of received messages
          if(l_nb_local_msg)
            {
#ifdef DEBUG_DICOPLUS_CELL
              if(m_gdb_debug)
                {
                  std::cout << name() << " : Treat local message @ " << sc_time_stamp() << std::endl ;
                  print_internal_state();
                }
#endif // DEBUG_DICOPLUS_CELL
              first_treat_local_message(l_local_messages,l_nb_valid);
              not_first_treat_local_message(l_local_messages,l_nb_valid);
              treat_local_message(l_local_messages,l_nb_valid);
            }
	  m_global_port_manager.listen();
          m_remaining_locals -= l_nb_local_msg;
          if(COMMON_CONFIRMED != m_internal_state)
            {
              if(!m_remaining_locals)
                {
#ifdef DEBUG_DICOPLUS_CELL
                  if(m_gdb_debug)
                    {
                      std::cout << name() << " : Message complete" << std::endl ;
                      print_internal_state();
                    }
#endif // DEBUG_DICOPLUS_CELL
                  first_treat_message_completion();
                  not_first_treat_message_completion();
                  
                  m_remaining_locals = m_nb_neighbour;
                }
            }
	  wait();
	}
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::treat_local_message(const t_local_message (&p_messages)[4],
                                            const unsigned int & p_nb_valid)
    {
      switch(m_internal_state)
        {
        case COMMON_INITIALIZED:
        case COMMON_UNINITIALIZED:
          // Ignore local messages
          break;
        case COMMON_READY2START:
          // One of my previous neighbour just match a char message so
          // So I`m potentialy not first letter
          if(VALID == p_messages[dicoplus_types::WEST] || VALID == p_messages[dicoplus_types::NORTH] )
            {
              set_internal_state(COMMON_START_POTENTIAL_NOT_FIRST);

              // Memorize which cells provide the potentiality
              m_not_first_local_link_states[dicoplus_types::WEST] = (VALID == p_messages[dicoplus_types::WEST] ? NOT_FIRST_PREVIOUS : NOT_FIRST_NONE );
              m_not_first_local_link_states[dicoplus_types::NORTH] = (VALID == p_messages[dicoplus_types::NORTH] ? NOT_FIRST_PREVIOUS : NOT_FIRST_NONE );
            }
          break;
        case COMMON_POTENTIAL_FIRST:
          if(p_nb_valid)
            {
              set_internal_state(COMMON_POTENTIAL_FIRST_NOT_FIRST);
            }
          break;
        case COMMON_NOT_FIRST:
          // One of my next neighbour just match a char message so
          // So I`m potentialy not first letter
          if(p_nb_valid)
            {
              set_internal_state(COMMON_POTENTIAL_NOT_FIRST);
              // Memorize which cells provide the potentiality
              for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
                {
                  m_not_first_local_link_states[l_orientation] = (VALID == p_messages[l_orientation] ? NOT_FIRST_PREVIOUS : NOT_FIRST_NONE );
                }
            }
          break;
        case COMMON_START_POTENTIAL_NOT_FIRST:
          // Nothing to do. Only the char message will change this state 
          // if this is potential first also
          break;
        case COMMON_POTENTIAL_NOT_FIRST:
          // Nothing to do, I`m already potential
          break;
        case COMMON_POTENTIAL_FIRST_NOT_FIRST:
          // Nothing to do. Event is managed by first_candidate FSM and not first_candidate FSM
          break;
        case COMMON_CONFIRMED:
          // Nothing to do 
          break;
        default:
          throw quicky_exception::quicky_logic_exception(std::string(name())+" : Unhandled state to receive a local message : \""+common_FSM_state2string(m_internal_state)+"\" in cell \""+name()+"\"",__LINE__,__FILE__);
          break;
        }
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::first_treat_message_completion(void)
    {
      switch(m_state_first_candidate)
        {
        case UNKNOWN_FIRST:
          if(COMMON_POTENTIAL_FIRST == m_internal_state || COMMON_POTENTIAL_FIRST_NOT_FIRST == m_internal_state)
            {
              // Set state of FSM dedicated to first letter candidate
              set_state_first_FSM(POTENTIAL_FIRST);
            }
          break;
        case POTENTIAL_FIRST:
          // Means that we did`nt receive neighbour validation
          set_state_first_FSM(UNKNOWN_FIRST);

          // Now we are sure that this is not first
          set_internal_state(COMMON_NOT_FIRST);
          break;
        case FIRST:
          // Nothing to do
          break;
        default:
          throw quicky_exception::quicky_logic_exception(std::string(name())+" : Unhandled first candidate state to receive a local message : \""+state_first_candidate2string(m_state_first_candidate)+"\" in cell \""+name()+"\"",__LINE__,__FILE__);
          break;
        }
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::not_first_treat_message_completion(void)
    {
      switch(m_state_not_first_candidate)
        {
        case UNKNOWN_NOT_FIRST:
          if(COMMON_POTENTIAL_NOT_FIRST == m_internal_state || COMMON_POTENTIAL_FIRST_NOT_FIRST == m_internal_state)
            {
              // Set state of FSM dedicated to not_first letter candidate
              set_state_not_first_FSM(POTENTIAL_NOT_FIRST);
            }
          break;
        case  MATCHING_NOT_FIRST:
          set_state_not_first_FSM(MATCHED_NOT_FIRST);
          break;
        case POTENTIAL_NOT_FIRST_RELOADED:
          set_state_not_first_FSM(POTENTIAL_NOT_FIRST);
          // Scan local link state to indicate that previous link is no more valid
          // TO BE DONE

          // Reset local link state
          for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
            {
              if(NOT_FIRST_PREVIOUS == m_not_first_local_link_states[l_orientation])
                {
                  m_not_first_local_link_states[l_orientation] = NOT_FIRST_NONE;
                }
              else if(NOT_FIRST_PREVIOUS_RELOADED == m_not_first_local_link_states[l_orientation])
                {
                  m_not_first_local_link_states[l_orientation] = NOT_FIRST_PREVIOUS;
                }
            }

          break;
        case POTENTIAL_NOT_FIRST:
          // Means that we did`nt receive neighbour validation
          set_state_not_first_FSM(UNKNOWN_NOT_FIRST);

          // Now we are sure that this is not first
          set_internal_state(COMMON_POTENTIAL_NOT_FIRST == m_internal_state ? COMMON_NOT_FIRST : COMMON_POTENTIAL_FIRST);

          // Scan local link state to indicate that previous link is no more valid
          // TO BE DONE

          // Reset local link state
          for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
            {
              if(NOT_FIRST_PREVIOUS == m_not_first_local_link_states[l_orientation])
                {
                  m_not_first_local_link_states[l_orientation] = NOT_FIRST_NONE;
                }
            }

          break;
        case ATTACHED_NOT_FIRST:
          // Nothing to do
          break;
        case MATCHED_NOT_FIRST:
          // The latest matched character didn't receive any neighbour
          // validation so it must be reverted
          set_state_not_first_FSM(UNKNOWN_NOT_FIRST);
          set_internal_state(COMMON_POTENTIAL_NOT_FIRST == m_internal_state ? COMMON_NOT_FIRST : COMMON_POTENTIAL_FIRST);

          // Send invalid message to predecessors
          m_local_output_port.cancel();
          break;
        default:
          throw quicky_exception::quicky_logic_exception(std::string(name())+" : Unhandled not_first candidate state to receive a local message : \""+state_not_first_candidate2string(m_state_not_first_candidate)+"\" in cell \""+name()+"\"",__LINE__,__FILE__);
          break;
                  
        }
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::first_treat_local_message(const t_local_message (&p_messages)[4],
                                                  const unsigned int & p_nb_valid)
    {
      if((POTENTIAL_FIRST == m_state_first_candidate || FIRST == m_state_first_candidate) && 1 == p_nb_valid)
        {
          set_state_first_FSM(FIRST);
          for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
            {
              if(VALID == p_messages[l_orientation])
                {
                  m_first_local_link_states[l_orientation] = FIRST_NEXT;
                }
            }
        }
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::not_first_treat_local_message(const t_local_message (&p_messages)[4],
                                                      const unsigned int & p_nb_valid)
    {
      if((MATCHED_NOT_FIRST == m_state_not_first_candidate || ATTACHED_NOT_FIRST == m_state_not_first_candidate) && 1 == p_nb_valid)
        {
          set_state_not_first_FSM(ATTACHED_NOT_FIRST);
          for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
            {
              if(VALID == p_messages[l_orientation])
                {
                  m_not_first_local_link_states[l_orientation] = NOT_FIRST_NEXT;
                }
            }
        }
      if(POTENTIAL_NOT_FIRST == m_state_not_first_candidate && 1 == p_nb_valid)
        {
          set_state_not_first_FSM(POTENTIAL_NOT_FIRST_RELOADED);
          for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
            {
              if(VALID == p_messages[l_orientation])
                {
                  m_not_first_local_link_states[l_orientation] = NOT_FIRST_PREVIOUS_RELOADED;
                }
            }

        }
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::first_treat_cancel_message(const t_local_message (&p_messages)[4])
    {
      switch(m_state_first_candidate)
        {
        case FIRST:
          {
            unsigned int l_remaining_valid_links = 0;
            for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
              {
                if(FIRST_NEXT == m_first_local_link_states[l_orientation])
                  {
                    if(CANCEL == p_messages[l_orientation])
                      {
                        m_first_local_link_states[l_orientation] = FIRST_NONE;
                      }
                    else
                      {
                        ++l_remaining_valid_links;
                      }
                  }
              }
            if(!l_remaining_valid_links)
              {
                set_state_first_FSM(UNKNOWN_FIRST);
                set_internal_state(COMMON_NOT_FIRST);
              }
          }
          break;
        case UNKNOWN_FIRST:
          break;
        default:
          throw quicky_exception::quicky_logic_exception(std::string(name())+" : Unhandled first candidate state to receive a cancel message : \""+state_first_candidate2string(m_state_first_candidate)+"\" in cell \""+name()+"\"",__LINE__,__FILE__);
          break;
          
        }    
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::not_first_treat_cancel_message(const t_local_message (&p_messages)[4])
    {
      switch(m_state_not_first_candidate)
        {
        case POTENTIAL_NOT_FIRST_RELOADED:
          {
            unsigned int l_remaining_valid_links = 0;
            for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
              {
                if(NOT_FIRST_PREVIOUS == m_not_first_local_link_states[l_orientation] || NOT_FIRST_PREVIOUS_RELOADED == m_not_first_local_link_states[l_orientation])
                  {
                    if(CANCEL == p_messages[l_orientation])
                      {
                        m_not_first_local_link_states[l_orientation] = NOT_FIRST_NONE;
                      }
                    else
                      {
                        ++l_remaining_valid_links;
                      }
                  }
              }
            if(!l_remaining_valid_links)
              {
                set_state_not_first_FSM(UNKNOWN_NOT_FIRST);
                set_internal_state(COMMON_POTENTIAL_NOT_FIRST == m_internal_state ? COMMON_NOT_FIRST : COMMON_POTENTIAL_FIRST);
              }
          }
                      
          break;
        case POTENTIAL_NOT_FIRST:
          {
            unsigned int l_remaining_valid_links = 0;
            for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
              {
                if(NOT_FIRST_PREVIOUS == m_not_first_local_link_states[l_orientation])
                  {
                    if(CANCEL == p_messages[l_orientation])
                      {
                        m_not_first_local_link_states[l_orientation] = NOT_FIRST_NONE;
                      }
                    else
                      {
                        ++l_remaining_valid_links;
                      }
                  }
              }
            if(!l_remaining_valid_links)
              {
                set_state_not_first_FSM(UNKNOWN_NOT_FIRST);
                set_internal_state(COMMON_POTENTIAL_NOT_FIRST == m_internal_state ? COMMON_NOT_FIRST : COMMON_POTENTIAL_FIRST);
              }
          }
          break;
        case UNKNOWN_NOT_FIRST:
          break;
        case ATTACHED_NOT_FIRST:
          {
            unsigned int l_remaining_valid_links = 0;
            for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
              {
                if(NOT_FIRST_NEXT == m_not_first_local_link_states[l_orientation])
                  {
                    if(CANCEL == p_messages[l_orientation])
                      {
                        m_not_first_local_link_states[l_orientation] = NOT_FIRST_NONE;
                      }
                    else
                      {
                        ++l_remaining_valid_links;
                      }
                  }
              }
            if(!l_remaining_valid_links)
              {
                set_state_not_first_FSM(UNKNOWN_NOT_FIRST);
                set_internal_state(COMMON_POTENTIAL_NOT_FIRST == m_internal_state ? COMMON_NOT_FIRST : COMMON_POTENTIAL_FIRST);
                m_local_output_port.cancel();
              }
          }
          break;
        case MATCHED_NOT_FIRST:
          // Nothing to do
          break;
        default:
          throw quicky_exception::quicky_logic_exception(std::string(name())+" : Unhandled not_first candidate state to receive a cancel message : \""+state_not_first_candidate2string(m_state_not_first_candidate)+"\" in cell \""+name()+"\"",__LINE__,__FILE__);
          break;
        }
    }


    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_north_port(dicoplus_local_bus & p_bus)
    {
      (*m_local_input_ports[dicoplus_types::NORTH])(p_bus);
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_east_port(dicoplus_local_bus & p_bus)
    {
      (*m_local_input_ports[dicoplus_types::EAST])(p_bus);
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_south_port(dicoplus_local_bus & p_bus)
    {
      (*m_local_input_ports[dicoplus_types::SOUTH])(p_bus);
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_west_port(dicoplus_local_bus & p_bus)
    {
      (*m_local_input_ports[dicoplus_types::WEST])(p_bus);
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_output_port(dicoplus_local_bus & p_bus)
    {
      m_local_output_port(p_bus);      
    }
    //----------------------------------------------------------------------------
    void dicoplus_cell::align_internal_state(void)
    {
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
	{
          print_internal_state();
        }
#endif //  DEBUG_DICOPLUS_CELL

      bool l_modified = false;
      t_rep_map::const_iterator l_iter = m_representations.find(std::pair<t_common_FSM_state,std::pair<t_first_FSM_state,t_not_first_FSM_state> >(m_internal_state,std::pair<t_first_FSM_state,t_not_first_FSM_state>(m_state_first_candidate,m_state_not_first_candidate)));
      if(m_representations.end() != l_iter)
        {
          l_modified = m_representation_state != l_iter->second;
          m_representation_state = l_iter->second;
        }
      else
        {
          throw quicky_exception::quicky_logic_exception(std::string(name())+" : No representation for state (" + common_FSM_state2string(m_internal_state) +"," + state_first_candidate2string(m_state_first_candidate) + "," + state_not_first_candidate2string(m_state_not_first_candidate) + ")",__LINE__,__FILE__);
        }


      if(m_listener && l_modified) m_listener->set_state(m_representation_state);
    }

    //----------------------------------------------------------------------------
    dicoplus_cell::~dicoplus_cell(void)
      {
	print_internal_state(m_status_file);
        for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
          {
            delete m_local_input_ports[l_orientation];
          }
      }

    //----------------------------------------------------------------------------
    const std::string dicoplus_cell::state_first_candidate2string(const t_first_FSM_state & p_state)
      {
        switch(p_state)
          {
          case UNKNOWN_FIRST:
            return "UNKNOWN_FIRST";
            break;
          case POTENTIAL_FIRST:
            return "POTENTIAL_FIRST";
            break;
          case FIRST:
            return "FIRST";
            break;
          default:
            std::stringstream l_stream;
            l_stream << p_state;
            throw quicky_exception::quicky_logic_exception("No string representation for first candidate state \""+l_stream.str()+"\"",__LINE__,__FILE__);
            break;
          }
      }

    //----------------------------------------------------------------------------
    const std::string dicoplus_cell::state_not_first_candidate2string(const t_not_first_FSM_state & p_state)
      {
        switch(p_state)
          {
          case UNKNOWN_NOT_FIRST:
            return "UNKNOWN_NOT_FIRST";
            break;
          case POTENTIAL_NOT_FIRST:
            return "POTENTIAL_NOT_FIRST";
            break;
          case POTENTIAL_NOT_FIRST_RELOADED:
            return "POTENTIAL_NOT_FIRST_RELOADED";
            break;
          case MATCHING_NOT_FIRST:
            return "MATCHING_NOT_FIRST";
            break;
          case MATCHED_NOT_FIRST:
            return "MATCHED_NOT_FIRST";
            break;
          case ATTACHED_NOT_FIRST:
            return "ATTACHED_NOT_FIRST";
            break;
          case NOT_FIRST:
            return "NOT_FIRST";
            break;
          default:
            std::stringstream l_stream;
            l_stream << p_state;
            throw quicky_exception::quicky_logic_exception("No string representation for not_first candidate state \""+l_stream.str()+"\"",__LINE__,__FILE__);
            break;
          }
      }

    //----------------------------------------------------------------------------
    const std::string dicoplus_cell::common_FSM_state2string(const t_common_FSM_state & p_state)
      {
        switch(p_state)
          {
          case COMMON_UNINITIALIZED:
            return "COMMON_UNINITIALIZED";
            break;
          case COMMON_INITIALIZED:
            return "COMMON_INITIALIZED";
            break;
          case COMMON_READY2START:
            return "COMMON_READY2START";
            break;
          case COMMON_POTENTIAL_FIRST:
            return "COMMON_POTENTIAL_FIRST";
            break;
          case COMMON_START_POTENTIAL_NOT_FIRST:
            return "COMMON_START_POTENTIAL_NOT_FIRST";
            break;
          case COMMON_POTENTIAL_NOT_FIRST:
            return "COMMON_POTENTIAL_NOT_FIRST";
            break;
          case COMMON_POTENTIAL_FIRST_NOT_FIRST:
            return "COMMON_POTENTIAL_FIRST_NOT_FIRST";
            break;
          case COMMON_NOT_FIRST:
            return "COMMON_NOT_FIRST";
            break;
          case COMMON_CONFIRMED:
            return "COMMON_CONFIRMED";
            break;
          default:
            std::stringstream l_stream;
            l_stream << p_state;
            throw quicky_exception::quicky_logic_exception("No string representation for common FSM state \""+l_stream.str()+"\"",__LINE__,__FILE__);
            break;
          }
      }


    //--------------------------------------------------------------------------
    const std::string dicoplus_cell::not_first_local_link_state2string(const dicoplus_cell::t_not_first_local_link_state & p_state)
      {
        switch(p_state)
          {
          case NOT_FIRST_NONE:
            return "NOT_FIRST_NONE";
            break;
          case NOT_FIRST_NEXT:
            return "NOT_FIRST_NEXT";
            break;
          case NOT_FIRST_PREVIOUS:
            return "NOT_FIRST_PREVIOUS";
            break;
          case NOT_FIRST_PREVIOUS_RELOADED:
            return "NOT_FIRST_PREVIOUS_RELOADED";
            break;
          default:
            std::stringstream l_stream;
            l_stream << p_state;
            throw quicky_exception::quicky_logic_exception("No string representation for not first local link state \""+l_stream.str()+"\"",__LINE__,__FILE__);
            break;
          }
      }

    //--------------------------------------------------------------------------
    const std::string dicoplus_cell::first_local_link_state2string(const dicoplus_cell::t_first_local_link_state & p_state)
      {
        switch(p_state)
          {
          case FIRST_NONE:
            return "FIRST_NONE";
            break;
          case FIRST_NEXT:
            return "FIRST_NEXT";
            break;
          default:
            std::stringstream l_stream;
            l_stream << p_state;
            throw quicky_exception::quicky_logic_exception("No string representation for first local link state \""+l_stream.str()+"\"",__LINE__,__FILE__);
            break;
          }
      }

    //--------------------------------------------------------------------------
    void dicoplus_cell::init(void)
    {
      if(!m_representations.size())
        {
          add_representation(COMMON_INITIALIZED,UNKNOWN_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::INITIALIZED);
          add_representation(COMMON_READY2START,UNKNOWN_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::READY2START);
          add_representation(COMMON_READY2START,UNKNOWN_FIRST,POTENTIAL_NOT_FIRST,dicoplus_types::POTENTIAL_NOT_FIRST);
          add_representation(COMMON_READY2START,UNKNOWN_FIRST,MATCHED_NOT_FIRST,dicoplus_types::MATCHED_NOT_FIRST);
          add_representation(COMMON_READY2START,UNKNOWN_FIRST,ATTACHED_NOT_FIRST,dicoplus_types::ATTACHED_NOT_FIRST);
          add_representation(COMMON_START_POTENTIAL_NOT_FIRST,UNKNOWN_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::START_POTENTIAL_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_FIRST,UNKNOWN_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::POTENTIAL_FIRST);
          add_representation(COMMON_POTENTIAL_FIRST,POTENTIAL_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::POTENTIAL_FIRST);
          add_representation(COMMON_POTENTIAL_FIRST,FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,UNKNOWN_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::POTENTIAL_FIRST_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,UNKNOWN_FIRST,POTENTIAL_NOT_FIRST,dicoplus_types::POTENTIAL_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,POTENTIAL_FIRST,POTENTIAL_NOT_FIRST,dicoplus_types::POTENTIAL_FIRST_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,POTENTIAL_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::POTENTIAL_FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,UNKNOWN_FIRST,POTENTIAL_NOT_FIRST_RELOADED,dicoplus_types::POTENTIAL_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,POTENTIAL_FIRST,POTENTIAL_NOT_FIRST_RELOADED,dicoplus_types::POTENTIAL_FIRST_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,FIRST,POTENTIAL_NOT_FIRST,dicoplus_types::FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,FIRST,MATCHING_NOT_FIRST,dicoplus_types::FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,FIRST,MATCHED_NOT_FIRST,dicoplus_types::MATCHED_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,FIRST,ATTACHED_NOT_FIRST,dicoplus_types::ATTACHED_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,FIRST,POTENTIAL_NOT_FIRST_RELOADED,dicoplus_types::FIRST);
          add_representation(COMMON_POTENTIAL_FIRST_NOT_FIRST,FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::FIRST);
          add_representation(COMMON_POTENTIAL_NOT_FIRST,UNKNOWN_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::POTENTIAL_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_NOT_FIRST,UNKNOWN_FIRST,POTENTIAL_NOT_FIRST,dicoplus_types::POTENTIAL_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_NOT_FIRST,UNKNOWN_FIRST,MATCHING_NOT_FIRST,dicoplus_types::MATCHING_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_NOT_FIRST,UNKNOWN_FIRST,MATCHED_NOT_FIRST,dicoplus_types::MATCHED_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_NOT_FIRST,UNKNOWN_FIRST,ATTACHED_NOT_FIRST,dicoplus_types::ATTACHED_NOT_FIRST);
          add_representation(COMMON_POTENTIAL_NOT_FIRST,UNKNOWN_FIRST,POTENTIAL_NOT_FIRST_RELOADED,dicoplus_types::POTENTIAL_NOT_FIRST);
          add_representation(COMMON_NOT_FIRST,UNKNOWN_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::NOT_FIRST);
          add_representation(COMMON_NOT_FIRST,UNKNOWN_FIRST,POTENTIAL_NOT_FIRST,dicoplus_types::POTENTIAL_NOT_FIRST);
          add_representation(COMMON_CONFIRMED,FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::CONFIRMED_FIRST);
          add_representation(COMMON_CONFIRMED,UNKNOWN_FIRST,ATTACHED_NOT_FIRST,dicoplus_types::CONFIRMED_MIDDLE);
          add_representation(COMMON_CONFIRMED,FIRST,ATTACHED_NOT_FIRST,dicoplus_types::CONFIRMED_MIDDLE);
          add_representation(COMMON_CONFIRMED,FIRST,POTENTIAL_NOT_FIRST,dicoplus_types::CONFIRMED_MIDDLE);
          add_representation(COMMON_CONFIRMED,FIRST,POTENTIAL_NOT_FIRST_RELOADED,dicoplus_types::CONFIRMED_MIDDLE);
          add_representation(COMMON_CONFIRMED,UNKNOWN_FIRST,MATCHED_NOT_FIRST,dicoplus_types::CONFIRMED_LAST);

          m_status_file.open("status.log");
          if(!m_status_file)
            {
              throw quicky_exception::quicky_logic_exception("Unable to create status.log file",__LINE__,__FILE__);
            }
        }
      else
        {
          throw quicky_exception::quicky_logic_exception("dicoplus_cell representation already initialised",__LINE__,__FILE__);

        }
    }

    //--------------------------------------------------------------------------
    void dicoplus_cell::terminate(void)
    {
      m_status_file.close();
    }
  
}
#endif // _DICOPLUS_CELL_H_
//EOF
