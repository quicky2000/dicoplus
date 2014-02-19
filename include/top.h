#ifndef _TOP_H_
#define _TOP_H_

#include "systemc.h"
#include "dicoplus.h"

namespace dicoplus
{
  class top:public sc_module
  {
  public:
    inline top(sc_module_name name,
               const std::string & p_config_file_name);
  private:
    sc_clock m_clk;
    dicoplus m_dicoplus;
  };

  //------------------------------------------------------------------------------
  top::top(sc_module_name name,
         const std::string & p_config_file_name):
  sc_module(name),
  m_clk("clk",10,SC_NS,05),
  m_dicoplus("dicoplus",p_config_file_name)    
    {
      m_dicoplus.m_clk(m_clk);
    }
}
#endif // _TOP_H_
//EOF
