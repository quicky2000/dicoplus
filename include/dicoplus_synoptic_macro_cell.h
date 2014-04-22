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
#ifndef _DICOPLUS_SYNOPTIC_MACRO_CELL_H_
#define _DICOPLUS_SYNOPTIC_MACRO_CELL_H_

#include "synoptic.h"
#include "zone_container.h"
#include "dicoplus_synoptic_cell.h"
#include "dicoplus_synoptic_global_bus.h"
#include "dicoplus_synoptic_V_bus.h"
#include "dicoplus_synoptic_H_bus.h"
#include "dicoplus_local_bus_listener_if.h"

namespace dicoplus
{
  class dicoplus_synoptic_macro_cell: public synoptic::zone_container
  {
  public:
    inline dicoplus_synoptic_macro_cell(synoptic::synoptic &,
					const std::string & p_name,
					bool p_V_bus
					);
    inline dicoplus_synoptic_cell & get_cell(void);
    inline dicoplus_synoptic_global_bus & get_global_bus(void);

    inline dicoplus_local_bus_listener_if & get_north_listener(void);
    inline dicoplus_local_bus_listener_if & get_east_listener(void);
    inline dicoplus_local_bus_listener_if & get_south_listener(void);
    inline dicoplus_local_bus_listener_if & get_west_listener(void);

    inline static void init(void);
    inline static const uint32_t & get_width(void);
    inline static const uint32_t & get_height(bool p_V_bus);
    inline ~dicoplus_synoptic_macro_cell(void);
  private:
    dicoplus_synoptic_global_bus m_global_bus;
    dicoplus_synoptic_cell m_cell;
    dicoplus_synoptic_H_bus m_H_bus;
    dicoplus_synoptic_V_bus * m_V_bus;
    static uint32_t m_fixed_width;
    static uint32_t m_fixed_height;
    static uint32_t m_fixed_big_height;
  };

  //----------------------------------------------------------------------------
  dicoplus_synoptic_macro_cell::dicoplus_synoptic_macro_cell(synoptic::synoptic & p_owner,
							     const std::string & p_name,
							     bool p_V_bus
							     ):
    synoptic::zone_container(p_name,m_fixed_width,(p_V_bus ? m_fixed_big_height : m_fixed_height)),
    m_global_bus(p_owner,p_name+"_global_bus"),
    m_cell(p_owner,p_name+"_cell"),
    m_H_bus(p_owner,p_name+"_H_bus"),
    m_V_bus(p_V_bus ? new dicoplus_synoptic_V_bus(p_owner,p_name+"_V_bus") : NULL)
      {
	add_zone(0,0,m_global_bus);
	add_zone(dicoplus_synoptic_H_bus::get_width(),0,m_cell);
	add_zone(0,dicoplus_synoptic_global_bus::get_height(),m_H_bus);
        m_H_bus.set_color_code(dicoplus_synoptic_H_bus::RIGHTWARD,255,255,255);
        m_H_bus.set_color_code(dicoplus_synoptic_H_bus::LEFTWARD,255,255,255);
        m_H_bus.set_color_code(dicoplus_synoptic_H_bus::BACKGROUND,0,0,0);
	if(m_V_bus)
          {
            add_zone(11,dicoplus_synoptic_cell::get_height(),*m_V_bus);
            m_V_bus->set_color_code(dicoplus_synoptic_V_bus::UPWARD,255,255,255);
            m_V_bus->set_color_code(dicoplus_synoptic_V_bus::DOWNWARD,255,255,255);
            m_V_bus->set_color_code(dicoplus_synoptic_V_bus::BACKGROUND,0,0,0);
          }

        
      }

    //----------------------------------------------------------------------------
    dicoplus_synoptic_macro_cell::~dicoplus_synoptic_macro_cell(void)
      {
	delete m_V_bus;
      }

    //----------------------------------------------------------------------------
    void dicoplus_synoptic_macro_cell::init(void)
    {
      m_fixed_width = dicoplus_synoptic_H_bus::get_width() + dicoplus_synoptic_cell::get_width();
      m_fixed_height = dicoplus_synoptic_cell::get_height();
      m_fixed_big_height = m_fixed_height + dicoplus_synoptic_V_bus::get_height();
    }

    //----------------------------------------------------------------------------
    const uint32_t & dicoplus_synoptic_macro_cell::get_width(void)
      {
	return m_fixed_width;
      }

    //----------------------------------------------------------------------------
    const uint32_t & dicoplus_synoptic_macro_cell::get_height(bool p_V_bus)
      {
	return p_V_bus ? m_fixed_big_height : m_fixed_height;
      }
    //----------------------------------------------------------------------------
    dicoplus_synoptic_cell & dicoplus_synoptic_macro_cell::get_cell(void)
      {
	return m_cell;
      }

    //----------------------------------------------------------------------------
    dicoplus_synoptic_global_bus & dicoplus_synoptic_macro_cell::get_global_bus(void)
      {
	return m_global_bus;
      }

    //----------------------------------------------------------------------------
    dicoplus_local_bus_listener_if & dicoplus_synoptic_macro_cell::get_north_listener(void)
      {
        assert(m_V_bus);
        return m_V_bus->get_north_listener();
      }

    //----------------------------------------------------------------------------
    dicoplus_local_bus_listener_if & dicoplus_synoptic_macro_cell::get_east_listener(void)
      {
        return m_H_bus.get_east_listener();
      }

    //----------------------------------------------------------------------------
    dicoplus_local_bus_listener_if & dicoplus_synoptic_macro_cell::get_south_listener(void)
      {
        assert(m_V_bus);
        return m_V_bus->get_north_listener();
      }

    //----------------------------------------------------------------------------
    dicoplus_local_bus_listener_if & dicoplus_synoptic_macro_cell::get_west_listener(void)
      {
        return m_H_bus.get_west_listener();
      }

}

#endif // _DICOPLUS_SYNOPTIC_MACRO_CELL_H_
//EOF
