#ifndef __IMU_CLK_MGR_H_
#define __IMU_CLK_MGR_H_



#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t hse_sem, hsi_sem;


#if !USE_CLK_MGR

#define clk_mgr_init()      {}

/*!
 @brief Wake up the HSI with PLL
 
 @pre Only LSI/LSE enabled
 */
#define clk_mgr_wakeup()    {}
#define clk_mgr_gosleep()   {}
#define clk_mgr_req_hse()   {}
#define clk_mgr_noreq_hse() {}
#define clk_mgr_req_hsi()   {}
#define clk_mgr_noreq_hsi() {}

#else

void clk_mgr_init();

/*!
 @brief Wake up the HSI with PLL
 
 @pre Only LSI/LSE enabled
 */
void clk_mgr_wakeup();
void clk_mgr_gosleep();
void clk_mgr_req_hse();
void clk_mgr_noreq_hse();
void clk_mgr_req_hsi();
void clk_mgr_noreq_hsi();

#endif

#ifdef __cplusplus
}
#endif

#endif
