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
#ifndef _DICOPLUS_H_
#define _DICOPLUS_H_

#include "quicky_exception.h"
#include "xmlParser.h"
#include "utf8.h"

#include "dicoplus_global_bus.h"
#include "dicoplus_global_bus_probe.h"

#include "dicoplus_macro_cell.h"
#include "dicoplus_local_output_port.h"
#include "dicoplus_local_bus.h"
#include "dicoplus_injector.h"

#include "dicoplus_char.h"
#include "systemc.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <inttypes.h>
#include <queue>

namespace dicoplus
{
  class cell_listener_if;
  class dicoplus:public sc_module
  {
  public:
    SC_HAS_PROCESS(dicoplus);
    inline dicoplus(sc_module_name p_name,
                    const std::string & p_input_file);
    inline ~dicoplus(void);
    sc_in<bool> m_clk;
    inline const unsigned int & get_width(void)const;
    inline const unsigned int & get_height(void)const;
    inline void attach_cell_listener(const uint32_t & p_x,
				     const uint32_t & p_y,
				     cell_listener_if & p_listener);
    inline void attach_global_bus_listener(const uint32_t & p_x,
                                           const uint32_t & p_y,
                                           dicoplus_global_bus_listener_if & p_listener);
  private:
    inline void clk_management(void);

    sc_signal<bool> m_clk_sig;
    dicoplus_local_output_port m_fake_output_port;
    dicoplus_local_bus m_fake_output_bus;
    std::vector<std::string> m_word_list;
    unsigned int m_width;
    unsigned int m_height;
    dicoplus_injector m_injector;
    dicoplus_macro_cell *** m_macro_cells;
    dicoplus_global_bus * m_injector_global_bus;
  };

  //----------------------------------------------------------------------------
  dicoplus::dicoplus(sc_module_name p_name,
                     const std::string & p_input_file):
    sc_module(p_name),
    m_clk("clk"),
    m_clk_sig("clk_sig"),
    m_fake_output_port("fake_output_port"),
    m_fake_output_bus("fake_output_bus"),
    m_width(0),
    m_height(0),
    m_injector("injector"),
    m_macro_cells(NULL),
    m_injector_global_bus(NULL)
      {

        // Check file existence
        std::ifstream l_input_file;
        l_input_file.open(p_input_file.c_str());
        if(NULL == l_input_file)
          {
            throw quicky_exception::quicky_runtime_exception("Unable to open file \"" + p_input_file + "\" for read",__LINE__,__FILE__);
          }
        l_input_file.close();

        // Parse file
        XMLResults l_err= {eXMLErrorNone,0,0};
        XMLNode l_node = XMLNode::parseFile(p_input_file.c_str(),"dicoplus",&l_err);
        std::string l_error_msg = XMLNode::getError(l_err.error);
      
        if(l_error_msg != "No error")
          {
	    std::stringstream l_line_number ;
	    l_line_number << l_err.nLine;
	    std::stringstream l_column_number ;
	    l_column_number << l_err.nColumn;

            l_error_msg = "dom_parser : \"" + l_error_msg+ "\" on line " + l_line_number.str() + " when parsing XML column " + l_column_number.str();
            throw quicky_exception::quicky_logic_exception("dom_parser : \"" + l_error_msg+ "\" on line " + l_line_number.str() + " when parsing XML column " + l_column_number.str(),__LINE__,__FILE__);
          }

	std::queue<dicoplus_types::t_global_data_type> & l_grid_content = * new std::queue<dicoplus_types::t_global_data_type>();
	std::vector<dicoplus_types::t_global_data_type> & l_word_list = * new std::vector<dicoplus_types::t_global_data_type>();

        // Extract data from XML tree
        int l_nb_child_object = l_node.nChildNode();
        for(int l_index = 0 ; l_index < l_nb_child_object ; ++l_index)
          {
            const XMLNode & l_child_node = l_node.getChildNode(l_index);
            int l_nb_attribute = l_child_node.nAttribute();
            std::string l_node_type = l_child_node.getName();
            if(1 != l_nb_attribute)
              {
                std::stringstream l_stream;
                l_stream << l_nb_attribute;
                throw quicky_exception::quicky_logic_exception("Attribute \""+l_node_type+"\" has "+l_stream.str()+" instead of 1",__LINE__,__FILE__);
              }

            XMLCSTR l_key_str = l_child_node.getAttribute("content");
            if(NULL == l_key_str)
              {
                throw quicky_exception::quicky_logic_exception("Mandatory attribute \"content\" is missing in node \"" + std::string(l_node_type) + "\"",__LINE__,__FILE__);
              }
            if("word" == l_node_type)
              {
                m_word_list.push_back(l_key_str);
		
                std::string l_key = l_key_str;
                try
                  {
                    // Extract word content and convert it to internal representation of dicoplus
                    std::string::const_iterator l_word_iter = l_key.begin();
                    std::string::const_iterator l_word_iter_end = l_key.end();
                    while(l_word_iter != l_word_iter_end)
                      {
                        uint32_t l_cp = utf8::next(l_word_iter,l_word_iter_end);
                        l_word_list.push_back(dicoplus_char::get_internal_code(l_cp));
                      }			       
                    // Adding separator ath the end of word
                    l_word_list.push_back(dicoplus_char::get_internal_code(0xc6));
                  }
                catch(quicky_exception::quicky_logic_exception e)
                  {
                    throw quicky_exception::quicky_logic_exception("Problem with word \""+l_key+"\" : "+e.what(),__LINE__,__FILE__);
                  }
              }
            else if("line" == l_node_type)
              {
                ++m_height;
                std::string l_key = l_key_str;
                unsigned int l_size = utf8::distance(l_key.begin(),l_key.end());
                if(m_width != l_size)
                  {
                    if(m_width)
                      {
                        std::stringstream l_width_stream;
                        l_width_stream << m_width;
                        std::stringstream l_height_stream;
                        l_height_stream << m_height;
                        std::stringstream l_key_size_stream;
                        l_key_size_stream << l_size;
                        throw quicky_exception::quicky_logic_exception("Node \"line\" number "+l_height_stream.str()+" has a width ("+l_key_size_stream.str()+") different from previous ("+ l_width_stream.str()+")",__LINE__,__FILE__);
                      }
                    else
                      {
                        m_width = l_size;
                      }

                  }
		// Extract line content and convert it to internal representation of dicoplus
                try
                  {
                    std::string::const_iterator l_line_iter = l_key.begin();
                    std::string::const_iterator l_line_iter_end = l_key.end();
                    while(l_line_iter != l_line_iter_end)
                      {
                        uint32_t l_cp = utf8::next(l_line_iter,l_line_iter_end);
                        l_grid_content.push(dicoplus_char::get_internal_code(l_cp));
                      }
                  }
                catch(quicky_exception::quicky_logic_exception e)
                  {
                    throw quicky_exception::quicky_logic_exception("Problem with line \""+l_key+"\" : "+e.what(),__LINE__,__FILE__);
                  }

              }
            else
              {
                throw quicky_exception::quicky_logic_exception("Unkown node type \""+std::string(l_node_type)+"\". It should be \"word\" or \"line\"",__LINE__,__FILE__);
              }
  
          }

        // Printing list of words
        for(std::vector<std::string>::const_iterator l_iter = m_word_list.begin() ; 
            l_iter != m_word_list.end();
            ++l_iter)
          {
            std::cout << "-> " << *l_iter << " => " << l_iter->size() << " vs " << utf8::distance(l_iter->begin(),l_iter->end()) << std::endl ;
          }
        std::cout << "Dimensions : " << m_width << " * " << m_height << std::endl ;

        std::string l_last = m_word_list[0/*m_word_list.size() -5*/];
        std::cout << "Len = \"" << l_last.size() << " of \"" << l_last << "\"" << std::endl ;
        std::cout << std::endl ;
        std::string::const_iterator l_toto = l_last.begin();
        std::string::const_iterator l_toto_end = l_last.end();
        int l_nb = 0;
        while(l_toto != l_toto_end)
          {
            char l_char_array[5] = {0x0,0x0,0x0,0x0,0x0};
            uint32_t l_cp = utf8::next(l_toto,l_toto_end);
            utf8::append(l_cp,l_char_array);
            std::cout << l_nb << " : " << std::hex << "Codepoint : 0x" << l_cp << std::dec << " -> '" << l_char_array << "'" << std::endl;
            ++l_nb;
          }
        std::cout << std::endl ;


	m_injector.set_grid_content(l_grid_content);
	m_injector.set_word_list(l_word_list);

	// Instantiate Cell grid
	unsigned int l_allocated_width = 0;
	unsigned int l_allocated_height = 0;
	std::string l_cell_base_name = "Cell_";
	try
	  {
	    // Creating macrocell array
	    m_macro_cells = new dicoplus_macro_cell** [m_width];
	    std::fill_n(m_macro_cells,m_width,static_cast<dicoplus_macro_cell**>(NULL));
	    
	    
	    
	    for(l_allocated_width = 0; l_allocated_width < m_width ; ++l_allocated_width)
	      {
		// Complete cell name
		std::stringstream l_width_stream;
		l_width_stream << l_allocated_width;
		std::string l_cell_base_width_name = l_width_stream.str();

		// Allocate macrocell colum
		m_macro_cells[l_allocated_width] = new dicoplus_macro_cell*[m_height];
		std::fill_n(m_macro_cells[l_allocated_width],m_height,static_cast<dicoplus_macro_cell*>(NULL));



		for(l_allocated_height = 0; l_allocated_height < m_height ; ++l_allocated_height)
		  {
		    // Completing cell name
		    std::stringstream l_height_stream;
		    l_height_stream << l_allocated_height;
		    std::string l_cell_suffix_name = l_cell_base_width_name+"_"+l_height_stream.str();



		    // Creating macro_cell
		    dicoplus_macro_cell * l_macro_cell = new dicoplus_macro_cell(l_cell_suffix_name.c_str(),m_clk_sig);
		    // Storing cell in array for deletion
		    m_macro_cells[l_allocated_width][l_allocated_height] = l_macro_cell;
		  }
	      }
	  }
	catch ( const std::bad_alloc & ) // Allocation error
	  {
	    // Free partially allocated dmemory
	    for (unsigned int l_index_width = 0; l_index_width < l_allocated_width - 1; ++l_index_width) 
	      {
		for(unsigned int l_index_height = 0; l_index_height < m_height ; ++l_index_height)
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

 	std::string l_previous_name = "Injector";
 	dicoplus_global_port_binding_if * l_previous_global = &m_injector;
        for(unsigned int l_index_height = 0; l_index_height < m_height ; ++l_index_height)
          {
            for (unsigned int l_index_width = 0; l_index_width < m_width ; ++l_index_width) 
              {
		// Complete cell name
                dicoplus_macro_cell * l_macro_cell = m_macro_cells[l_index_width][l_index_height];

                // Creating bus
                std::string l_bus_name = "FROM_" + l_previous_name +"_TO_" + l_macro_cell->get_name();
                dicoplus_global_bus * l_bus = new dicoplus_global_bus(l_bus_name.c_str());
  
                // Binding with previous cell
		l_previous_global->bind_output_port(*l_bus);
	
		l_macro_cell->bind_input_port(*l_bus);

                l_previous_global = l_macro_cell;
		l_previous_name = l_macro_cell->get_name();
              }
          }

	// Bind injector
	m_injector.m_clk(m_clk_sig);
	m_injector_global_bus = new dicoplus_global_bus("FROM_" + l_previous_name +"_TO_injector");
	l_previous_global->bind_output_port(*m_injector_global_bus);
	m_injector.bind_input_port(*m_injector_global_bus);

	if(m_height < 1) throw quicky_exception::quicky_logic_exception("Grid height is less than 1 !",__LINE__,__FILE__);

	// Bind spy bus of injector
	m_injector.spy_bus(m_clk_sig,m_macro_cells[0][1]->get_global_bus());

	// Bind fake output port
	m_fake_output_port(m_fake_output_bus);

	// Bind north port of first line and south port of bottom line
	for(unsigned int l_index = 0 ; l_index < m_width ; ++l_index)
	  {
	    m_macro_cells[l_index][0]->bind_north_port(m_fake_output_bus);
	    m_macro_cells[l_index][m_height - 1]->bind_south_port(m_fake_output_bus);
	  }
        
	for(unsigned int l_x_index = 0; l_x_index < m_width ; ++l_x_index)
	  {
	    for(unsigned int l_y_index = 1; l_y_index < m_height ; ++l_y_index)
	      {
		m_macro_cells[l_x_index][l_y_index]->bind_north_port(m_macro_cells[l_x_index][l_y_index - 1]->get_output_bus());
                m_macro_cells[l_x_index][l_y_index - 1]->bind_south_port(m_macro_cells[l_x_index][l_y_index]->get_output_bus());
	      }
	  }

	// Bind west port of first column and south port of right column
	for(unsigned int l_index = 0 ; l_index < m_height ; ++l_index)
	  {
	    m_macro_cells[0][l_index]->bind_west_port(m_fake_output_bus);
	    m_macro_cells[m_width - 1][l_index]->bind_east_port(m_fake_output_bus);
	  }
        
	for(unsigned int l_x_index = 1; l_x_index < m_width ; ++l_x_index)
	  {
	    for(unsigned int l_y_index = 0; l_y_index < m_height ; ++l_y_index)
	      {
		m_macro_cells[l_x_index][l_y_index]->bind_west_port(m_macro_cells[l_x_index - 1][l_y_index]->get_output_bus());
                m_macro_cells[l_x_index - 1][l_y_index]->bind_east_port(m_macro_cells[l_x_index][l_y_index]->get_output_bus());
	      }
	  }

        SC_METHOD(clk_management);
        sensitive << m_clk;
      }

    //--------------------------------------------------------------------------
    dicoplus::~dicoplus(void)
      {
        for(unsigned int l_index = 0; l_index < m_width ; ++l_index)
          {
            for(unsigned int l_index2 = 0; l_index2 < m_height ; ++l_index2)
              {
                delete m_macro_cells[l_index][l_index2];
              }
            delete[] m_macro_cells[l_index];
          }        
        delete[] m_macro_cells;
	delete m_injector_global_bus;
    

        
      }

    //--------------------------------------------------------------------------
    void dicoplus::attach_cell_listener(const uint32_t & p_x,
					const uint32_t & p_y,
					cell_listener_if & p_listener)
    {
      
      if(p_x >= m_width)
	{
	  std::stringstream l_stream;
	  l_stream << "Value " << p_x << " >= to dicoplus width " << m_width;
	  throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	}
      if(p_y >= m_height)
	{
	  std::stringstream l_stream;
	  l_stream << "Value " << p_y << " >= to dicoplus height " << m_height;
	  throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	}
      m_macro_cells[p_x][p_y]->attach_cell_listener(p_listener);
    }

    //--------------------------------------------------------------------------
    void dicoplus::attach_global_bus_listener(const uint32_t & p_x,
                                              const uint32_t & p_y,
                                              dicoplus_global_bus_listener_if & p_listener)
    {
      if(p_x >= m_width)
	{
	  std::stringstream l_stream;
	  l_stream << "Value " << p_x << " >= to dicoplus width " << m_width;
	  throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	}
      if(p_y >= m_height)
	{
	  std::stringstream l_stream;
	  l_stream << "Value " << p_y << " >= to dicoplus height " << m_height;
	  throw quicky_exception::quicky_logic_exception(l_stream.str(),__LINE__,__FILE__);
	}
      std::stringstream l_x_str;
      l_x_str << p_x;
      std::stringstream l_y_str;
      l_y_str << p_y;
      std::string l_probe_name = "probe_bus_from_cell_" + l_x_str.str() + "_" + l_y_str.str();
      dicoplus_global_bus_probe * l_probe = new dicoplus_global_bus_probe(l_probe_name.c_str(),p_listener);
      l_probe->m_clock(m_clk_sig);

      m_macro_cells[p_x][p_y]->attach_global_bus_listener(*l_probe);
    }

    //--------------------------------------------------------------------------
    void dicoplus::clk_management(void)
    {
      m_clk_sig.write(m_clk.read());
    }

    //--------------------------------------------------------------------------
    const unsigned int & dicoplus::get_width(void)const
      {
	return m_width;
      }

    //--------------------------------------------------------------------------
    const unsigned int & dicoplus::get_height(void)const
      {
	return m_height;
      }

}

#endif // _DICOPLUS_H_
//EOF
