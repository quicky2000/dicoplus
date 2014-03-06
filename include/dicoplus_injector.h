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

namespace dicoplus
{
  class dicoplus_injector:public sc_module, public dicoplus_global_port_binding_if
  {
  public:
    SC_HAS_PROCESS(dicoplus_injector);
    dicoplus_injector(sc_module_name name);
    
    // Methods inherited from dicoplus_global_port_binding_if
    inline void bind_input_port(dicoplus_global_bus & p_bus);
    inline void bind_output_port(dicoplus_global_bus & p_bus);
    // End of methods inherited from dicoplus_global_port_binding_if

    sc_in<bool> m_clk;
  private:
    void run(void);

    dicoplus_global_port_manager m_global_port_manager;
  };

  //----------------------------------------------------------------------------
  dicoplus_injector::dicoplus_injector(sc_module_name name):
    sc_module(name),
    m_clk("clk"),
    m_global_port_manager("global_port_manager")
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
    void dicoplus_injector::run(void)
    {
      std::cout << "Injector starting !" << std::endl ;		       
	  m_global_port_manager.post_message(*new dicoplus_global_message_char(1));
	  wait();
	  m_global_port_manager.post_message(*new dicoplus_global_message_char(2));
	  wait();
	  m_global_port_manager.post_message(*new dicoplus_global_message_char(3));
	  wait();
	  m_global_port_manager.post_message(*new dicoplus_global_message_char(4));
	  wait();
	  wait();
	  wait();
	  sc_stop();
    }
  
}
#endif // _DICOPLUS_INJECTOR_H_
//EOF
