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
#ifndef _DICOPLUS_SYNOPTIC_GRID_H_
#define _DICOPLUS_SYNOPTIC_GRID_H_
#include "zone_container.h"
#include "dicoplus_synoptic_cell.h"
#include <inttypes.h>
#include <sstream>

namespace dicoplus
{
  class dicoplus_synoptic_grid: public synoptic::zone_container
  {
  public:
    inline dicoplus_synoptic_grid(synoptic::synoptic &,
				  const std::string & p_name,
				  const uint32_t & p_width,
				  const uint32_t & p_height);

    inline ~dicoplus_synoptic_grid(void);
    inline dicoplus_synoptic_cell & get_cell(const uint32_t & p_x,
					     const uint32_t & p_y);
  private:
    dicoplus_synoptic_cell *** m_cells;
    const uint32_t m_nb_cell_width;
    const uint32_t m_nb_cell_height;
    static const uint32_t m_inter_h_cell;
    static const uint32_t m_inter_v_cell;
  };

  //----------------------------------------------------------------------------
  dicoplus_synoptic_cell & dicoplus_synoptic_grid::get_cell(const uint32_t & p_x,
							    const uint32_t & p_y)
    {
      if(p_x >= m_nb_cell_width)
	{
	  std::stringstream l_stream;
	  l_stream << "Value " << p_x << " >= to dicoplus grid synoptic width " << m_nb_cell_width;
	  throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	}
      if(p_y >= m_nb_cell_height)
	{
	  std::stringstream l_stream;
	  l_stream << "Value " << p_y << " >= to dicoplus grid symoptic height " << m_nb_cell_height;
	  throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	}
      return *(m_cells[p_x][p_y]);
    }

  //----------------------------------------------------------------------------
  dicoplus_synoptic_grid::dicoplus_synoptic_grid(synoptic::synoptic &p_owner,
						 const std::string & p_name,
						 const uint32_t & p_width,
						 const uint32_t & p_height):
    synoptic::zone_container(p_name,p_width * ( dicoplus_synoptic_cell::get_width() + m_inter_h_cell),p_height *(dicoplus_synoptic_cell::get_height() + m_inter_v_cell)),
    m_cells(NULL),
    m_nb_cell_width(p_width),
    m_nb_cell_height(p_height)
    {
	unsigned int l_allocated_width = 0;
	unsigned int l_allocated_height = 0;
	std::string l_cell_base_name = "synoptic_cell_";
	try
	  {
	    // Creating cell array
	    m_cells = new dicoplus_synoptic_cell** [m_nb_cell_width];
	    std::fill_n(m_cells,m_nb_cell_width,static_cast<dicoplus_synoptic_cell**>(NULL));

	    for(l_allocated_width = 0; l_allocated_width < m_nb_cell_width ; ++l_allocated_width)
	      {
		// Complete cell name
		std::stringstream l_width_stream;
		l_width_stream << l_allocated_width;
		std::string l_cell_base_width_name = l_cell_base_name + l_width_stream.str();

		// Allocate cell colum
		m_cells[l_allocated_width] = new dicoplus_synoptic_cell*[m_nb_cell_height];
		std::fill_n(m_cells[l_allocated_width],m_nb_cell_height,static_cast<dicoplus_synoptic_cell*>(NULL));

		for(l_allocated_height = 0; l_allocated_height < m_nb_cell_height ; ++l_allocated_height)
		  {
		    // Completing cell name
		    std::stringstream l_height_stream;
		    l_height_stream << l_allocated_height;
		    std::string l_cell_name = l_cell_base_width_name+"_"+l_height_stream.str();
		    // Creating cell
		    uint32_t l_x = (dicoplus_synoptic_cell::get_width() + m_inter_h_cell) * l_allocated_width;
		    uint32_t l_y = (dicoplus_synoptic_cell::get_height() + m_inter_v_cell) * l_allocated_height;
		    dicoplus_synoptic_cell * l_cell = new dicoplus_synoptic_cell(p_owner,l_cell_name.c_str());
                    add_zone(l_x,l_y,*l_cell);
		    m_cells[l_allocated_width][l_allocated_height] = l_cell;
		  }
	      }
	  }
	catch ( const std::bad_alloc & ) // Allocation error
	  {
	    // Free partially allocated dmemory
	    for (unsigned int l_index_width = 0; l_index_width < l_allocated_width - 1; ++l_index_width) 
	      {
		for(unsigned int l_index_height = 0; l_index_height < m_nb_cell_height ; ++l_index_height)
		  {
		    delete m_cells[l_index_width][l_index_height];
		  }
		delete m_cells[l_index_width];
	      }
	    for(unsigned int l_index_height = 0; l_index_height < l_allocated_height ; ++l_index_height)
	      {
		delete m_cells[l_allocated_width][l_index_height];
	      }
	    delete[] m_cells[l_allocated_width];
	    delete[] m_cells;
	  }
    }

    //--------------------------------------------------------------------------
    dicoplus_synoptic_grid::~dicoplus_synoptic_grid(void)
      {
        for(unsigned int l_index = 0; l_index < m_nb_cell_width ; ++l_index)
          {
            for(unsigned int l_index2 = 0; l_index2 < m_nb_cell_height ; ++l_index2)
              {
                delete m_cells[l_index][l_index2];
              }
            delete[] m_cells[l_index];
          }        
        delete[] m_cells;
      }
}
#endif // _DICOPLUS_SYNOPTIC_GRID_H_
//EOF
