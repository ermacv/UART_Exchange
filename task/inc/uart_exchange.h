#ifndef UART_EXCHANGE_H
#define UART_EXCHANGE_H

/**
 * @brief UART Exchange task that should be called in a loop
 * @defgroup uart_exchange_task UART Exchange Task
 * @{
 */

/**
 * @brief Process UART Exchange Task 
 * @details uart must be inited before using this function
 * @return 0 on success, non-zero value otherwise
 */
int uart_exchange(void);

#endif // UART_EXCHANGE_H

/** @} */
