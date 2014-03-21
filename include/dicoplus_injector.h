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
#include "dicoplus_global_bus_listener_if.h"
#include "dicoplus_global_bus_probe.h"
#include "dicoplus_char.h"
#include <queue>
#include <vector>

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
    inline void treat(const dicoplus_global_message_char & p_message);
    inline void treat(const dicoplus_global_message_separator & p_message);
    // End of methods inherited from dicoplus_global_message_analyzer_if

    inline void set_grid_content(std::queue<dicoplus_types::t_global_data_type> & p_content);

    inline void spy_bus(const sc_signal<bool> & p_clk_sig,
			dicoplus_global_bus  & p_bus);

    inline ~dicoplus_injector(void);

    sc_in<bool> m_clk;
  private:

    class spy_port_listener:public dicoplus_global_bus_listener_if
    {
    public:
      inline spy_port_listener(dicoplus_injector & p_injector);
      // Methods inherited from dicoplus_global_bus_listener_if
      inline void treat(const dicoplus_global_message_char & p_message);
      inline void treat(const dicoplus_global_message_separator & p_message);
      inline void no_activity(void);
      // End of methods inherited from dicoplus_global_bus_listener_if
    private:
      dicoplus_injector & m_injector;
      bool m_wait_first_separator;
    };

    void run(void);

    dicoplus_global_port_manager m_global_port_manager;
    spy_port_listener m_spy_listener;
    dicoplus_global_bus_probe m_spy_probe;
    std::queue<dicoplus_types::t_global_data_type> * m_grid_content;
    bool m_ready2send_new_char;
  };

  //----------------------------------------------------------------------------
  dicoplus_injector::spy_port_listener::spy_port_listener(dicoplus_injector & p_injector):
    m_injector(p_injector),
    m_wait_first_separator(true)
    {
    }

  //----------------------------------------------------------------------------
  void dicoplus_injector::spy_port_listener::treat(const dicoplus_global_message_char & p_message)
  {
    if(!m_wait_first_separator) m_injector.m_ready2send_new_char = true;
  }

  //----------------------------------------------------------------------------
  void dicoplus_injector::spy_port_listener::treat(const dicoplus_global_message_separator & p_message)
  {
    m_injector.m_ready2send_new_char = true;
    m_wait_first_separator = false;
  }

  //----------------------------------------------------------------------------
  void dicoplus_injector::spy_port_listener::no_activity(void)
  {
  }

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
    m_global_port_manager("global_port_manager",*this),
    m_spy_listener(*this),
    m_spy_probe("spy_probe",m_spy_listener),
    m_grid_content(NULL),
    m_ready2send_new_char(false)
      {
	m_global_port_manager.m_clk(m_clk);

	SC_THREAD(run);
	sensitive << m_clk.pos();	

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
    void dicoplus_injector::spy_bus(const sc_signal<bool> & p_clk_sig,
				    dicoplus_global_bus  & p_bus)
    {
      m_spy_probe.m_clock(p_clk_sig);
      m_spy_probe(p_bus);
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

      while(!m_global_port_manager.output_box_empty())
	{
	  wait();
	}
      m_global_port_manager.post_message(*new dicoplus_global_message_separator());

      while(!m_global_port_manager.output_box_empty())
	{
	  wait();
	}

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
