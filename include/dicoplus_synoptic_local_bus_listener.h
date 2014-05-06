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

#ifndef DICOPLUS_SYNOPTIC_LOCAL_BUS_LISTENER_IF_H
#define DICOPLUS_SYNOPTIC_LOCAL_BUS_LISTENER_IF_H

#include "dicoplus_local_bus_listener_if.h"

namespace dicoplus
{
  class synoptic_local_bus_listener: public dicoplus_local_bus_listener_if
  {
  public:
    inline synoptic_local_bus_listener(const unsigned int & p_color_code,
                                       dicoplus_synoptic_local_bus & p_bus);
    // Virtual methods to implement inherited from dicoplus_local_bus_listener_if
    inline void no_activity(void);
    inline void data(bool p_data);
    inline void cancel(void);
    // End of virtual methods
  private:
    const unsigned int m_color_code;
    dicoplus_synoptic_local_bus & m_bus;
  };

  //----------------------------------------------------------------------------
  synoptic_local_bus_listener::synoptic_local_bus_listener(const unsigned int & p_color_code,
                                                           dicoplus_synoptic_local_bus & p_bus):
    m_color_code(p_color_code),
    m_bus(p_bus)
      {
      }
    // Virtual methods to implement inherited from dicoplus_local_bus_listener_if
    //----------------------------------------------------------------------------
    void synoptic_local_bus_listener::no_activity(void)
    {
      m_bus.set_color_code(m_color_code,255,255,255);
      m_bus.paint();
    }

    //----------------------------------------------------------------------------
    void synoptic_local_bus_listener::data(bool p_data)
    {
      m_bus.set_color_code(m_color_code,p_data ? 0 : 255, p_data ? 255 : 0,0);
      m_bus.paint();
    }

    //----------------------------------------------------------------------------
    void synoptic_local_bus_listener::cancel(void)
    {
      m_bus.set_color_code(m_color_code,0xFF,0xFF,0);
      m_bus.paint();
    }

}
#endif // DICOPLUS_SYNOPTIC_LOCAL_BUS_LISTENER_IF_H
//EOF
