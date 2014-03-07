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
#include "parameter_manager.h"
#include "static_manager.h"
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
      parameter_if l_refresh_delay("refresh_delay",true);
      l_param_manager.add(l_refresh_delay);

      // Treating parameters
      l_param_manager.treat_parameters(argc,argv);

      if(!l_refresh_delay.value_set()) l_refresh_delay.set_text_value("1");

      dicoplus::static_manager l_static_manager;
      dicoplus::top l_top("top",l_input_file.get_value<std::string>(),l_refresh_delay.get_value<uint32_t>());
      
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
