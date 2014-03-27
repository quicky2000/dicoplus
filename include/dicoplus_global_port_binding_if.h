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
#ifndef _DICOPLUS_GLOBAL_PORT_BINDING_IF_H_
#define _DICOPLUS_GLOBAL_PORT_BINDING_IF_H_

namespace dicoplus
{
  class dicoplus_global_bus;

  class dicoplus_global_port_binding_if
  {
  public:
    virtual void bind_input_port(dicoplus_global_bus & p_bus)=0;
    virtual void bind_output_port(dicoplus_global_bus & p_bus)=0;
    inline virtual ~dicoplus_global_port_binding_if(void){}
  };
}
#endif // _DICOPLUS_GLOBAL_PORT_BINDING_IF_H_
//EOF
