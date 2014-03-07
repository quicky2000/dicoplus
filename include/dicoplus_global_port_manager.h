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
#include "dicoplus_global_message_box.h"
#include "dicoplus_global_message_factory.h"
#include "dicoplus_global_message_analyzer_if.h"

namespace dicoplus
{
  class dicoplus_global_port_manager:sc_module
  {
  public:
    SC_HAS_PROCESS(dicoplus_global_port_manager);
    inline dicoplus_global_port_manager(sc_module_name p_name,
                                        dicoplus_global_message_analyzer_if & p_listener);
    inline void bind_input_port(dicoplus_global_bus & p_bus);
    inline void bind_output_port(dicoplus_global_bus & p_bus);

    inline void post_message(const dicoplus_global_message_base & p_msg);
    inline bool output_box_empty(void)const;
    inline bool message_received(void)const;
    inline const dicoplus_global_message_base & get_message(void);

    sc_in<bool> m_clk;
  private:

    inline void listen(void);
    inline void manage_output(void);
    dicoplus_global_input_port m_global_input_port;
    dicoplus_global_output_port m_global_output_port;
    dicoplus_global_message_box m_input_box;
    dicoplus_global_message_box m_output_box;
    dicoplus_global_message_analyzer_if & m_listener;
  };

  //----------------------------------------------------------------------------
  dicoplus_global_port_manager::dicoplus_global_port_manager(sc_module_name p_name,
                                        dicoplus_global_message_analyzer_if & p_listener):
    sc_module(p_name),
    m_clk("clk"),
    m_global_input_port("gi_port"),
    m_global_output_port("go_port"),
    m_listener(p_listener)
   {
#ifdef DEBUG_GLOBAL_PORT_MANAGER_COMMUNICATION
     if(std::string(name())=="top.dicoplus.Cell_0_0.global_port_manager")
       {
#endif
         SC_METHOD(listen);
         sensitive << m_clk.pos();
#ifdef DEBUG_GLOBAL_PORT_MANAGER_COMMUNICATION
       }

     if(std::string(name())=="top.dicoplus.injector.global_port_manager")
       {
#endif
         SC_THREAD(manage_output);
         sensitive << m_clk.pos();
#ifdef DEBUG_GLOBAL_PORT_MANAGER_COMMUNICATION
       }
#endif
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

    //----------------------------------------------------------------------------
    void dicoplus_global_port_manager::listen(void)
    { 
#ifdef DEBUG_GLOBAL_PORT_MANAGER_COMMUNICATION
      std::cout << name() << " : " ;;
      std::cout << "listen input port @ " << sc_time_stamp() << std::endl ;
#endif
      if(m_input_box.is_empty())
        {
          m_global_input_port.ack(true);
          if(m_global_input_port.req()==true)
            {
#ifdef DEBUG_GLOBAL_PORT_MANAGER_COMMUNICATION
              std::cout << name() << " : " ;;
              std::cout << "Receive message" << std::endl ;
              std::cout << "Data : " << m_global_input_port.data() << std::endl;
#endif
              const dicoplus_global_message_base & l_message = *dicoplus_global_message_factory::decode_message(m_global_input_port.cmd(),
                                                                                                                m_global_input_port.data());
              m_input_box.set_message(l_message);
              m_input_box.get_message().be_treated(m_listener);
            }
        }
      else
        {
          m_global_input_port.ack(false);
        }
    }

    //----------------------------------------------------------------------------
    void dicoplus_global_port_manager::manage_output(void)
    {
	while(1)
	  {
#ifdef DEBUG_GLOBAL_PORT_MANAGER_COMMUNICATION
	    std::cout << name() << " : " ;;
            std::cout << "Manage output port @ " << sc_time_stamp() << std::endl ;
#endif
	    if(m_output_box.is_empty())
	      {
		m_global_output_port.req(false);
		wait();
	      }
	    else
	      {
#ifdef DEBUG_GLOBAL_PORT_MANAGER_COMMUNICATION
		std::cout << name() << " : " ;;
		std::cout << "Send message" << std::endl ;            
#endif
		const dicoplus_global_message_base & l_message = m_output_box.get_message();
		// Write message content on port
		m_global_output_port.cmd(l_message.get_cmd());
		m_global_output_port.data(l_message.get_data());
		delete(&l_message);

		// Indicate that message is ready to be sent
		m_global_output_port.req(true);

		// waiting for acknowledge
		do
		  {
		    wait();
		  }
		while(m_global_output_port.ack()==false);
#ifdef DEBUG_GLOBAL_PORT_MANAGER_COMMUNICATION
		std::cout << name() << " : " ;;
		std::cout << "Acknowledge received" << std::endl ;            
#endif

	      }
	  }
    }

    //--------------------------------------------------------------------------
    void dicoplus_global_port_manager::post_message(const dicoplus_global_message_base & p_msg)
    {
      if(!m_output_box.is_empty())
	{
	  throw quicky_exception::quicky_logic_exception(std::string(name())+" : try to post a message wherease output box is not empty",__LINE__,__FILE__);
	}
      m_output_box.set_message(p_msg);
    }

    //--------------------------------------------------------------------------
    bool dicoplus_global_port_manager::output_box_empty(void)const
    {
      return m_output_box.is_empty();
    }

    //--------------------------------------------------------------------------
    bool dicoplus_global_port_manager::message_received(void)const
    {
      return !m_input_box.is_empty();
    }

    //--------------------------------------------------------------------------
    const dicoplus_global_message_base & dicoplus_global_port_manager::get_message(void)
      {
	return m_input_box.get_message();
      }

}
#endif // _DICOPLUS_GLOBAL_PORT_MANAGER_H_
//EOF
