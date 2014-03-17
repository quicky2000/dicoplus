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
#ifndef _DICOPLUS_SYNOPTIC_GLOBAL_BUS_H_
#define _DICOPLUS_SYNOPTIC_GLOBAL_BUS_H_

#include "zone_container.h"
#include "dicoplus_synoptic_char.h"
#include "dicoplus_global_message_analyzer_if.h"
#include "dicoplus_global_message_char.h"

namespace dicoplus
{
  class dicoplus_synoptic_global_bus: public synoptic::zone_container,public dicoplus_global_message_analyzer_if
  {
  public:
    inline dicoplus_synoptic_global_bus(synoptic::synoptic &,
					const std::string & p_name);
    inline virtual ~dicoplus_synoptic_global_bus(void){}

    // Methods inherited from dicoplus_global_message_analyzer_if
    inline void treat(const dicoplus_global_message_char & p_message);
    inline void treat(const dicoplus_global_message_separator & p_message);
    // End of methods inherited from dicoplus_global_message_analyzer_if
  private:
    dicoplus_synoptic_char m_char_display;    
  };

  //----------------------------------------------------------------------------
  dicoplus_synoptic_global_bus::dicoplus_synoptic_global_bus(synoptic::synoptic & p_owner,
							     const std::string & p_name
							     ):
    synoptic::zone_container(p_name,7,11),
    m_char_display(p_owner,"char_display")
    {
      add_zone(1,1,m_char_display);
    }
    //----------------------------------------------------------------------------
    void dicoplus_synoptic_global_bus::treat(const dicoplus_global_message_char & p_message)
    {
      m_char_display.set_content(p_message.get_data().to_uint());
      m_char_display.paint();
    }
    //----------------------------------------------------------------------------
    void dicoplus_synoptic_global_bus::treat(const dicoplus_global_message_separator & p_message)
    {
    }
}

#endif // _DICOPLUS_SYNOPTIC_GLOBAL_BUS_H_
//EOF
