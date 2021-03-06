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
#ifndef DICOPLUS_MACRO_CELL_H
#define DICOPLUS_MACRO_CELL_H

#include "dicoplus_cell.h"
#include "dicoplus_global_bus.h"
#include "dicoplus_local_bus.h"
#include "dicoplus_global_bus_probe.h"
#include "dicoplus_local_bus_probe.h"
#include "dicoplus_global_port_binding_if.h"

namespace dicoplus
{
  class dicoplus_macro_cell: public dicoplus_global_port_binding_if
  {
  public:
    inline dicoplus_macro_cell(const std::string & p_name,
			       sc_signal<bool> & p_sig, 
                               const unsigned int & p_nb_neighbour);
    inline void bind(dicoplus_macro_cell & p_macro_cell);
    inline dicoplus_global_bus & get_global_bus(void)const;
    inline void attach_cell_listener(cell_listener_if & p_listener);
    inline void attach_global_bus_listener(dicoplus_global_bus_probe & p_probe);
    inline bool is_attached_local_bus_listener_probe(void)const;
    inline void attach_local_bus_listener_probe(const dicoplus_types::t_orientation & p_orientation,
						dicoplus_local_bus_probe & p_probe);
    inline ~dicoplus_macro_cell(void);
    inline const std::string & get_name(void)const;

    inline void bind_local_input_port(const dicoplus_types::t_orientation & p_orientation,
				      dicoplus_local_bus & p_bus);
    inline dicoplus_local_bus & get_output_bus(const dicoplus_types::t_orientation & p_orientation);

    // Methods inherited from dicoplus_global_port_binding_if
    inline void bind_input_port(dicoplus_global_bus & p_bus);
    inline void bind_output_port(dicoplus_global_bus & p_bus);
    // End of methods inherited from dicoplus_global_port_binding_if

  private:
    std::string m_name;
    dicoplus_cell m_cell;
    dicoplus_global_bus * m_global_bus;
    dicoplus_global_bus_probe * m_global_probe;
    dicoplus_local_bus * m_local_output_buses[4];
    dicoplus_local_bus_probe * m_local_probes[4];
  };

  //----------------------------------------------------------------------------
  dicoplus_macro_cell::dicoplus_macro_cell(const std::string & p_name,
					   sc_signal<bool> & p_sig, 
                                           const unsigned int & p_nb_neighbour):
    m_name(p_name),
    m_cell(sc_module_name(("Cell_"+p_name).c_str()),p_nb_neighbour),
    m_global_bus(NULL),
    m_global_probe(NULL)
      {
        for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
          {
	    m_local_output_buses[l_orientation] = new dicoplus_local_bus("Cell_"+p_name+"_"+dicoplus_types::orientation2string((dicoplus_types::t_orientation)l_orientation)+"_output");
	    m_cell.bind_local_output_port((dicoplus_types::t_orientation)l_orientation,*m_local_output_buses[l_orientation]);
	    m_local_probes[l_orientation] = NULL;
	  }
	m_cell.m_clk(p_sig);
      }

    //----------------------------------------------------------------------------
    void dicoplus_macro_cell::bind_local_input_port(const dicoplus_types::t_orientation & p_orientation,
						    dicoplus_local_bus & p_bus)
    {
      m_cell.bind_local_input_port(p_orientation,p_bus);
    }

    //----------------------------------------------------------------------------
    dicoplus_local_bus & dicoplus_macro_cell::get_output_bus(const dicoplus_types::t_orientation & p_orientation)
      {
	return *m_local_output_buses[p_orientation];
      }

    //----------------------------------------------------------------------------
    const std::string & dicoplus_macro_cell::get_name(void)const
      {
	return m_name;
      }

    //----------------------------------------------------------------------------
    void dicoplus_macro_cell::bind(dicoplus_macro_cell & p_macro_cell)
    {
      p_macro_cell.m_cell.bind_output_port(*m_global_bus);
    }
    //----------------------------------------------------------------------------
    dicoplus_global_bus & dicoplus_macro_cell::get_global_bus(void)const
      {
	if(!m_global_bus) throw quicky_exception::quicky_logic_exception("Macro_Cell \""+m_name+"\" global bus has not been initialised",__LINE__,__FILE__);
	return *m_global_bus;
      }

    //----------------------------------------------------------------------------
    void dicoplus_macro_cell::attach_cell_listener(cell_listener_if & p_listener)
    {
      m_cell.set_listener(p_listener);
    }

    //----------------------------------------------------------------------------
    void dicoplus_macro_cell::attach_global_bus_listener(dicoplus_global_bus_probe & p_probe)
    {
      if(!m_global_bus) throw quicky_exception::quicky_logic_exception("Macro_Cell \""+m_name+"\" global bus has not been initialised",__LINE__,__FILE__);
      p_probe.operator ()(*m_global_bus);
      m_global_probe = & p_probe;
    }

    //----------------------------------------------------------------------------
    void dicoplus_macro_cell::attach_local_bus_listener_probe(const dicoplus_types::t_orientation & p_orientation,
							      dicoplus_local_bus_probe & p_probe)
    {
      m_local_probes[p_orientation] = & p_probe;
      p_probe.operator ()(*m_local_output_buses[p_orientation]);
    }

    //----------------------------------------------------------------------------
    void dicoplus_macro_cell::bind_input_port(dicoplus_global_bus & p_bus)
    {
      m_global_bus = &p_bus;
      m_cell.bind_input_port(p_bus);
    }

    //----------------------------------------------------------------------------
    void dicoplus_macro_cell::bind_output_port(dicoplus_global_bus & p_bus)
    {
      m_cell.bind_output_port(p_bus);
    }

    //----------------------------------------------------------------------------
    dicoplus_macro_cell::~dicoplus_macro_cell(void)
      {
        for(unsigned int l_orientation = dicoplus_types::NORTH; l_orientation < dicoplus_types::WEST + 1; ++l_orientation)
          {
	    delete m_local_probes[l_orientation];
	  }
	delete m_global_probe;
	delete m_global_bus;
      }

}
#endif // DICOPLUS_MACRO_CELL_H
//EOF
