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

//! @name Clock Requirement Routines
//! @{

/*!
 * @brief Indicate that calling task requires HSE
 * 
 * @note This may require starting up the HSE
 */
void clk_mgr_req_hse();

/*!
 * @brief Indicate that HSE is no longer required
 * 
 * @note This may involve an HSI startup
 */
void clk_mgr_noreq_hse();

/*!
 * @brief Indicate that the calling task requires HSI
 * 
 * @note No clock will be started with this. HSI or HSE already going...
 */
void clk_mgr_req_hsi();

/*!
 * @brief Indicate that HSI is no longer required
 * 
 * Then clocks will shut down when idle
 */
void clk_mgr_noreq_hsi();
//! @}

#endif

#ifdef __cplusplus
}
#endif

#endif
