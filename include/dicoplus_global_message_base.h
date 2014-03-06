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
#ifndef _DICOPLUS_GLOBAL_MESSAGE_BASE_H_
#define _DICOPLUS_GLOBAL_MESSAGE_BASE_H_

#include "dicoplus_types.h"

namespace dicoplus
{
  class dicoplus_global_message_analyzer_if;

  class dicoplus_global_message_base
  {
  public:
    inline dicoplus_global_message_base(const dicoplus_types::t_global_cmd_type & p_cmd,
					const dicoplus_types::t_global_data_type & p_data);
    virtual void be_treated(dicoplus_global_message_analyzer_if & p_analyzer)const=0;
    inline virtual ~dicoplus_global_message_base(void){}
    typedef enum {CHAR,SEPARATOR} t_cmd_global_message;

    inline const dicoplus_types::t_global_cmd_type & get_cmd(void)const;
    inline const dicoplus_types::t_global_data_type & get_data(void)const;

  private:
    const dicoplus_types::t_global_cmd_type m_cmd;
    const dicoplus_types::t_global_data_type m_data;
  };

  //----------------------------------------------------------------------------
  dicoplus_global_message_base::dicoplus_global_message_base(const dicoplus_types::t_global_cmd_type & p_cmd,
							     const dicoplus_types::t_global_data_type & p_data):
    m_cmd(p_cmd),
    m_data(p_data)
    {
    }

    //--------------------------------------------------------------------------
    const dicoplus_types::t_global_cmd_type & dicoplus_global_message_base::get_cmd(void)const
      {
	return m_cmd;
      }

    //--------------------------------------------------------------------------
    const dicoplus_types::t_global_data_type & dicoplus_global_message_base::get_data(void)const
      {
	return m_data;
      }

}

#endif // _DICOPLUS_GLOBAL_MESSAGE_BASE_H_
//EOF
