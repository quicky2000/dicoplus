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
#ifndef DICOPLUS_SYNOPTIC_H_BUS_H
#define DICOPLUS_SYNOPTIC_H_BUS_H

#include "dicoplus_synoptic_local_bus.h"
#include "dicoplus_synoptic_local_bus_listener.h"
#include "quicky_exception.h"
#include <set>
#include <inttypes.h>
#include <stdlib.h>

namespace dicoplus
{
  class dicoplus_synoptic_H_bus:public dicoplus_synoptic_local_bus
  {
  public:
    inline dicoplus_synoptic_H_bus(synoptic::zone_owner_if &,
				   const std::string & p_name);
    inline dicoplus_local_bus_listener_if & get_east_listener(void);
    inline dicoplus_local_bus_listener_if & get_west_listener(void);
    inline static const uint32_t & get_width(void);
    inline static const uint32_t & get_height(void);
    inline static void init(void);
    typedef enum {BACKGROUND=0,LEFTWARD,RIGHTWARD}t_color_code;
  private:
    // Pure virtual method inherited from dicoplus_synoptic_local_bus
    inline const std::set<std::pair<uint32_t,uint32_t> > & get_pixel_location(const unsigned int & p_code)const;
    // End of pure virtual method inherited from dicoplus_synoptic_local_bus

    static const std::string m_coded_representation;
    static std::set<std::pair<uint32_t,uint32_t> > m_pixel_locations[4];
    static const uint32_t m_width;
    static const uint32_t m_height;
    synoptic_local_bus_listener m_east_listener;
    synoptic_local_bus_listener m_west_listener;
  };

  //----------------------------------------------------------------------------
  dicoplus_synoptic_H_bus::dicoplus_synoptic_H_bus(synoptic::zone_owner_if & p_owner,
						   const std::string & p_name):
    dicoplus_synoptic_local_bus(p_owner,p_name,m_width,m_height),
    m_east_listener(RIGHTWARD,*this),
    m_west_listener(LEFTWARD,*this)
      {
      }

    //----------------------------------------------------------------------------
    dicoplus_local_bus_listener_if & dicoplus_synoptic_H_bus::get_east_listener(void)
      {
        return m_east_listener;
      }
    //----------------------------------------------------------------------------
    dicoplus_local_bus_listener_if & dicoplus_synoptic_H_bus::get_west_listener(void)
      {
        return m_west_listener;
      }

    //--------------------------------------------------------------------------
    const std::set<std::pair<uint32_t,uint32_t> > & dicoplus_synoptic_H_bus::get_pixel_location(const unsigned int & p_code)const
      {
	assert(p_code <= 3);
	return m_pixel_locations[p_code];
      }
    //--------------------------------------------------------------------------
    void dicoplus_synoptic_H_bus::init(void)
    {
      dicoplus_synoptic_local_bus::init(m_coded_representation,m_pixel_locations,m_width,m_height);
    }

    //--------------------------------------------------------------------------
    const uint32_t & dicoplus_synoptic_H_bus::get_width(void)
      {
	return m_width;
      }

    //--------------------------------------------------------------------------
    const uint32_t & dicoplus_synoptic_H_bus::get_height(void)
      {
	return m_height;
      }

}
#endif // DICOPLUS_SYNOPTIC_H_BUS_H
//EOF
