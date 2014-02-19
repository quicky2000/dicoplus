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
#include "top.h"
#include "dicoplus_synoptic_grid.h"
#include "screen_display.h"
#include "parameter_manager.h"
#include "synoptic.h"

using namespace std;
using namespace parameter_manager;

int sc_main(int argc,char ** argv)
{
  try
    {
      // Defining application command line parameters
      parameter_manager::parameter_manager l_param_manager("dicoplus.exe","--",1);
      parameter_if l_input_file("input_file",false);
      l_param_manager.add(l_input_file);

      // Treating parameters
      l_param_manager.treat_parameters(argc,argv);

      dicoplus::top l_top("top",l_input_file.get_value<std::string>());
      
      synoptic::synoptic l_synoptic(1550,850);
      dicoplus::dicoplus_synoptic_cell::compute_dim();
      dicoplus::dicoplus_synoptic_grid l_grid(l_synoptic,"grid",31,31);
      l_synoptic.add_zone(642,0,l_grid);
      dicoplus::screen_display l_screen_display(l_synoptic);
      l_synoptic.add_zone(0,0,l_screen_display);
      l_synoptic.pack();
      l_synoptic.paint();
      l_synoptic.refresh();
      sleep(10);
      sc_start();
    }
  catch(quicky_exception::quicky_runtime_exception & e)
    {
      std::cout << "ERROR : " << e.what() << std::endl ;
      return(-1);
    }
  catch(quicky_exception::quicky_logic_exception & e)
    {
      std::cout << "ERROR : " << e.what() << std::endl ;
      return(-1);
    }
  return 0;
}
