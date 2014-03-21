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
#ifndef _DICOPLUS_GLOBAL_BUS_PROBE_H_
#define _DICOPLUS_GLOBAL_BUS_PROBE_H_

#include "dicoplus_types.h"
#include "dicoplus_global_bus.h"
#include "dicoplus_global_message_factory.h"
#include "dicoplus_global_bus_listener_if.h"
#include "dicoplus_char.h"
#include "systemc.h"

namespace dicoplus
{
  class dicoplus_global_bus_probe: public sc_module
  {
  public:
    SC_HAS_PROCESS(dicoplus_global_bus_probe);
    inline dicoplus_global_bus_probe(sc_module_name p_name,
				     dicoplus_global_bus_listener_if & p_listener);
    inline void operator () (const dicoplus_global_bus  & p_bus);
    sc_in<bool> m_clock;
  private:
    void run(void);

    sc_in<bool> m_req;
    sc_in<bool> m_ack;
    sc_in<typename dicoplus_types::t_global_cmd_type> m_cmd;
    sc_in<typename dicoplus_types::t_global_data_type> m_data;

    dicoplus_global_bus_listener_if & m_listener;
  };

  //------------------------------------------------------------
  dicoplus_global_bus_probe::dicoplus_global_bus_probe(sc_module_name p_name,
						       dicoplus_global_bus_listener_if & p_listener):
    sc_module(p_name),
    m_clock("clock"),
    m_req("req"),
    m_ack("ack"),
    m_cmd("cmd"),
    m_data("data"),
    m_listener(p_listener)
    {
      SC_METHOD(run);
      dont_initialize();
      sensitive << m_clock.pos();
    }

    //------------------------------------------------------------
    void dicoplus_global_bus_probe::operator () (const dicoplus_global_bus  & p_bus)
    {
      m_req(p_bus.m_req);
      m_ack(p_bus.m_ack);
      m_cmd(p_bus.m_cmd);
      m_data(p_bus.m_data);    
    }

    //------------------------------------------------------------
    void dicoplus_global_bus_probe::run(void)
    {

      if(!m_req.read() || !m_ack.read())
	{
	  m_listener.no_activity();
	}
      else
	{
	  const dicoplus_global_message_base & l_message = *dicoplus_global_message_factory::decode_message(m_cmd.read(),
													    m_data.read());
	  l_message.be_treated(m_listener);
	  delete & l_message;
	}

    }

}
#endif // _DICOPLUS_GLOBAL_BUS_PROBE_H_
//EOF
