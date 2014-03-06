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
#ifndef _DICOPLUS_GLOBAL_MESSAGE_FACTORY_H_
#define _DICOPLUS_GLOBAL_MESSAGE_FACTORY_H_

#include "dicoplus_global_message_separator.h"
#include "dicoplus_global_message_char.h"
#include <sstream>

namespace dicoplus
{
  class dicoplus_global_message_factory
  {
  public:
    static inline const dicoplus_global_message_base * const decode_message(const dicoplus_types::t_global_cmd_type & p_cmd,
									    const dicoplus_types::t_global_data_type & p_data);
  private:
  };

  //----------------------------------------------------------------------------
  const dicoplus_global_message_base * const dicoplus_global_message_factory::decode_message(const dicoplus_types::t_global_cmd_type & p_cmd,
                                                                                        const dicoplus_types::t_global_data_type & p_data)
    {
      switch(p_cmd.to_uint())
        {
        case dicoplus_global_message_base::CHAR:
          return new dicoplus_global_message_char(p_data);
          break;
        case dicoplus_global_message_base::SEPARATOR:
          return new dicoplus_global_message_separator();
          break;
        default:
	  {
	    std::stringstream l_stream;
	    l_stream << "Unkown command code " << p_cmd.to_uint() << " for global message";
	    throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	  }
	  break;
        }
      return NULL;
    }
}

#endif // _DICOPLUS_GLOBAL_MESSAGE_FACTORY_H_
//EOF
