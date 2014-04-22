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
#ifndef DICOPLUS_SYNOPTIC_LOCAL_BUS_H
#define DICOPLUS_SYNOPTIC_LOCAL_BUS_H

#include "display_zone.h"
#include "quicky_exception.h"
#include <set>
#include <inttypes.h>
#include <stdlib.h>
#include <assert.h>

namespace dicoplus
{
  class dicoplus_synoptic_local_bus:public synoptic::display_zone
  {
  public:
    inline void set_color_code(const unsigned int & p_code,
                               const uint8_t & p_r,
                               const uint8_t & p_g,
                               const uint8_t & p_b);

  protected:
    inline dicoplus_synoptic_local_bus(synoptic::zone_owner_if &,
				       const std::string & p_name,
				       const unsigned int & p_width,
				       const unsigned int & p_height);
    inline static void init(const std::string & p_coded_representation,
			    std::set<std::pair<uint32_t,uint32_t> > (&p_pixel_locations)[4],
                            const uint32_t & p_width,
                            const uint32_t p_height);
    inline virtual ~dicoplus_synoptic_local_bus(void){}


  private:
    virtual const std::set<std::pair<uint32_t,uint32_t> > & get_pixel_location(const unsigned int & p_code)const=0;

    // virtual method inherited from zone
    inline void internal_paint(void);
    //end of virtual method inherited from zone

    std::pair<bool,uint32_t> m_color_codes[4];
  };


  //----------------------------------------------------------------------------
  dicoplus_synoptic_local_bus::dicoplus_synoptic_local_bus(synoptic::zone_owner_if & p_owner,
							   const std::string & p_name,
							   const unsigned int & p_width,
							   const unsigned int & p_height):
    synoptic::display_zone(p_owner,p_name,p_width,p_height)
    {
      m_color_codes[0] = std::pair<bool,uint32_t>(true,p_owner.get_color_code(255,255,255));
      m_color_codes[1] = std::pair<bool,uint32_t>(true,p_owner.get_color_code(0,0,255));
      m_color_codes[2] = std::pair<bool,uint32_t>(true,p_owner.get_color_code(0,255,0));
      m_color_codes[3] = std::pair<bool,uint32_t>(true,p_owner.get_color_code(255,0,0));
    }

    //----------------------------------------------------------------------------
    void dicoplus_synoptic_local_bus::set_color_code(const unsigned int & p_code,
                                                 const uint8_t & p_r,
                                                 const uint8_t & p_g,
                                                 const uint8_t & p_b)
    {
      if(p_code > 3) throw quicky_exception::quicky_logic_exception("Out of bounds code color "+p_code,__LINE__,__FILE__);
      uint32_t l_new_color_code = get_owner().get_color_code(p_r,p_g,p_b);
      if(l_new_color_code != m_color_codes[p_code].second) 
        {
          m_color_codes[p_code] = std::pair<bool,uint32_t>(true,l_new_color_code);      
        }
    }

    //----------------------------------------------------------------------------
    void dicoplus_synoptic_local_bus::internal_paint(void)
    {
      for(unsigned int l_index = 0 ; l_index < 4 ; ++l_index)
        {
          if(m_color_codes[l_index].first)
            {
              m_color_codes[l_index].first = false;
	      const std::set<std::pair<uint32_t,uint32_t> > & l_pixel_locations = this->get_pixel_location(l_index);
              for(std::set<std::pair<uint32_t,uint32_t> >::const_iterator l_iter = l_pixel_locations.begin();
                  l_pixel_locations.end() != l_iter ;
                  ++l_iter)
                {
                  set_pixel(l_iter->first,l_iter->second,m_color_codes[l_index].second);
                }
            }
        }
    }

    //--------------------------------------------------------------------------
    void dicoplus_synoptic_local_bus::init(const std::string & p_coded_representation,
					   std::set<std::pair<uint32_t,uint32_t> > (&p_pixel_locations)[4],
                                           const uint32_t & p_width,
                                           const uint32_t p_height)
    {
      if("0x" != p_coded_representation.substr(0,2)) throw quicky_exception::quicky_logic_exception("Bad internal representation for H Bus : \"" + p_coded_representation +"\"",__LINE__,__FILE__);

      for(size_t l_pos =  p_coded_representation.size() - 1; l_pos >= 2 ; --l_pos)
	{
	  unsigned int l_reversed_pos = p_coded_representation.size() - 1 - l_pos;
	  unsigned int l_char = (unsigned int)strtol(std::string(1,p_coded_representation[l_pos]).c_str(),NULL,16);
	  for(unsigned int l_internal_index = 0 ; l_internal_index < 2 ; ++l_internal_index)
	    {
	      unsigned int l_code = l_char & 0x3;
	      l_char = l_char >> 2;
	      unsigned int l_x = ( p_width * p_height - 1 - l_reversed_pos * 2 - l_internal_index ) % p_width;
              unsigned int l_y = ( p_width * p_height - 1 - l_reversed_pos * 2 - l_internal_index ) / p_width;
	      p_pixel_locations[l_code].insert(std::pair<uint32_t,uint32_t>(l_x,l_y));
	    }
	}
    }

}
#endif // DICOPLUS_SYNOPTIC_LOCAL_BUS_H
//EOF
