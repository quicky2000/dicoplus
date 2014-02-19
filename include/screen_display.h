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
#ifndef SCREEN_DISPLAY_H
#define SCREEN_DISPLAY_H

#include "synoptic.h"
#include "zone_container.h"
#include "color_zone.h"
#include "drawing_zone.h"

namespace dicoplus
{
  class screen_display: public synoptic::zone_container
  {
  public:
    inline screen_display(synoptic::synoptic &);
  private:
    synoptic::color_zone m_up_border;
    synoptic::color_zone m_right_border;
    synoptic::color_zone m_down_border;
    synoptic::color_zone m_left_border;
    synoptic::drawing_zone m_drawing_zone;
    static const uint32_t m_internal_width;
    static const uint32_t m_internal_height;
    static const uint32_t m_border_size;
  };

  //----------------------------------------------------------------------------
  screen_display::screen_display(synoptic::synoptic & p_owner):
    synoptic::zone_container("screen_display",m_internal_width + 2 * m_border_size,m_internal_height + 2 * m_border_size),
    m_up_border(p_owner,"screen_display_up_border",m_internal_width,m_border_size,128,128,128),
    m_right_border(p_owner,"screen_display_right_border",m_border_size,m_internal_height,128,128,128),
    m_down_border(p_owner,"screen_display_down_border",m_internal_width,m_border_size,128,128,128),
    m_left_border(p_owner,"screen_display_left_border",m_border_size,m_internal_height,128,128,128),
    m_drawing_zone(p_owner,"screen_display_drawing_zone",m_internal_width,m_internal_height)
    {
      add_zone(m_border_size,0,m_up_border);
      add_zone(m_border_size + m_internal_width,m_border_size,m_right_border);
      add_zone(m_border_size,m_border_size + m_internal_height,m_down_border);
      add_zone(0,m_border_size,m_left_border);
      add_zone(m_border_size,m_border_size,m_drawing_zone);
    }
}

#endif // SCREEN_DISPLAY_H
//EOF
