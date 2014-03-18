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
#ifndef _DICOPLUS_CHAR_H_
#define _DICOPLUS_CHAR_H_

#include "quicky_exception.h"
#include "utf8.h"
#include <string>
#include <inttypes.h>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>

namespace dicoplus
{
  class dicoplus_char
  {
    friend std::ostream& operator<<(std::ostream& s, const dicoplus_char & p_dicoplus_char);
  public:
    inline static void init(void);
    inline static void clear(void);
    inline static const uint32_t get_code_point(const uint32_t & p_internal_code);
    inline static const uint32_t get_internal_code(const uint32_t & p_code_point);
    inline const uint32_t & get_internal_code(void)const;
    inline const uint32_t & get_code_point(void)const;
  private:
    inline static void register_code_point(const uint32_t & p_code_point);
    inline dicoplus_char(const uint32_t & p_code_point,
			 const uint32_t & p_dicoplus_code);

    char m_char_array[5];
    const uint32_t m_code_point;
    const uint32_t m_dicoplus_code;

    static std::map<uint32_t,const dicoplus_char *> m_code_points;
    static std::vector<const dicoplus_char *> m_internals;
  };

  inline std::ostream& operator<<(std::ostream& s, const dicoplus_char & p_dicoplus_char);
  //----------------------------------------------------------------------------
  std::ostream& operator<<(std::ostream& s, const dicoplus_char & p_dicoplus_char)
    {
      s << p_dicoplus_char.m_char_array;
      return s;
    }

  //----------------------------------------------------------------------------
  const uint32_t & dicoplus_char::get_internal_code(void)const
    {
      return m_dicoplus_code;
    }

  //----------------------------------------------------------------------------
  const uint32_t & dicoplus_char::get_code_point(void)const
    {
      return m_code_point;
    }

  //----------------------------------------------------------------------------
  const uint32_t dicoplus_char::get_code_point(const uint32_t & p_internal_code)
  {
    if(p_internal_code <  m_internals.size())
      {
	return m_internals[p_internal_code]->get_code_point();
      }
    else
      {
	throw quicky_exception::quicky_logic_exception("dicoplus_char : Unknown internal code "+p_internal_code,__LINE__,__FILE__);
      }
  }

  //----------------------------------------------------------------------------
  const uint32_t dicoplus_char::get_internal_code(const uint32_t & p_code_point)
  {
    std::map<uint32_t,const dicoplus_char *>::const_iterator l_iter = m_code_points.find(p_code_point);
    if(m_code_points.end() != l_iter)
      {
	return l_iter->second->get_internal_code();
      }
    else
      {
	dicoplus_char l_char(p_code_point,0);
	std::stringstream l_stream;
	l_stream << "dicoplus_char : Code point " << std::hex << p_code_point << std::dec << " representing char '" << l_char << "' is not supported";
	throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
      }
  }

  //----------------------------------------------------------------------------
  dicoplus_char::dicoplus_char(const uint32_t & p_code_point,
			       const uint32_t & p_dicoplus_code):
    m_code_point(p_code_point),
    m_dicoplus_code(p_dicoplus_code)
    {
      std::fill_n(m_char_array,5,NULL);
      utf8::append(p_code_point,m_char_array);
    }
  
  //----------------------------------------------------------------------------
    void dicoplus_char::register_code_point(const uint32_t & p_code_point)
    {
      const dicoplus_char * l_char = new dicoplus_char(p_code_point,m_code_points.size());
      std::cout << "Insert char '" << *l_char << "' with code point 0x" << std::hex << p_code_point << std::dec << " and internal representation = 0x" << std::hex << m_code_points.size() << std::dec << std::endl; 
      m_code_points.insert(std::map<uint32_t,const dicoplus_char *>::value_type(p_code_point,l_char));
      m_internals.push_back(l_char);
    }


    //----------------------------------------------------------------------------
    void dicoplus_char::clear(void)
    {					    
      for(std::vector<const dicoplus_char *>::const_iterator l_iter = m_internals.begin();
	  m_internals.end() != l_iter;
	  ++l_iter)
	{
	  delete(*l_iter);
	}

    }

    //----------------------------------------------------------------------------
    void dicoplus_char::init(void)
    {
      if(!m_code_points.size() && !m_internals.size())
	{
	  std::cout << "Static initialisation : dicoplus_char" << std::endl ;
	  register_code_point(0x20); // " "
	  register_code_point(0x41); // "A"
	  register_code_point(0x42); // "B"
	  register_code_point(0x43); // "C"
	  register_code_point(0x44); // "D"
	  register_code_point(0x45); // "E"
	  register_code_point(0x46); // "F"
	  register_code_point(0x47); // "G"
	  register_code_point(0x48); // "H"
	  register_code_point(0x49); // "I"
	  register_code_point(0x4a); // "J"
	  register_code_point(0x4b); // "K"
	  register_code_point(0x4c); // "L"
	  register_code_point(0x4d); // "M"
	  register_code_point(0x4e); // "N"
	  register_code_point(0x4f); // "O"
	  register_code_point(0x50); // "P"
	  register_code_point(0x51); // "Q"
	  register_code_point(0x52); // "R"
	  register_code_point(0x53); // "S"
	  register_code_point(0x54); // "T"
	  register_code_point(0x55); // "U"
	  register_code_point(0x56); // "V"
	  register_code_point(0x57); // "W"
	  register_code_point(0x58); // "X"
	  register_code_point(0x59); // "Y"
	  register_code_point(0x5a); // "Z"
	  register_code_point(0xc0); // "À"
	  register_code_point(0xc2); // "Â"
	  register_code_point(0xc6); // "Æ"
	  register_code_point(0xc7); // "Ç"
	  register_code_point(0xc8); // "È"
	  register_code_point(0xc9); // "É"
	  register_code_point(0xca); // "Ê"
	  register_code_point(0xcb); // "Ë"
	  register_code_point(0xce); // "Î"
	  register_code_point(0xcf); // "Ï"
	  register_code_point(0xd4); // "Ô"
	  register_code_point(0xdc); // "Ü"
	  register_code_point(0xd9); // "Ù"
	  register_code_point(0xdb); // "Û"
	  register_code_point(0x178); // Y:
	  register_code_point(0xc6); // OE:
	}
      else
	{
	  throw quicky_exception::quicky_logic_exception("dicoplus char : static content was already initiliazed",__LINE__,__FILE__);
	}
    }
}

#endif // _DICOPLUS_CHAR_H_
//EOF
