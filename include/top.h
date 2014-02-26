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
               const std::string & p_config_file_name);
  private:
    inline void refresh_GUI(void);
    sc_clock m_clk;
    dicoplus m_dicoplus;
    synoptic::synoptic m_synoptic;
    dicoplus_synoptic_grid m_grid;
  };

  //------------------------------------------------------------------------------
  top::top(sc_module_name name,
	   const std::string & p_config_file_name):
    sc_module(name),
    m_clk("clk",10,SC_NS,05),
    m_dicoplus("dicoplus",p_config_file_name),
    m_synoptic(1550,850),
    m_grid((dicoplus_synoptic_cell::compute_dim(),dicoplus_char::init(),m_synoptic),"grid",m_dicoplus.get_width(),m_dicoplus.get_height())
    {
      // Bind clock
      m_dicoplus.m_clk(m_clk);

      // Manage GUI
      for(uint32_t l_x_index = 0 ; l_x_index < m_dicoplus.get_width() ; ++l_x_index)
	{
	  for(uint32_t l_y_index = 0 ; l_y_index < m_dicoplus.get_height() ; ++l_y_index)
	    {
	      m_dicoplus.attach_cell_listener(l_x_index,l_y_index,m_grid.get_cell(l_x_index,l_y_index));
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
    }
    
}
#endif // _TOP_H_
//EOF
