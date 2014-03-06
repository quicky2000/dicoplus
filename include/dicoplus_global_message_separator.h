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
#ifndef _DICOPLUS_GLOBAL_MESSAGE_SEPARATOR_H_
#define _DICOPLUS_GLOBAL_MESSAGE_SEPARATOR_H_

#include "dicoplus_global_message_base.h"
#include "dicoplus_global_message_analyzer_if.h"

namespace dicoplus
{
  class dicoplus_global_message_separator: public dicoplus_global_message_base
  {
  public:
    inline dicoplus_global_message_separator(void);
    inline void be_treated(dicoplus_global_message_analyzer_if & p_analyzer)const;
  private:
  };

  //----------------------------------------------------------------------------
  dicoplus_global_message_separator::dicoplus_global_message_separator(void):
    dicoplus_global_message_base(dicoplus_global_message_base::SEPARATOR,0)
    {
    }
  //----------------------------------------------------------------------------
    void dicoplus_global_message_separator::be_treated(dicoplus_global_message_analyzer_if & p_analyzer)const
    {
      p_analyzer.treat(*this);
    }
}
#endif // _DICOPLUS_GLOBAL_MESSAGE_SEPARATOR_H_
//EOF
