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
    inline void bind_local_input_port(const dicoplus_types::t_orientation & p_orientation,
				      dicoplus_local_bus & p_bus);
    inline void bind_local_output_port(const dicoplus_types::t_orientation & p_orientation,
				       dicoplus_local_bus & p_bus);

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
        MATCHED_NOT_FIRST,
        ATTACHED_NOT_FIRST
      } t_not_first_FSM_state;

    // Possible states of common internal FSM
    typedef enum
      {
	COMMON_UNINITIALIZED=0,
	COMMON_INITIALIZED,
	COMMON_READY2START,
	COMMON_FIRST_NOT_FIRST,
	COMMON_NOT_FIRST,
        COMMON_CONFIRMED
      } t_common_FSM_state;

    inline void set_internal_state(const t_common_FSM_state & p_state,
                                   bool p_align=true);

    inline void set_state_first_FSM(const t_first_FSM_state & p_state,
                                    bool p_align=true);
    inline void set_state_not_first_FSM(const t_not_first_FSM_state & p_state,
                                        bool p_align=true);
    
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
    inline void send_data(bool p_data);
    inline void no_data(void);

    // Connectivity
    dicoplus_global_port_manager m_global_port_manager;
    dicoplus_local_input_port* m_local_input_ports[4];
    dicoplus_local_output_port* m_local_output_ports[4];

    // Outside listener communication
    cell_listener_if * m_listener;
    dicoplus_types::t_cell_FSM_state m_representation_state;

    // Content
    dicoplus_types::t_global_data_type m_content;

    // FSMs
    t_common_FSM_state m_internal_state;
    t_first_FSM_state m_state_first_candidate;
    t_not_first_FSM_state m_state_not_first_candidate;

    dicoplus_link_state m_local_link_states[4];
    bool m_last_match;

    // Messages related variables
    const unsigned int m_nb_neighbour;
    unsigned int m_remaining_locals;
    unsigned int m_nb_valid_msg;
    unsigned int m_nb_invalid_msg;
    t_local_message m_local_messages[4];

    // Variable to set to true by the debugger to activate some messages
    bool m_gdb_debug;

    char m_char_array[5];

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
    // Outside listener communication
    m_listener(NULL),
    m_representation_state(dicoplus_types::UNINITIALIZED),
    // Content
    m_content(0),
    // FSMs
    m_internal_state(COMMON_UNINITIALIZED),
    m_state_first_candidate(UNKNOWN_FIRST),
    m_state_not_first_candidate(UNKNOWN_NOT_FIRST),
    m_last_match(false),
    // Messages related variables
    m_nb_neighbour(p_nb_neighbour),
    m_remaining_locals(m_nb_neighbour),
    m_nb_valid_msg(0),
    m_nb_invalid_msg(0),
    // Variable to set to true by the debugger to activate some messages
    m_gdb_debug(false)
      {

	m_global_port_manager.m_clk(m_clk);
        memset(m_char_array,0x0,5*sizeof(char));
        for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
          {
            m_local_input_ports[l_orientation] = new dicoplus_local_input_port(dicoplus_types::orientation2string((dicoplus_types::t_orientation)l_orientation)+"_input");
            m_local_output_ports[l_orientation] = new dicoplus_local_output_port(dicoplus_types::orientation2string((dicoplus_types::t_orientation)l_orientation)+"_output");
            m_local_link_states[l_orientation].set_orientation((dicoplus_types::t_orientation)l_orientation);
            m_local_messages[l_orientation] = NO_MESSAGE;
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
    void dicoplus_cell::set_internal_state(const t_common_FSM_state & p_state,
                                           bool p_align)
    {
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
	{
	  std::cout << name() << " : set internal state to \"" << common_FSM_state2string(p_state) << "\"" << std::endl ;
	}
#endif //  DEBUG_DICOPLUS_CELL

      m_internal_state = p_state;
      if(p_align) align_internal_state();
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::set_state_first_FSM(const t_first_FSM_state & p_state,
                                            bool p_align)
    {
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
	{
	  std::cout << name() << " : set state first candidate to \"" << state_first_candidate2string(p_state) << "\"" << std::endl ;
	}
#endif //  DEBUG_DICOPLUS_CELL

      m_state_first_candidate = p_state;
      if(p_align) align_internal_state();
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::set_state_not_first_FSM(const t_not_first_FSM_state & p_state,
                                                bool p_align)
    {
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
	{
	  std::cout << name() << " : set state not first candidate to \"" << state_not_first_candidate2string(p_state) << "\"" << std::endl ;
	}
#endif //  DEBUG_DICOPLUS_CELL

      m_state_not_first_candidate = p_state;
      if(p_align) align_internal_state();
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::print_internal_state(std::ostream & p_stream)const
    {
      p_stream << std::string(name()) << " : '" << m_char_array << "'" << std::endl ;
      p_stream << "\tInternal state : \t\t" << common_FSM_state2string(m_internal_state) << std::endl ;
      p_stream << "\tFirst candidate state :\t\t" << state_first_candidate2string(m_state_first_candidate) << std::endl ;
      p_stream << "\tNot first candidate state :\t" << state_not_first_candidate2string(m_state_not_first_candidate) << std::endl ;
      for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
	{
	  p_stream << "\t" << m_local_link_states[l_orientation] << std::endl;
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
          {
            // Use message data to initialise itself
            m_content = p_message.get_data();
            set_internal_state(COMMON_INITIALIZED);
            uint32_t l_cp = dicoplus_char::get_code_point(p_message.get_data().to_uint());
            utf8::append(l_cp,m_char_array);
            
            // Notify the listener of content change for GUI display
            if(m_listener) m_listener->set_content(p_message.get_data().to_uint());
            // Don`t forward the message to following cell !!!!
          }
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
              set_internal_state(COMMON_FIRST_NOT_FIRST);

              // Notify neighbour cells that content is matching
              send_data(true);
            }
          else
            {
              // The first message doesn't match our content so we are not the first letter
              set_internal_state(COMMON_NOT_FIRST);

              // Notify neighbour cells that content is not matching
              send_data(false);
            }
	  m_global_port_manager.post_message(p_message);
	  break;
	case COMMON_FIRST_NOT_FIRST:
	case COMMON_NOT_FIRST:
	  {
	    m_last_match = p_message.get_data() == m_content;
	    bool l_notify_ok = false;
	    // Notify neighbour cells about content matching results
	    for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
	      {
		l_notify_ok |= m_local_link_states[l_orientation].is_potential_predecessor();
	      }
	    
	    send_data(l_notify_ok && p_message.get_data() == m_content);
          // Forward the message
	    m_global_port_manager.post_message(p_message);
	  }
	  break;
        case COMMON_CONFIRMED:
          // Don't care about message content
	  send_data(false);
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
	case COMMON_FIRST_NOT_FIRST:
	case COMMON_NOT_FIRST :
          {
            bool l_confirmed = false;
            if(FIRST == m_state_first_candidate)
              {
                l_confirmed = true;
              }
            else
              {
                set_state_first_FSM(UNKNOWN_FIRST,false);
              }

            if(MATCHED_NOT_FIRST == m_state_not_first_candidate ||
               ATTACHED_NOT_FIRST == m_state_not_first_candidate)
              {
                l_confirmed =true;
                if(MATCHED_NOT_FIRST == m_state_not_first_candidate)
                  {
                    for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
                      {
                        if(m_local_link_states[l_orientation].is_matched_predecessor()) m_local_link_states[l_orientation].confirm_predecessor();
                      }
                  }

              }
            else
              {
                set_state_not_first_FSM(UNKNOWN_NOT_FIRST,false);
              }

            // Clean links
            for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
              {
                m_local_link_states[l_orientation].clean(!l_confirmed);
              }
 
	  // Reinitialise internal state
            if(!l_confirmed)
              {
                set_internal_state(COMMON_READY2START,true);
              }
            else
              {
                set_internal_state(COMMON_CONFIRMED,true);
              }

            // Information is transmitted to following cells
            m_global_port_manager.post_message(p_message);
          }
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
          unsigned int l_nb_local_msg = 0;
          unsigned int l_nb_cancel_msg = 0;
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
		      ++m_nb_valid_msg;
                      ++l_nb_local_msg;
		      m_local_messages[l_orientation] = VALID;
		      break;
		    case dicoplus_types::LOCAL_MESSAGE_NOT_VALID :
		      ++m_nb_invalid_msg;
                      ++l_nb_local_msg;
		      m_local_messages[l_orientation] = NOT_VALID;
		      break;
		    case dicoplus_types::LOCAL_MESSAGE_CANCEL :
		      ++l_nb_cancel_msg;
		      m_local_messages[l_orientation] = CANCEL;
                      break;
		    default:
		      quicky_exception::quicky_logic_exception("Unhandled local message \""+dicoplus_types::local_message_content2string((dicoplus_types::t_local_message_content)m_local_input_ports[l_orientation]->get_data().to_uint())+"\" in cell \""+std::string(name())+"\"",__LINE__,__FILE__);
		      break;
		    }

                }
            }

          no_data();

          // Check number of local messages
	  if(1 < l_nb_local_msg) 
	    {
	      std::stringstream l_stream;
	      l_stream << l_nb_local_msg;
	      throw quicky_exception::quicky_logic_exception(std::string(name())+": Only one local message is possible at a time : "+l_stream.str(),__LINE__,__FILE__);
	    }

          
          if(l_nb_cancel_msg && COMMON_CONFIRMED != m_internal_state)
            {
	      if(COMMON_FIRST_NOT_FIRST == m_internal_state)
		{
		  first_treat_cancel_message(m_local_messages);
		}
              not_first_treat_cancel_message(m_local_messages);
            }

	  m_global_port_manager.listen();
          m_remaining_locals -= l_nb_local_msg;
          if(COMMON_CONFIRMED != m_internal_state)
            {
              if(!m_remaining_locals)
                {
                  if(COMMON_FIRST_NOT_FIRST == m_internal_state)
                    {
                      first_treat_message_completion();
                    }
                  not_first_treat_message_completion();
                  
                  //Reinitialise messages related variables
                  for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
                    {
                      m_local_messages[l_orientation] = NO_MESSAGE;
                    }

                  m_remaining_locals = m_nb_neighbour;
                  m_nb_valid_msg = 0;
                  m_nb_invalid_msg = 0;
                }
            }
	  wait();
	}
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::treat_local_message(const t_local_message (&p_messages)[4],
                                            const unsigned int & p_nb_valid)
    {
      // To be removed
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::first_treat_message_completion(void)
    {
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
	{
	  std::cout << name() << " : First FSM : Messages complete" << std::endl ;
	  print_internal_state();
	}
#endif // DEBUG_DICOPLUS_CELL

      switch(m_state_first_candidate)
        {
        case UNKNOWN_FIRST:
          for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
            {
              m_local_link_states[l_orientation].set_potential_first_predecessor();
            }         
          set_state_first_FSM(POTENTIAL_FIRST);
          break;
        case POTENTIAL_FIRST:
          {
            // Check if we received neighbour validation that would confirm the first position
            bool l_confirmed = false; 
            for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
              {
                if(m_local_link_states[l_orientation].is_potential_first_predecessor())
                  {
                    if(VALID == m_local_messages[l_orientation])
                      {
                        m_local_link_states[l_orientation].confirm_first_predecessor();
                        l_confirmed = true;
                      }
                    else
                      {
                        m_local_link_states[l_orientation].cancel_first_predecessor();
                      }
                  }    
              } 

            if(l_confirmed)
              {
                // Means that we did receive neighbour validation
                set_state_first_FSM(FIRST);
              }
            else
              {
                // Means that we did`nt receive neighbour validation
                set_state_first_FSM(UNKNOWN_FIRST,false);

                // Now we are sure that this is not first
                set_internal_state(COMMON_NOT_FIRST);
              }              
          }
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
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
	{
	  std::cout << name() << " : Not First FSM : Messages complete" << std::endl ;
	  print_internal_state();
	  ;
	}
#endif // DEBUG_DICOPLUS_CELL
      switch(m_state_not_first_candidate)
        {
        case ATTACHED_NOT_FIRST:
        case MATCHED_NOT_FIRST:
	case POTENTIAL_NOT_FIRST:
          for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
            {
	      // Search predecessor link that need to be confirmed
	      if(m_local_link_states[l_orientation].is_matched_predecessor())
		{
		  bool l_confirmed = false;
		  // Search VALID message to confirm
                  for(unsigned int l_orientation2 = dicoplus_types::NORTH; l_orientation2 < dicoplus_types::WEST + 1; ++l_orientation2)
                    {
		      if(l_orientation2 != l_orientation 
                         && m_local_link_states[l_orientation2].is_potential_successor_of((dicoplus_types::t_orientation)l_orientation) 
			 )
                        {
			  if(VALID == m_local_messages[l_orientation2])
			    {
			      l_confirmed = true;
			      m_local_link_states[l_orientation2].confirm_successor_of((dicoplus_types::t_orientation)l_orientation);
			    }
			  else if(NOT_VALID == m_local_messages[l_orientation])
			    {
			      // Invalid potential successors
			      m_local_link_states[l_orientation2].cancel_successor_of((dicoplus_types::t_orientation)l_orientation);
			    }
			}
                    }
		  if(l_confirmed) 
		    {
		      m_local_link_states[l_orientation].confirm_predecessor();
		    }
		  else
		    {
		      m_local_link_states[l_orientation].cancel_matched_predecessor();
		      // Send cancel on local port
#ifdef DEBUG_DICOPLUS_CELL
		      if(m_gdb_debug)
			{
			  std::cout << name() << " : Not First FSM : Send cancel1 message to " << dicoplus_types::orientation2string((dicoplus_types::t_orientation)l_orientation) << std::endl ;
			  print_internal_state();
			}
#endif // DEBUG_DICOPLUS_CELL
		      m_local_output_ports[l_orientation]->cancel();
		    }
                }
            }
          // POTENTIAL NOT FIRST actions
          // Search for potential predecessors to validate/invalidate them in function of last match result
          for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
            {
              if(m_local_link_states[l_orientation].is_potential_predecessor())
                {
                  if(m_last_match)
                    {
                      m_local_link_states[l_orientation].set_matched_predecessor();
                      
                      // Others link become potential successors
                      for(unsigned int l_orientation2 = dicoplus_types::NORTH; l_orientation2 < dicoplus_types::WEST + 1; ++l_orientation2)
                        {
                          if(l_orientation2 != l_orientation)
                            {
                              m_local_link_states[l_orientation2].set_potential_successor_of((dicoplus_types::t_orientation)l_orientation);
                            }
                        }
                    }
                  else
                    {
                      // Invalid previous links
                      m_local_link_states[l_orientation].cancel_potential_predecessor(); 
                    }
                }
            }
        case UNKNOWN_NOT_FIRST:
          {
            for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
              {
                if(VALID == m_local_messages[l_orientation] && !m_local_link_states[l_orientation].is_potential_predecessor())
                  {
		    bool l_successor = false;
		    for(unsigned int l_orientation2 = dicoplus_types::NORTH; l_orientation2 < dicoplus_types::WEST + 1; ++l_orientation2)
		      {
			l_successor |=  m_local_link_states[l_orientation].is_successor_of((dicoplus_types::t_orientation)l_orientation2);
		      }
		    if(!l_successor) m_local_link_states[l_orientation].set_potential_predecessor();  
                  }
	      }
          }
          break;
        default:
          throw quicky_exception::quicky_logic_exception(std::string(name())+" : Unhandled not_first candidate state to receive a local message : \""+state_not_first_candidate2string(m_state_not_first_candidate)+"\" in cell \""+name()+"\"",__LINE__,__FILE__);
          break;
          
        }
      // Determine state of FSM by looking at link states
      bool l_attached = false;
      bool l_matched = false;
      bool l_potential = false;
      for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
	{
	  l_matched |= m_local_link_states[l_orientation].is_matched_predecessor();
	  l_potential |=  m_local_link_states[l_orientation].is_potential_predecessor(); 
          for(unsigned int l_orientation2 = dicoplus_types::NORTH; l_orientation2 < dicoplus_types::WEST + 1; ++l_orientation2)
	    {
	      if(l_orientation2 != l_orientation)
		{
		  l_attached |= m_local_link_states[l_orientation2].is_predecessor();
		}
	    }
	}
      // Set state of FSM dedicated to not_first letter candidate
      if(l_attached)
	{
	  if(ATTACHED_NOT_FIRST != m_state_not_first_candidate) set_state_not_first_FSM(ATTACHED_NOT_FIRST);
	}
      else if(l_matched)
	{
	  if(MATCHED_NOT_FIRST != m_state_not_first_candidate) set_state_not_first_FSM(MATCHED_NOT_FIRST);
	}
      else if(l_potential)
	{
	  if(POTENTIAL_NOT_FIRST != m_state_not_first_candidate) set_state_not_first_FSM(POTENTIAL_NOT_FIRST);
	}
      else
	{
	  set_state_not_first_FSM(UNKNOWN_NOT_FIRST);
	}
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::first_treat_local_message(const t_local_message (&p_messages)[4],
                                                  const unsigned int & p_nb_valid)
    {
      // To be removed
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::not_first_treat_local_message(const t_local_message (&p_messages)[4],
                                                      const unsigned int & p_nb_valid)
    {
      // To be removed
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::first_treat_cancel_message(const t_local_message (&p_messages)[4])
    {
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
	{
	  std::cout << name() << " : First FSM : Treat cancel message @ " << sc_time_stamp() << std::endl ;
	  print_internal_state();
	}
#endif // DEBUG_DICOPLUS_CELL

      switch(m_state_first_candidate)
        {
        case FIRST:
          {
            unsigned int l_remaining_valid_links = 0;
            for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
              {
                if(m_local_link_states[l_orientation].is_first_predecessor())
                  {
                    if(CANCEL == p_messages[l_orientation])
                      {
                        m_local_link_states[l_orientation].cancel_first_predecessor();
                      }
                    else
                      {
                        ++l_remaining_valid_links;
                      }
                  }
              }
            if(!l_remaining_valid_links)
              {
                set_state_first_FSM(UNKNOWN_FIRST,false);
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
#ifdef DEBUG_DICOPLUS_CELL
      if(m_gdb_debug)
	{
	  std::cout << name() << " : Not First FSM : Treat cancel message @ " << sc_time_stamp() << std::endl ;
	  print_internal_state();
	}
#endif // DEBUG_DICOPLUS_CELL
      switch(m_state_not_first_candidate)
        {
	case POTENTIAL_NOT_FIRST:
	case MATCHED_NOT_FIRST:
	  // Nothing to do
	  break;
        case ATTACHED_NOT_FIRST:
          {
            unsigned int l_remaining_valid_links = 0;
	    bool l_potential = false;
	    bool l_matched = false;
            // Seach the cancel message
            for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
              {
		l_potential |= m_local_link_states[l_orientation].is_potential_predecessor();
		l_matched |= m_local_link_states[l_orientation].is_matched_predecessor();
                // Search the confirmed successors to this direction
                for(unsigned int l_orientation2 = dicoplus_types::NORTH; l_orientation2 < dicoplus_types::WEST + 1; ++l_orientation2)
                  {
                    if(m_local_link_states[l_orientation].is_successor_of((dicoplus_types::t_orientation)l_orientation2))
                      {
                        if(CANCEL == p_messages[l_orientation])
                          {
			    m_local_link_states[l_orientation].cancel_successor_of((dicoplus_types::t_orientation)l_orientation2);
                            
                            // Check if this port has other confirmed successors
                            bool l_still_valid = false;
                            // Search the confirmed successors to this direction
                            for(unsigned int l_orientation3 = dicoplus_types::NORTH; l_orientation3 < dicoplus_types::WEST + 1; ++l_orientation3)
                              {
                                if(m_local_link_states[l_orientation3].is_successor_of((dicoplus_types::t_orientation)l_orientation2))
                                  {
                                    l_still_valid = true;
                                  }
                              }
                            if(!l_still_valid)
                              {
                                m_local_link_states[l_orientation2].cancel_predecessor();
#ifdef DEBUG_DICOPLUS_CELL
                                if(m_gdb_debug)
                                  {
                                    std::cout << name() << " : Not First FSM : Send cancel2 message to " << dicoplus_types::orientation2string((dicoplus_types::t_orientation)l_orientation2) << std::endl ;
                                    print_internal_state();
                                  }
#endif // DEBUG_DICOPLUS_CELL
                                m_local_output_ports[l_orientation2]->cancel();
                              }
			  }
			else
			  {
			    ++l_remaining_valid_links;
			  }
		      }
		  }
                
              }
            if(!l_remaining_valid_links)
              {
		if(l_matched)
		  {
		    set_state_not_first_FSM(MATCHED_NOT_FIRST);
		  }
		else if(l_potential)
		  {
		    set_state_not_first_FSM(POTENTIAL_NOT_FIRST);
		  }
		else
		  {
		    set_state_not_first_FSM(UNKNOWN_NOT_FIRST);
		  }
              }
          }
                      
          break;
        case UNKNOWN_NOT_FIRST:
	  // Nothing to do 
          break;
        default:
          throw quicky_exception::quicky_logic_exception(std::string(name())+" : Unhandled not_first candidate state to receive a cancel message : \""+state_not_first_candidate2string(m_state_not_first_candidate)+"\" in cell \""+name()+"\"",__LINE__,__FILE__);
          break;
        }
    }


    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_local_input_port(const dicoplus_types::t_orientation & p_orientation,
					      dicoplus_local_bus & p_bus)
    {
      (*m_local_input_ports[p_orientation])(p_bus);
    }

    //----------------------------------------------------------------------------
    void dicoplus_cell::bind_local_output_port(const dicoplus_types::t_orientation & p_orientation,
					       dicoplus_local_bus & p_bus)
    {
      (*m_local_output_ports[p_orientation])(p_bus);      
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
            delete m_local_output_ports[l_orientation];
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
          case MATCHED_NOT_FIRST:
            return "MATCHED_NOT_FIRST";
            break;
          case ATTACHED_NOT_FIRST:
            return "ATTACHED_NOT_FIRST";
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
	  case COMMON_FIRST_NOT_FIRST:
	    return "COMMON_FIRST_NOT_FIRST";
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
    void dicoplus_cell::init(void)
    {
      if(!m_representations.size())
        {
          add_representation(COMMON_INITIALIZED,UNKNOWN_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::INITIALIZED);
          add_representation(COMMON_READY2START,UNKNOWN_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::READY2START);
          add_representation(COMMON_NOT_FIRST,UNKNOWN_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::NOT_FIRST);
          add_representation(COMMON_FIRST_NOT_FIRST,UNKNOWN_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::FIRST_NOT_FIRST);
          add_representation(COMMON_FIRST_NOT_FIRST,POTENTIAL_FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::POTENTIAL_FIRST);
          add_representation(COMMON_FIRST_NOT_FIRST,FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::FIRST);
          add_representation(COMMON_FIRST_NOT_FIRST,FIRST,POTENTIAL_NOT_FIRST,dicoplus_types::FIRST);
          add_representation(COMMON_NOT_FIRST,UNKNOWN_FIRST,POTENTIAL_NOT_FIRST,dicoplus_types::POTENTIAL_NOT_FIRST);
          add_representation(COMMON_NOT_FIRST,UNKNOWN_FIRST,MATCHED_NOT_FIRST,dicoplus_types::MATCHED_NOT_FIRST);
          add_representation(COMMON_NOT_FIRST,UNKNOWN_FIRST,ATTACHED_NOT_FIRST,dicoplus_types::ATTACHED_NOT_FIRST);
          add_representation(COMMON_CONFIRMED,FIRST,UNKNOWN_NOT_FIRST,dicoplus_types::CONFIRMED_FIRST);
          add_representation(COMMON_CONFIRMED,UNKNOWN_FIRST,ATTACHED_NOT_FIRST,dicoplus_types::CONFIRMED_MIDDLE);
          add_representation(COMMON_CONFIRMED,UNKNOWN_FIRST,MATCHED_NOT_FIRST,dicoplus_types::CONFIRMED_LAST);

	  add_representation(COMMON_FIRST_NOT_FIRST,FIRST,MATCHED_NOT_FIRST,dicoplus_types::FIRST);
          add_representation(COMMON_FIRST_NOT_FIRST,POTENTIAL_FIRST,POTENTIAL_NOT_FIRST,dicoplus_types::POTENTIAL_FIRST);

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
    void dicoplus_cell::send_data(bool p_data)
    {
      for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
	{
	  m_local_output_ports[l_orientation]->send_data(p_data);
	}
    }

    //--------------------------------------------------------------------------
    void dicoplus_cell::no_data(void)
    {
      for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
	{
	  m_local_output_ports[l_orientation]->no_data();
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
