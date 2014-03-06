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
#ifndef DICOPLUS_GLOBAL_MESSAGE_BOX_H
#define DICOPLUS_GLOBAL_MESSAGE_BOX_H

#include "dicoplus_global_message_base.h"

namespace dicoplus
{
    class dicoplus_global_message_box
    {
    public:
      inline dicoplus_global_message_box(void);
      inline void set_message(const dicoplus_global_message_base & p_message);
      inline const dicoplus_global_message_base & get_message(void);
      inline bool is_empty(void)const;
    private:
	bool m_empty;
	dicoplus_global_message_base const * m_message;
    };

    //--------------------------------------------------------------------------
    dicoplus_global_message_box::dicoplus_global_message_box(void):
	m_empty(true),
	m_message(NULL)
	  {
	  }

    //--------------------------------------------------------------------------
      void dicoplus_global_message_box::set_message(const dicoplus_global_message_base & p_message)
	  {
	    m_empty = false;
	    m_message = & p_message;
	  }

    //--------------------------------------------------------------------------
    const dicoplus_global_message_base & dicoplus_global_message_box::get_message(void)
        {
	  if(NULL == m_message) throw quicky_exception::quicky_logic_exception("Try to read empty message_box",__LINE__,__FILE__);
          dicoplus_global_message_base const * l_result = m_message;
          m_empty = true;
          m_message = NULL;
          return *l_result;
        }

    //--------------------------------------------------------------------------
    bool dicoplus_global_message_box::is_empty(void)const
    {
      return m_empty;
    }
}
#endif // DICOPLUS_GLOBAL_MESSAGE_BOX_H
//EOF
