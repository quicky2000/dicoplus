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
#ifndef _DICOPLUS_CONFIGURATION_H_
#define _DICOPLUS_CONFIGURATION_H_

#include "systemc_utils.h"

namespace dicoplus
{
  class dicoplus_configuration
  {
  public:
    static const unsigned int m_nb_characters = 43;
    static const unsigned int m_nb_global_cmd = 4;
    static const unsigned int m_global_cmd_nb_bits = systemc_utils::systemc_utils<dicoplus_configuration::m_nb_global_cmd>::m_nb_bits_for_nb_values;
    static const unsigned int m_characters_nb_bits = systemc_utils::systemc_utils<dicoplus_configuration::m_nb_characters>::m_nb_bits_for_nb_values;
  private:
  };
}
#endif // _DICOPLUS_CONFIGURATION_H_
//EOF
