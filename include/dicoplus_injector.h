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
#ifndef _DICOPLUS_INJECTOR_H_
#define _DICOPLUS_INJECTOR_H_

#include "systemc.h"
#include "dicoplus_global_port_manager.h"
#include "dicoplus_global_port_binding_if.h"
#include "dicoplus_global_message_analyzer_if.h"
#include <queue>

namespace dicoplus
{
  class dicoplus_injector:public sc_module, public dicoplus_global_port_binding_if,public dicoplus_global_message_analyzer_if
  {
  public:
    SC_HAS_PROCESS(dicoplus_injector);
    dicoplus_injector(sc_module_name name);
    
    // Methods inherited from dicoplus_global_port_binding_if
    inline void bind_input_port(dicoplus_global_bus & p_bus);
    inline void bind_output_port(dicoplus_global_bus & p_bus);
    // End of methods inherited from dicoplus_global_port_binding_if

    // Methods inherited from dicoplus_global_message_analyzer_if
    void treat(const dicoplus_global_message_char & p_message);
    void treat(const dicoplus_global_message_separator & p_message);
    // End of methods inherited from dicoplus_global_message_analyzer_if

    inline void set_grid_content(std::queue<dicoplus_types::t_global_data_type> & p_content);

    inline ~dicoplus_injector(void);

    sc_in<bool> m_clk;
    sc_in<bool> m_global_req;
    sc_in<bool> m_global_ack;
  private:
    void run(void);
    inline void listen_char_propagation(void);

    dicoplus_global_port_manager m_global_port_manager;
    std::queue<dicoplus_types::t_global_data_type> * m_grid_content;
    bool m_ready2send_new_char;
  };

  //----------------------------------------------------------------------------
  void dicoplus_injector::set_grid_content(std::queue<dicoplus_types::t_global_data_type> & p_content)
  {
    m_grid_content = & p_content;
  }

  //----------------------------------------------------------------------------
  dicoplus_injector::~dicoplus_injector(void)
    {
      delete m_grid_content;
    }
  
  //----------------------------------------------------------------------------
  dicoplus_injector::dicoplus_injector(sc_module_name name):
    sc_module(name),
    m_clk("clk"),
    m_global_req("global_req"),
    m_global_ack("global_ack"),
    m_global_port_manager("global_port_manager",*this),
    m_grid_content(NULL),
    m_ready2send_new_char(false)
      {
	m_global_port_manager.m_clk(m_clk);

	SC_THREAD(run);
	sensitive << m_clk.pos();	

	SC_METHOD(listen_char_propagation);
	sensitive << m_clk.pos();
      }


    //----------------------------------------------------------------------------
    void dicoplus_injector::listen_char_propagation(void)
    {
      if(m_global_req.read() && m_global_ack.read())
	{
	  std::cout << name() << " : Ready to send a new char" << std::endl ;
	  m_ready2send_new_char = true;
	}
    }

    //----------------------------------------------------------------------------
    void dicoplus_injector::bind_input_port(dicoplus_global_bus & p_bus)
    {
      m_global_port_manager.bind_input_port(p_bus);
    }

    //----------------------------------------------------------------------------
    void dicoplus_injector::bind_output_port(dicoplus_global_bus & p_bus)
    {
      m_global_port_manager.bind_output_port(p_bus);
    }

    //----------------------------------------------------------------------------
    void dicoplus_injector::run(void)
    {
      std::cout << "Injector starting !" << std::endl ;
      if(NULL == m_grid_content) throw quicky_exception::quicky_logic_exception("Grid content has not been loaded",__LINE__,__FILE__);
      while(m_grid_content->size())
	{
	  if(m_global_port_manager.output_box_empty())
	    {
	      m_global_port_manager.post_message(*new dicoplus_global_message_char(m_grid_content->front()));
	      m_grid_content->pop();
	    }
	  wait();
	}

#if 0
      if(m_global_port_manager.output_box_empty())
	{
	  m_global_port_manager.post_message(*new dicoplus_global_message_char(1));
	}
      wait();
      if(m_global_port_manager.output_box_empty())
	{
	  m_global_port_manager.post_message(*new dicoplus_global_message_char(2));
	}
      wait();
      if(m_global_port_manager.output_box_empty())
	{
	  m_global_port_manager.post_message(*new dicoplus_global_message_char(3));
	}
      wait();
      if(m_global_port_manager.output_box_empty())
	{
	  m_global_port_manager.post_message(*new dicoplus_global_message_char(4));
	}
#endif
      for(uint l_index = 0 ; l_index < 20 ; ++l_index)
	{
	  wait();
	}
      sc_stop();
    }
    
    //----------------------------------------------------------------------------
    void dicoplus_injector::treat(const dicoplus_global_message_char & p_message)
    {
      std::cout << name() << " : Treat char message @ " << sc_time_stamp() << std::endl ;
    }

    //----------------------------------------------------------------------------
    void dicoplus_injector::treat(const dicoplus_global_message_separator & p_message)
    {
      std::cout << name() << " : Treat separator message @ " << sc_time_stamp() << std::endl ;
    }

}
#endif // _DICOPLUS_INJECTOR_H_
//EOF
