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

#ifndef _STATIC_MANAGER_H_
#define _STATIC_MANAGER_H_

#include "dicoplus_char.h"
#include "dicoplus_synoptic_cell.h"
#include "dicoplus_synoptic_char.h"
#include <iostream>

namespace dicoplus
{
  class static_manager
  {
  public:
    inline static_manager(void);
    inline ~static_manager();
  private:
  };

  //----------------------------------------------------------------------------
  static_manager::static_manager(void)
    {
      std::cout << "Static initialisation started" << std::endl;
      dicoplus_char::init();
      dicoplus_synoptic_char::init();
      dicoplus_synoptic_cell::compute_dim();
      std::cout << "Static initialisation done" << std::endl ;
    }

  //----------------------------------------------------------------------------
  static_manager::~static_manager(void)
    {
      std::cout << "Static destruction started" << std::endl;
      dicoplus_char::clear();
      std::cout << "Static destruction done" << std::endl ;
    }
}

#endif // _STATIC_MANAGER_H_
//EOF
