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
#ifndef _DICOPLUS_LINK_STATE_H_
#define _DICOPLUS_LINK_STATE_H_

#include "quicky_exception.h"
#include "dicoplus_types.h"
#include <map>
#include <sstream>

namespace dicoplus
{
  class dicoplus_link_state
  {
    friend std::ostream & operator<<(std::ostream & p_stream,const dicoplus_link_state & p_link_state);
  public:
    inline dicoplus_link_state(void);
    inline void set_orientation(const dicoplus_types::t_orientation & p_orientation);
    inline void set_potential_predecessor(void);
    inline void set_matched_predecessor(void);
    inline void confirm_predecessor(void);
    inline void cancel_predecessor(void);
    inline void cancel_matched_predecessor(void);
    inline void cancel_potential_predecessor(void);

    inline bool is_potential_predecessor(void)const;
    inline bool is_matched_predecessor(void)const;
    inline bool is_predecessor(void)const;

    inline void set_potential_first_predecessor(void);
    inline void confirm_first_predecessor(void);
    inline void cancel_first_predecessor(void);

    inline bool is_potential_first_predecessor(void)const;
    inline bool is_first_predecessor(void)const;

    inline void set_potential_successor_of(const dicoplus_types::t_orientation & p_orientation);
    inline void confirm_successor_of(const dicoplus_types::t_orientation & p_orientation);
    inline void cancel_successor_of(const dicoplus_types::t_orientation & p_orientation);

    inline bool is_potential_successor_of(const dicoplus_types::t_orientation & p_orientation)const;
    inline bool is_successor_of(const dicoplus_types::t_orientation & p_orientation)const;
    inline void clean(bool p_force);
  private:
    typedef enum
      {
	LINK_NONE,
	LINK_TO_BE_CONFIRMED,
	LINK_CONFIRMED
      } t_link_state;

    inline static std::string link_state2string(const t_link_state & p_state);


    // Link orientation
    dicoplus_types::t_orientation m_orientation;

    // Indicate if this the first link
    t_link_state m_first_link;

    // Indicate if this a link to a previous letter
    bool m_link2previous_potential;
    bool m_link2previous_matched;
    bool m_link2previous_confirmed;

    // Indicate if this a link to a following letter and which orientation has the previous one
    typedef std::map<dicoplus_types::t_orientation,t_link_state> t_link2next;
    t_link2next m_link2next;
  };

  //----------------------------------------------------------------------------
  inline std::ostream & operator<<(std::ostream & p_stream,const dicoplus_link_state & p_link_state)
    {
      p_stream << dicoplus_types::orientation2string(p_link_state.m_orientation) << "\t: " ;
      if(dicoplus_link_state::LINK_NONE != p_link_state.m_first_link)
        {
          p_stream << "FIRST : " << dicoplus_link_state::link_state2string(p_link_state.m_first_link) ;
        }
      if(p_link_state.m_link2previous_potential) 
	{
	  p_stream << "Pot";
	}
      if(p_link_state.m_link2previous_matched) 
	{
	  p_stream << "Mat";
	}
      if(p_link_state.m_link2previous_confirmed) 
	{
	  p_stream << "Cfd";
	}
      for(dicoplus_link_state::t_link2next::const_iterator l_iter = p_link_state.m_link2next.begin() ; l_iter != p_link_state.m_link2next.end() ; ++l_iter)
        {
          if(dicoplus_link_state::LINK_NONE != l_iter->second)
            {
              p_stream << "\t" << dicoplus_types::orientation2string(l_iter->first) << "-> " << dicoplus_link_state::link_state2string(l_iter->second) << "\t";
            }
        }
      return p_stream;
    }

  //----------------------------------------------------------------------------
  dicoplus_link_state::dicoplus_link_state(void):
    m_first_link(LINK_NONE),
    m_link2previous_potential(false),
    m_link2previous_matched(false),
    m_link2previous_confirmed(false)
      {
      }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::clean(bool p_force)
    {
      if(p_force || LINK_CONFIRMED != m_first_link) m_first_link = LINK_NONE;
      m_link2previous_potential = false;
      m_link2previous_matched = false;
      for(t_link2next::iterator l_iter = m_link2next.begin() ; m_link2next.end() != l_iter ; ++l_iter)
        {
          if(p_force || LINK_CONFIRMED != l_iter->second) l_iter->second = LINK_NONE;
        }
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::set_orientation(const dicoplus_types::t_orientation & p_orientation)
    {
      m_orientation = p_orientation;
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::set_potential_predecessor(void)
    {
      m_link2previous_potential = true;
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::set_matched_predecessor(void)
    {
      m_link2previous_potential = false;
      m_link2previous_matched = true;
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::confirm_predecessor(void)
    {
      m_link2previous_matched = false;
      m_link2previous_confirmed = true;;
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::cancel_predecessor(void)
    {
      m_link2previous_confirmed = false;
    }
    //----------------------------------------------------------------------------
    void dicoplus_link_state::cancel_matched_predecessor(void)
    {
      m_link2previous_matched = false;
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::cancel_potential_predecessor(void)
    {
      m_link2previous_potential = false;
    }

    //----------------------------------------------------------------------------
    bool dicoplus_link_state::is_potential_predecessor(void)const
    {
      return m_link2previous_potential;
    }

    //----------------------------------------------------------------------------
    bool dicoplus_link_state::is_matched_predecessor(void)const
    {
      return m_link2previous_matched;
    }

    //----------------------------------------------------------------------------
    bool dicoplus_link_state::is_predecessor(void)const
    {
      return m_link2previous_confirmed;
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::set_potential_first_predecessor(void)
    {
      m_first_link = LINK_TO_BE_CONFIRMED;
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::confirm_first_predecessor(void)
    {
      m_first_link = LINK_CONFIRMED;
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::cancel_first_predecessor(void)
    {
      m_first_link = LINK_NONE;
    }

    //----------------------------------------------------------------------------
    bool dicoplus_link_state::is_potential_first_predecessor(void)const
    {
      return LINK_TO_BE_CONFIRMED == m_first_link;
    }

    //----------------------------------------------------------------------------
    bool dicoplus_link_state::is_first_predecessor(void)const
    {
      return LINK_CONFIRMED == m_first_link;
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::set_potential_successor_of(const dicoplus_types::t_orientation & p_orientation)
    {
      if(p_orientation == m_orientation)
        throw quicky_exception::quicky_logic_exception("Link \""+dicoplus_types::orientation2string(p_orientation)+"\" cannot be potential successor of itself",__LINE__,__FILE__);

      t_link2next::iterator l_iter = m_link2next.find(p_orientation);
      if(m_link2next.end() != l_iter)
        {
          assert(LINK_NONE == l_iter->second);
          l_iter->second = LINK_TO_BE_CONFIRMED;
        }
      else
        {
          m_link2next.insert(t_link2next::value_type(p_orientation,LINK_TO_BE_CONFIRMED));
        }
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::confirm_successor_of(const dicoplus_types::t_orientation & p_orientation)
    {
      if(p_orientation == m_orientation)
        throw quicky_exception::quicky_logic_exception("Link \""+dicoplus_types::orientation2string(p_orientation)+"\" cannot be successor of itself",__LINE__,__FILE__);
      t_link2next::iterator l_iter = m_link2next.find(p_orientation);
      if(m_link2next.end() != l_iter)
        {
          assert(LINK_TO_BE_CONFIRMED == l_iter->second);
          l_iter->second = LINK_CONFIRMED;
        }
      else
        {
          m_link2next.insert(t_link2next::value_type(p_orientation,LINK_CONFIRMED));
        }
    }

    //----------------------------------------------------------------------------
    void dicoplus_link_state::cancel_successor_of(const dicoplus_types::t_orientation & p_orientation)
    {
      if(p_orientation == m_orientation)
        throw quicky_exception::quicky_logic_exception("Link \""+dicoplus_types::orientation2string(p_orientation)+"\" cannot have been defined as successor of itself",__LINE__,__FILE__);
      t_link2next::iterator l_iter = m_link2next.find(p_orientation);
      if(m_link2next.end() != l_iter)
        {
          l_iter->second = LINK_NONE;
        }
      else
        {
          m_link2next.insert(t_link2next::value_type(p_orientation,LINK_NONE));
        }
    }

    //----------------------------------------------------------------------------
    bool dicoplus_link_state::is_potential_successor_of(const dicoplus_types::t_orientation & p_orientation)const
    {
      
      t_link2next::const_iterator l_iter = m_link2next.find(p_orientation);
      return m_link2next.end() == l_iter ? false : LINK_TO_BE_CONFIRMED == l_iter->second;
    }

    //----------------------------------------------------------------------------
    bool dicoplus_link_state::is_successor_of(const dicoplus_types::t_orientation & p_orientation)const
    {
      t_link2next::const_iterator l_iter = m_link2next.find(p_orientation);
      return m_link2next.end() == l_iter ? false : LINK_CONFIRMED == l_iter->second;
    }

    //----------------------------------------------------------------------------
    std::string dicoplus_link_state::link_state2string(const t_link_state & p_state)
      {
        switch(p_state)
          {
          case LINK_NONE:
            return "LINK_NONE";
            break;
          case LINK_TO_BE_CONFIRMED:
            return "LINK_TO_BE_CONFIRMED";
            break;
          case LINK_CONFIRMED:
            return "LINK_CONFIRMED";
            break;
          default:
            std::stringstream l_stream;
            l_stream << p_state;
            throw quicky_exception::quicky_logic_exception("No string representation for link state \""+l_stream.str()+"\"",__LINE__,__FILE__);
            break;
          }
      }
}

#endif // _DICOPLUS_LINK_STATE_H_
//EOF
