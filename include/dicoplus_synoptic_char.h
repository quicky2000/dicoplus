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
  dicoplus_synoptic_char::dicoplus_synoptic_char(synoptic::zone_owner_if & p_owner,
						 const std::string & p_name):
    synoptic::custom_field_zone(p_owner,p_name,m_width,m_height)
    {
      if(!m_content_representation.size())
	{
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(0, 0x454631fc631));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(1, 0x1e4a52e4a53e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(2, 0xe8c2108422e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(3, 0x1e4a5294a53e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(4, 0x1f8421e8421f));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(5, 0x1f8421e84210));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(6, 0xe8c2109c62e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(7, 0x118c63f8c631));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(8, 0xe210842108e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(9, 0x71084210a4c));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(10, 0x118ca98a4a31));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(11, 0x10842108421f));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(12, 0x118eeb58c631));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(13, 0x11ce6b59ce31));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(14, 0xe8c6318c62e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(15, 0x1e8c63e84210));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(16, 0x1d18c6318d5c1));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(17, 0x1e8c63ea4a31));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(18, 0xe8c20e0862e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(19, 0x1f2108421084));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(20, 0x118c6318c62e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(21, 0x118c62a52884));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(22, 0x118c635ad6aa));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(23, 0x118a94452a31));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(24, 0x118a94421084));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(25, 0x1f088444221f));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(26, 0x104011518fe31));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(27, 0xc9011518fe31));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(28, 0x1d1842108b888));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(29, 0x10407e10f421f));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(30, 0x4407e10f421f));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(31, 0xc907e10f421f));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(32, 0x14a07e10f421f));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(33, 0xc9038842108e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(34, 0x14a038842108e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(35, 0xc903a318c62e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(36, 0x14a046318c62e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(37, 0x104046318c62e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(38, 0xc9046318c62e));
	  m_content_representation.insert(std::map<uint32_t,uint64_t>::value_type(39, 0x14a0454a21084));
	}
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
