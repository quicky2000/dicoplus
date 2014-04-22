#ifndef _TOP_H_
#define _TOP_H_

#include "systemc.h"
#include "dicoplus.h"
#include "synoptic.h"
#include "dicoplus_synoptic_grid.h"
#include "dicoplus_char.h"
#include "screen_display.h"

namespace dicoplus
{
  class top:public sc_module
  {
  public:
    SC_HAS_PROCESS(top);
    inline top(sc_module_name name,
               const std::string & p_config_file_name,
	       const uint32_t & p_refresh_delay);
  private:
    inline void refresh_GUI(void);
    sc_clock m_clk;
    dicoplus m_dicoplus;
    synoptic::synoptic m_synoptic;
    dicoplus_synoptic_grid m_grid;
    const uint32_t m_refresh_delay;
  };

  //------------------------------------------------------------------------------
  top::top(sc_module_name name,
	   const std::string & p_config_file_name,
	   const uint32_t & p_refresh_delay):
    sc_module(name),
    m_clk("clk",10,SC_NS,0.5),
    m_dicoplus("dicoplus",p_config_file_name),
    m_synoptic(1550,850),
    m_grid(m_synoptic,"grid",m_dicoplus.get_width(),m_dicoplus.get_height()),
    m_refresh_delay(p_refresh_delay)
    {
      // Bind clock
      m_dicoplus.m_clk(m_clk);

      // Manage GUI
      for(uint32_t l_x_index = 0 ; l_x_index < m_dicoplus.get_width() ; ++l_x_index)
	{
	  for(uint32_t l_y_index = 0 ; l_y_index < m_dicoplus.get_height() ; ++l_y_index)
	    {
	      m_dicoplus.attach_cell_listener(l_x_index,l_y_index,m_grid.get_cell(l_x_index,l_y_index));
	      m_dicoplus.attach_global_bus_listener(l_x_index,l_y_index,m_grid.get_global_bus(l_x_index,l_y_index));
	      if(l_x_index) m_dicoplus.attach_local_bus_listener(l_x_index,l_y_index,m_grid.get_east_listener(l_x_index - 1,l_y_index));
	      if(l_y_index) m_dicoplus.attach_local_bus_listener(l_x_index,l_y_index,m_grid.get_south_listener(l_x_index,l_y_index - 1));
	      if(l_x_index + 2 < m_dicoplus.get_width())m_dicoplus.attach_local_bus_listener(l_x_index,l_y_index,m_grid.get_west_listener(l_x_index + 1,l_y_index));
	      if(l_y_index + 2 < m_dicoplus.get_height())m_dicoplus.attach_local_bus_listener(l_x_index,l_y_index,m_grid.get_north_listener(l_x_index,l_y_index + 1));
	    }
	}

      m_synoptic.add_zone(642,0,m_grid);
      screen_display l_screen_display(m_synoptic);
      m_synoptic.add_zone(0,0,l_screen_display);
      m_synoptic.pack();
      m_synoptic.paint();
      m_synoptic.refresh();

      SC_METHOD(refresh_GUI);
      sensitive << m_clk.negedge_event();
    }

    //------------------------------------------------------------------------------
    void top::refresh_GUI(void)
    {
      m_synoptic.refresh();
      sleep(m_refresh_delay);
    }
    
}
#endif // _TOP_H_
//EOF
