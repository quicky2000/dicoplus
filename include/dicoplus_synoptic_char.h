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
#ifndef _DICOPLUS_SYNOPTIC_CHAR_H_
#define _DICOPLUS_SYNOPTIC_CHAR_H_

#include "custom_field_zone.h"
#include "dicoplus_char.h"
#include <map>
#include <inttypes.h>
#include <string>

namespace dicoplus
{
  class dicoplus_synoptic_char: public synoptic::custom_field_zone
  {
  public:
    inline dicoplus_synoptic_char(synoptic::zone_owner_if &,
				  const std::string & p_name);
    inline static const uint32_t & get_width(void);
    inline static const uint32_t & get_height(void);
    inline static void init(void);
  private:
    // Methods inherited from custom_field_zone
    inline const uint64_t & get_content_representation(const uint32_t & p_content);
    // End of methods inherited from custom_field_zone

    static std::map<uint32_t,uint64_t> m_content_representation;
    static const uint64_t m_default_representation;
    static const uint32_t m_width;
    static const uint32_t m_height;
  };

  //----------------------------------------------------------------------------
  const uint32_t & dicoplus_synoptic_char::get_width(void)
    {
      return m_width;
    }
  //----------------------------------------------------------------------------
  const uint32_t & dicoplus_synoptic_char::get_height(void)
    {
      return m_height;
    }

  //----------------------------------------------------------------------------
  void dicoplus_synoptic_char::init(void)
  {
      if(!m_content_representation.size())
	{
	  // The key is the Unicode code point
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x20), 0x0));   // " "
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x41), 0x454631fc631));   // A
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x42), 0x1e4a52e4a53e));  // B
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x43), 0xe8c2108422e));   // C
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x44), 0x1e4a5294a53e));  // D
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x45), 0x1f8421e8421f));  // E
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x46), 0x1f8421e84210));  // F
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x47), 0xe8c2109c62e));   // G
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x48), 0x118c63f8c631));  // H
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x49), 0xe210842108e));   // I
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x4a), 0x71084210a4c));   // J
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x4b), 0x118ca98a4a31)); // K
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x4c), 0x10842108421f)); // L
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x4d), 0x118eeb58c631)); // M
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x4e), 0x11ce6b59ce31)); // N
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x4f), 0xe8c6318c62e));  // O
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x50), 0x1e8c63e84210)); // P
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x51), 0x1d18c6318d5c1));// Q
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x52), 0x1e8c63ea4a31)); // R
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x53), 0xe8c20e0862e));  // S
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x54), 0x1f2108421084)); // T
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x55), 0x118c6318c62e)); // U
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x56), 0x118c62a52884)); // V
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x57), 0x118c635ad6aa)); // W
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x58), 0x118a94452a31)); // X
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x59), 0x118a94421084)); // Y
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x5a), 0x1f088444221f)); // Z
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xc0), 0x104011518fe31));// À
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xc2), 0xc9011518fe31)); // Â
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xc7), 0x1d1842108b888));// Ç
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xc8), 0x10407e10f421f));// È
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xc9), 0x4407e10f421f)); // É
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xca), 0xc907e10f421f)); // Ê
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xcb), 0x14a07e10f421f));// Ë
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xce), 0xc9038842108e)); // Î
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xcf), 0x14a038842108e));// Ï
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xd4), 0xc903a318c62e)); // Ô
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xdc), 0x14a046318c62e));// Ü
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xd9), 0x104046318c62e));// Ù
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xdb), 0xc9046318c62e)); // Û
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0x178), 0x14a0454a21084));// Y:
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(dicoplus_char::get_internal_code(0xc6), 0xba5296e5297));// OE
	}
      else
	{
	  throw quicky_exception::quicky_logic_exception("dicoplus_synoptic_char content representation already initialised",__LINE__,__FILE__);
	}
  }

  //----------------------------------------------------------------------------
  dicoplus_synoptic_char::dicoplus_synoptic_char(synoptic::zone_owner_if & p_owner,
						 const std::string & p_name):
    synoptic::custom_field_zone(p_owner,p_name,m_width,m_height)
    {
    }

    //--------------------------------------------------------------------------
    const uint64_t & dicoplus_synoptic_char::get_content_representation(const uint32_t & p_content)
      {
	std::map<uint32_t,uint64_t>::const_iterator l_iter = m_content_representation.find(p_content);
	if(m_content_representation.end() != l_iter)
	  {
	    return l_iter->second;
	  }
	return m_default_representation;
      }

}

#endif // _DICOPLUS_SYNOPTIC_CHAR_H_
//EOF
