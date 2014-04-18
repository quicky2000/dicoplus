/*    This file is part of dicoplus
      The aim of this software is to solve dicoplus game
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
#ifndef _DICOPLUS_LOCAL_BUS_PROBE_H_
#define _DICOPLUS_LOCAL_BUS_PROBE_H_

#include "dicoplus_local_bus.h"
#include "dicoplus_local_bus_listener_if.h"
#include "systemc.h"
#include <vector>

namespace dicoplus
{
  class dicoplus_local_bus_probe: public sc_module
  {
  public:
    SC_HAS_PROCESS(dicoplus_local_bus_probe);
    inline dicoplus_local_bus_probe(sc_module_name p_name,
				    dicoplus_local_bus_listener_if & p_listener);
    inline void operator () (const dicoplus_local_bus  & p_bus);
    inline void attach_listener(dicoplus_local_bus_listener_if & p_listener);
    sc_in<bool> m_clock;
  private:
    void run(void);

    sc_in<bool> m_valid;
    sc_in<bool> m_data;
    bool m_previous_valid;
    std::vector<dicoplus_local_bus_listener_if *> m_listeners;
  };

  //------------------------------------------------------------
  dicoplus_local_bus_probe::dicoplus_local_bus_probe(sc_module_name p_name,
						       dicoplus_local_bus_listener_if & p_listener):
    sc_module(p_name),
    m_clock("clock"),
    m_valid("valid"),
    m_data("data"),
    m_previous_valid(false)
    {
      SC_METHOD(run);
      dont_initialize();
      sensitive << m_clock.pos();

      m_listeners.push_back(&p_listener);
    }

    //------------------------------------------------------------
    void dicoplus_local_bus_probe::operator () (const dicoplus_local_bus  & p_bus)
    {
      m_valid(p_bus.m_valid);
      m_data(p_bus.m_data);    
    }

    //------------------------------------------------------------
    void dicoplus_local_bus_probe::attach_listener(dicoplus_local_bus_listener_if & p_listener)
    {
      m_listeners.push_back(&p_listener);
    }

    //------------------------------------------------------------
    void dicoplus_local_bus_probe::run(void)
    {

      if(m_valid.read() != m_previous_valid)
        {
          m_previous_valid = m_valid.read();
          for(std::vector<dicoplus_local_bus_listener_if*>::iterator l_iter = m_listeners.begin();
              m_listeners.end() != l_iter;
              ++l_iter)
            {
              if(!m_valid.read())
                {
                  (*l_iter)->no_activity();
                }
              else
                {
                  (*l_iter)->data(m_data.read());
                }
            }
        }

    }

}
#endif // _DICOPLUS_LOCAL_BUS_PROBE_H_
//EOF
