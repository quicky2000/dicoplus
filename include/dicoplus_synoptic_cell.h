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
#ifndef _DICOPLUS_SYNOPTIC_CELL_H_
#define _DICOPLUS_SYNOPTIC_CELL_H_

#include "synoptic.h"
#include "zone_container.h"
#include "color_zone.h"
#include "dicoplus_synoptic_char.h"
#include "dicoplus_synoptic_port.h"
#include "cell_listener_if.h"
#include <inttypes.h>

namespace dicoplus
{
  class dicoplus_synoptic_cell: public synoptic::zone_container, public cell_listener_if
  {
  public:
    inline dicoplus_synoptic_cell(synoptic::synoptic &,
				  const std::string & p_name
                                  );
    inline static void compute_dim(void);
    inline static const uint32_t get_width(void);
    inline static const uint32_t get_height(void);

    // Methods inherited from cell_listener_if
    inline void set_content(const uint32_t & p_content);
    //End of methods inherited from cell_listener_if

    inline virtual ~dicoplus_synoptic_cell(void){}
  private:
    synoptic::color_zone m_up_border;
    synoptic::color_zone m_right_border;
    synoptic::color_zone m_down_border;
    synoptic::color_zone m_left_border;
    dicoplus_synoptic_char m_char_display;
    dicoplus_synoptic_port m_up_req_port;
    dicoplus_synoptic_port m_up_gnt_port;
    dicoplus_synoptic_port m_down_req_port;
    dicoplus_synoptic_port m_down_gnt_port;
    dicoplus_synoptic_port m_left_greq_port;
    dicoplus_synoptic_port m_left_ggnt_port;
    dicoplus_synoptic_port m_left_req_port;
    dicoplus_synoptic_port m_left_gnt_port;
    dicoplus_synoptic_port m_right_greq_port;
    dicoplus_synoptic_port m_right_ggnt_port;
    dicoplus_synoptic_port m_right_req_port;
    dicoplus_synoptic_port m_right_gnt_port;
      

    static bool m_computed_dim;
    static uint32_t m_width;
    static uint32_t m_height;
    static const uint32_t m_req_gnt_dim;
    static const uint32_t m_border_padding;
    static const uint32_t m_middle_dim;
  };

  //----------------------------------------------------------------------------
  void dicoplus_synoptic_cell::set_content(const uint32_t & p_content)
  {
    m_char_display.set_content(p_content);
    m_char_display.paint();
  }

  //----------------------------------------------------------------------------
  const uint32_t dicoplus_synoptic_cell::get_width(void)
  {
    return m_width;
  }

  //----------------------------------------------------------------------------
  const uint32_t dicoplus_synoptic_cell::get_height(void)
  {
    return m_height;
  }

  //----------------------------------------------------------------------------
  dicoplus_synoptic_cell::dicoplus_synoptic_cell(synoptic::synoptic & p_owner,
						 const std::string & p_name
                                                 ):
    synoptic::zone_container(p_name,m_width,m_height),
    m_up_border(p_owner,p_name + "_up_border",m_width,1,255,255,255),
    m_right_border(p_owner,p_name + "_right_border",1,m_height - 2,255,255,255),
    m_down_border(p_owner,p_name + "_down_border",m_width,1,255,255,255),
    m_left_border(p_owner,p_name + "_left_border",1,m_height - 2,255,255,255),
    m_char_display(p_owner,p_name + "_char"),
    m_up_req_port(p_owner,p_name+"_up_req_port",dicoplus_synoptic_port::UP),
    m_up_gnt_port(p_owner,p_name+"_up_gnt_port",dicoplus_synoptic_port::UP),
    m_down_req_port(p_owner,p_name+"_down_req_port",dicoplus_synoptic_port::DOWN),
    m_down_gnt_port(p_owner,p_name+"_down_gnt_port",dicoplus_synoptic_port::DOWN),
    m_left_greq_port(p_owner,p_name+"_left_greq_port",dicoplus_synoptic_port::LEFT),
    m_left_ggnt_port(p_owner,p_name+"_left_ggnt_port",dicoplus_synoptic_port::LEFT),
    m_left_req_port(p_owner,p_name+"_left_req_port",dicoplus_synoptic_port::LEFT),
    m_left_gnt_port(p_owner,p_name+"_left_gnt_port",dicoplus_synoptic_port::LEFT),
    m_right_greq_port(p_owner,p_name+"_right_greq_port",dicoplus_synoptic_port::RIGHT),
    m_right_ggnt_port(p_owner,p_name+"_right_ggnt_port",dicoplus_synoptic_port::RIGHT),
    m_right_req_port(p_owner,p_name+"_right_req_port",dicoplus_synoptic_port::RIGHT),
    m_right_gnt_port(p_owner,p_name+"_right_gnt_port",dicoplus_synoptic_port::RIGHT)
    {
      // Adding borders
      add_zone(0,0,m_up_border);
      add_zone(m_width - 1,1,m_right_border);
      add_zone(0,m_height - 1,m_down_border);
      add_zone(0,1,m_left_border);

      // Adding char representation
      add_zone(4,5,m_char_display);

      // Adding ports
      uint32_t l_x_up_down_left_port = 5;
      uint32_t l_x_up_down_right_port = l_x_up_down_left_port + m_up_req_port.get_width() + 3;
      uint32_t l_y_up_ports = 1;
      uint32_t l_y_down_ports = m_height - 1 - dicoplus_synoptic_port::get_short_dim();
      add_zone(l_x_up_down_left_port,l_y_up_ports,m_up_req_port);
      add_zone(l_x_up_down_right_port,l_y_up_ports,m_up_gnt_port);
      add_zone(l_x_up_down_left_port,l_y_down_ports,m_down_req_port);
      add_zone(l_x_up_down_right_port,l_y_down_ports,m_down_gnt_port);

      uint32_t l_x_left_ports = 1;
      uint32_t l_x_right_ports = m_width - 1 - dicoplus_synoptic_port::get_short_dim();

      uint32_t l_y_greq_ports = 2;
      uint32_t l_y_ggnt_ports = l_y_greq_ports + dicoplus_synoptic_port::get_long_dim() + 1;
      uint32_t l_y_req_ports = l_y_ggnt_ports + dicoplus_synoptic_port::get_long_dim() + 2;
      uint32_t l_y_gnt_ports = l_y_req_ports + dicoplus_synoptic_port::get_long_dim() + 1;
      add_zone(l_x_left_ports,l_y_greq_ports,m_left_greq_port);
      add_zone(l_x_left_ports,l_y_ggnt_ports,m_left_ggnt_port);
      add_zone(l_x_left_ports,l_y_req_ports,m_left_req_port);
      add_zone(l_x_left_ports,l_y_gnt_ports,m_left_gnt_port);
      add_zone(l_x_right_ports,l_y_greq_ports,m_right_greq_port);
      add_zone(l_x_right_ports,l_y_ggnt_ports,m_right_ggnt_port);
      add_zone(l_x_right_ports,l_y_req_ports,m_right_req_port);
      add_zone(l_x_right_ports,l_y_gnt_ports,m_right_gnt_port);

    }
  //----------------------------------------------------------------------------
  void dicoplus_synoptic_cell::compute_dim(void)
  {
    if(!m_computed_dim)
      {
	m_computed_dim = true;
	m_width = 1 * 2 + 2 * 2 + 3 + 2 * dicoplus_synoptic_port::get_short_dim()  + 2 * dicoplus_synoptic_port::get_long_dim();
	m_height = 1 * 2 + 2 * 1 + 4 * 1 +4 * dicoplus_synoptic_port::get_long_dim();
      }
  }
}

#endif // _DICOPLUS_SYNOPTIC_CELL_H_
//EOF
