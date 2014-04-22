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
#include "dicoplus_synoptic_macro_cell.h"
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
    inline dicoplus_synoptic_global_bus & get_global_bus(const uint32_t & p_x,
                                                         const uint32_t & p_y);

    inline dicoplus_local_bus_listener_if & get_north_listener(const uint32_t & p_x,
							       const uint32_t & p_y);
    inline dicoplus_local_bus_listener_if & get_east_listener(const uint32_t & p_x,
							      const uint32_t & p_y);
    inline dicoplus_local_bus_listener_if & get_south_listener(const uint32_t & p_x,
							       const uint32_t & p_y);
    inline dicoplus_local_bus_listener_if & get_west_listener(const uint32_t & p_x,
							      const uint32_t & p_y);

  private:
    inline void check_coord(const uint32_t & p_x,
			    const uint32_t & p_y,
			    const unsigned int & p_line,
			    const std::string & p_file);

    dicoplus_synoptic_macro_cell *** m_macro_cells;
    const uint32_t m_nb_cell_width;
    const uint32_t m_nb_cell_height;
  };

  //----------------------------------------------------------------------------
  void dicoplus_synoptic_grid::check_coord(const uint32_t & p_x,
					   const uint32_t & p_y,
					   const unsigned int & p_line,
					   const std::string & p_file)
  {
    if(p_x >= m_nb_cell_width)
      {
	std::stringstream l_stream;
	l_stream << "Value " << p_x << " >= to dicoplus grid synoptic width " << m_nb_cell_width;
	throw quicky_exception::quicky_logic_exception(l_stream.str(),p_line,p_file);
      }
    if(p_y >= m_nb_cell_height)
      {
	std::stringstream l_stream;
	l_stream << "Value " << p_y << " >= to dicoplus grid symoptic height " << m_nb_cell_height;
	throw quicky_exception::quicky_logic_exception(l_stream.str(),p_line,p_file);
      }
  }

  //----------------------------------------------------------------------------
  dicoplus_synoptic_cell & dicoplus_synoptic_grid::get_cell(const uint32_t & p_x,
							    const uint32_t & p_y)
    {
      check_coord(p_x,p_y,__LINE__,__FILE__);
      return (m_macro_cells[p_x][p_y])->get_cell();

    }

  //----------------------------------------------------------------------------
  dicoplus_synoptic_global_bus & dicoplus_synoptic_grid::get_global_bus(const uint32_t & p_x,
                                                                        const uint32_t & p_y)
    {
      check_coord(p_x,p_y,__LINE__,__FILE__);
      return (m_macro_cells[p_x][p_y])->get_global_bus();
    }

  //----------------------------------------------------------------------------
  dicoplus_local_bus_listener_if & dicoplus_synoptic_grid::get_north_listener(const uint32_t & p_x,
									      const uint32_t & p_y)
    {
      check_coord(p_x,p_y,__LINE__,__FILE__);
      return (m_macro_cells[p_x][p_y])->get_north_listener();
    }

  //----------------------------------------------------------------------------
  dicoplus_local_bus_listener_if & dicoplus_synoptic_grid::get_east_listener(const uint32_t & p_x,
									     const uint32_t & p_y)
    {
      check_coord(p_x,p_y,__LINE__,__FILE__);
      return (m_macro_cells[p_x][p_y])->get_east_listener();
    }

  //----------------------------------------------------------------------------
  dicoplus_local_bus_listener_if & dicoplus_synoptic_grid::get_south_listener(const uint32_t & p_x,
									      const uint32_t & p_y)
    {
      check_coord(p_x,p_y,__LINE__,__FILE__);
      return (m_macro_cells[p_x][p_y])->get_south_listener();
    }

  //----------------------------------------------------------------------------
  dicoplus_local_bus_listener_if & dicoplus_synoptic_grid::get_west_listener(const uint32_t & p_x,
									     const uint32_t & p_y)
    {
      check_coord(p_x,p_y,__LINE__,__FILE__);
      return (m_macro_cells[p_x][p_y])->get_west_listener();
    }


  //----------------------------------------------------------------------------
  dicoplus_synoptic_grid::dicoplus_synoptic_grid(synoptic::synoptic &p_owner,
						 const std::string & p_name,
						 const uint32_t & p_width,
						 const uint32_t & p_height):
    synoptic::zone_container(p_name,p_width * dicoplus_synoptic_macro_cell::get_width() ,(p_height - 1) * dicoplus_synoptic_macro_cell::get_height(true) + dicoplus_synoptic_macro_cell::get_height(false)),
    m_macro_cells(NULL),
    m_nb_cell_width(p_width),
    m_nb_cell_height(p_height)
      {
	unsigned int l_allocated_width = 0;
	unsigned int l_allocated_height = 0;
	std::string l_macro_cell_base_name = "synoptic_macro_cell_";
	try
	  {
	    // Creating macro_cell array
	    m_macro_cells = new dicoplus_synoptic_macro_cell** [m_nb_cell_width];
	    std::fill_n(m_macro_cells,m_nb_cell_width,static_cast<dicoplus_synoptic_macro_cell**>(NULL));

	    for(l_allocated_width = 0; l_allocated_width < m_nb_cell_width ; ++l_allocated_width)
	      {
		// Complete macro_cell name
		std::stringstream l_width_stream;
		l_width_stream << l_allocated_width;
		std::string l_macro_cell_base_width_name = l_macro_cell_base_name + l_width_stream.str();

		// Allocate macro_cell colum
		m_macro_cells[l_allocated_width] = new dicoplus_synoptic_macro_cell*[m_nb_cell_height];
		std::fill_n(m_macro_cells[l_allocated_width],m_nb_cell_height,static_cast<dicoplus_synoptic_macro_cell*>(NULL));

		for(l_allocated_height = 0; l_allocated_height < m_nb_cell_height ; ++l_allocated_height)
		  {
		    // Completing macro_cell name
		    std::stringstream l_height_stream;
		    l_height_stream << l_allocated_height;
		    std::string l_macro_cell_name = l_macro_cell_base_width_name+"_"+l_height_stream.str();

		    // Creating macro_cell
		    uint32_t l_x = dicoplus_synoptic_macro_cell::get_width() * l_allocated_width;
		    uint32_t l_y = dicoplus_synoptic_macro_cell::get_height(true) * l_allocated_height;
		    dicoplus_synoptic_macro_cell * l_macro_cell = new dicoplus_synoptic_macro_cell(p_owner,l_macro_cell_name.c_str(),m_nb_cell_height != l_allocated_height + 1);
                    add_zone(l_x,l_y,*l_macro_cell);
		    m_macro_cells[l_allocated_width][l_allocated_height] = l_macro_cell;
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
		    delete m_macro_cells[l_index_width][l_index_height];
		  }
		delete m_macro_cells[l_index_width];
	      }
	    for(unsigned int l_index_height = 0; l_index_height < l_allocated_height ; ++l_index_height)
	      {
		delete m_macro_cells[l_allocated_width][l_index_height];
	      }

	    delete[] m_macro_cells[l_allocated_width];
	    delete[] m_macro_cells;
	  }
      }

    //--------------------------------------------------------------------------
    dicoplus_synoptic_grid::~dicoplus_synoptic_grid(void)
      {
        for(unsigned int l_index = 0; l_index < m_nb_cell_width ; ++l_index)
          {
            for(unsigned int l_index2 = 0; l_index2 < m_nb_cell_height ; ++l_index2)
              {
                delete m_macro_cells[l_index][l_index2];
              }
            delete[] m_macro_cells[l_index];
          }        
        delete[] m_macro_cells;
      }
}
#endif // _DICOPLUS_SYNOPTIC_GRID_H_
//EOF
