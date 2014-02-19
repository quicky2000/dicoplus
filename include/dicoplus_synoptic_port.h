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
#ifndef _DICOPLUS_SYNOPTIC_PORT_H_
#define _DICOPLUS_SYNOPTIC_PORT_H_

#include "synoptic.h"
#include "color_zone.h"
#include "zone_container.h"

#include <map>
#include <inttypes.h>
#include <string>

namespace dicoplus
{
  class dicoplus_synoptic_port: public synoptic::zone_container
  {
  public:
    typedef enum {UP,DOWN,LEFT,RIGHT} t_orientation;
    inline dicoplus_synoptic_port(synoptic::synoptic &,
				  const std::string & p_name,
				  const dicoplus_synoptic_port::t_orientation & p_orientation);
    inline static const uint32_t & get_short_dim(void);
    inline static const uint32_t & get_long_dim(void);
    inline virtual ~dicoplus_synoptic_port(void){}
  private:
    synoptic::color_zone m_zone1;
    synoptic::color_zone m_zone2;
    synoptic::color_zone m_zone3;
    static const uint32_t m_short_dim;
    static const uint32_t m_long_dim;
  };

  //----------------------------------------------------------------------------
  const uint32_t & dicoplus_synoptic_port::get_short_dim(void)
    {
      return m_short_dim;
    }

  //----------------------------------------------------------------------------
  const uint32_t & dicoplus_synoptic_port::get_long_dim(void)
    {
      return m_long_dim;
    }

  //----------------------------------------------------------------------------
  dicoplus_synoptic_port::dicoplus_synoptic_port(synoptic::synoptic & p_owner,
						 const std::string & p_name,
						 const dicoplus_synoptic_port::t_orientation & p_orientation):
    synoptic::zone_container(p_name,p_orientation == UP || p_orientation==DOWN ? m_long_dim: m_short_dim,p_orientation == UP || p_orientation==DOWN ? m_short_dim:m_long_dim),
    m_zone1(p_owner,p_name+"_Z1",(p_orientation == UP || p_orientation==DOWN ? 1 : m_short_dim),(p_orientation == UP || p_orientation==DOWN ? m_short_dim : 1),255,255,255),
    m_zone2(p_owner,p_name+"_Z2",(p_orientation == UP || p_orientation==DOWN ? m_long_dim - 2 : 1),(p_orientation == UP || p_orientation==DOWN ? 1 : m_long_dim - 2),255,255,255),
    m_zone3(p_owner,p_name+"_Z3",(p_orientation == UP || p_orientation==DOWN ? 1 : m_short_dim),(p_orientation == UP || p_orientation==DOWN ? m_short_dim : 1),255,255,255)
      {
	switch(p_orientation)
	  {
	  case UP:
	    add_zone(0,0,m_zone1);
	    add_zone(1,m_short_dim - 1,m_zone2);
	    add_zone(m_long_dim - 1,0,m_zone3);
	    break;
	  case DOWN:
	    add_zone(0,0,m_zone1);
	    add_zone(1,0,m_zone2);
	    add_zone(m_long_dim - 1,0,m_zone3);
	    break;
	  case LEFT:
	    add_zone(0,0,m_zone1);
	    add_zone(m_short_dim - 1,1,m_zone2);
	    add_zone(0,m_long_dim - 1,m_zone3);
	    break;
	  case RIGHT:
	    add_zone(0,0,m_zone1);
	    add_zone(0,1,m_zone2);
	    add_zone(0,m_long_dim - 1,m_zone3);
	    break;
	  }
      }
}

#endif // _DICOPLUS_SYNOPTIC_PORT_H_
//EOF
