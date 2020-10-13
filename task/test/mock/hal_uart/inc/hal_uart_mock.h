#ifndef HAL_UART_MOCK_H
#define HAL_UART_MOCK_H

#include "hal_uart.h"

/**
 * @brief Test task that should be called in a loop
 * @defgroup uart_exchange_task UART Exchange Task
 * @{
 */

/**
 * @brief Special test function that returns an address of the current UART_1 RX callback
 * @return NULL if there is no UART 1 RX CB funtion, non NULL value otherwise
 */
hal_uart_rx_cb hal_uart_mock_get_rx_cb(void);

/**
 * @brief Special test function that returns data from UART_2 TX
 * @param[out] tx_buf Pointer to the output buffer
 * @return 0 if there is no output data, non 0 value otherwise
 */
size_t hal_uart_mock_get_tx_data(const char **tx_buf);

/**
 * @brief Converts input HAL UART return code to the string
 * @param[in] rc HAL UART return code
 * @return Pointer to stringified rc
 */
const char *hal_uart_stringify_rc(hal_uart_rc_t rc);

#endif // HAL_UART_MOCK_H

/** @} */
