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
#include "dicoplus_synoptic_cell.h"

namespace dicoplus
{
  bool dicoplus_synoptic_cell::m_computed_dim = false;
  uint32_t dicoplus_synoptic_cell::m_width = 0;
  uint32_t dicoplus_synoptic_cell::m_height = 0;
  const uint32_t dicoplus_synoptic_cell::m_req_gnt_dim = 3;
  const uint32_t dicoplus_synoptic_cell::m_border_padding = 3;
  const uint32_t dicoplus_synoptic_cell::m_middle_dim = 16;
  std::map<dicoplus_types::t_cell_FSM_state,std::pair<uint32_t,uint32_t> > dicoplus_synoptic_cell::m_cell_state_representation;
}

//EOF
