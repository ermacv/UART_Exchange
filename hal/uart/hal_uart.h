#ifndef HAL_UART_H
#define HAL_UART_H

#include <stddef.h>

/**
 * @brief Module is used to perform serial communication
 * @addtogroup hal_uart HAL UART
 * @{
 */

/** @brief Available UART baudrates */
typedef enum
{
  /** @brief 9600 Baudrate */
  UART_9600_BAUD = 0,
  /** @brief 115200 Baudrate */
  UART_115200_BAUD
} hal_uart_baudrate_t;

/** @brief Available UART stop bit duration */
typedef enum
{
  /** @brief 1 stop bit */
  UART_1_0_STOP_BIT = 0,
  /** @brief 1.5 stop bit */
  UART_1_5_STOP_BIT,
  /** @brief 2 stop bits */
  UART_2_0_STOP_BIT
} hal_uart_stop_bits_t;

/** @brief Available UART parity control bit */
typedef enum
{
  /** @brief Odd parity bit */
  UART_PARITY_BIT_ODD = 0,
  /** @brief Even parity bit */
  UART_PARITY_BIT_EVEN,
  /** @brief Parity bit disabled */
  UART_PARITY_BIT_DISABLED
} hal_uart_parity_bit_t;

/** @brief Available UART HW blocks */
typedef enum
{
  /** @brief UART 1 HW block */
  UART_1 = 0,
  /** @brief UART 2 HW block */
  UART_2,
  /** @brief UART 3 HW block */
  UART_3
} hal_uart_entity_t;

/** @brief UART RX callback function prototype */
typedef void (*hal_uart_rx_cb)(char, void *);

/** @brief UART TX callback function prototype */
typedef void (*hal_uart_tx_cb)(void *);

/** @brief Describes current UART properties */
typedef struct
{
  /** @brief UART entity stop bits */
  hal_uart_stop_bits_t stop_bits;
  /** @brief UART entity baudrate */
  hal_uart_baudrate_t baudrate;
  /** @brief UART entity parity bit */
  hal_uart_parity_bit_t parity_bit;
  /** @brief Chosen UART entity */
  hal_uart_entity_t entity;
  /**
   * @brief UART entity TX callback 
   * @details Could be NULL if it's not needed
   */
  hal_uart_rx_cb rx_cb;
  /**
   * @brief UART entity RX callback 
   * @details Could be NULL if it's not needed
   */
  hal_uart_tx_cb tx_cb;
} hal_uart_t;

/** @brief Return codes for HAL UART module */
typedef enum
{
  /** @brief Function successfully executed */
  kHAL_UART_RC_OK = 0,
  /** @brief There is an error related to input parameters */
  kHAL_UART_RC_ERR_INPUT,
  /** @brief There is an internal error */
  kHAL_UART_RC_ERR_INTERNAL,
  /** @brief Special return code for stringy function only */
  kHAL_UART_RC_UNKNOWN
} hal_uart_rc_t;

/**
 * @brief Init UART HW block using input uart parameter
 * @details uart entity fields should be set by the user
 * @param[in] uart UART entity
 * @return hal_uart_rc_t
 */
hal_uart_rc_t hal_uart_init(const hal_uart_t *uart);

/**
 * @brief Deinit UART HW block and free internal resources
 * @details uart must be inited before using this function
 * @param[in] uart UART entity
 * @return hal_uart_rc_t
 */
hal_uart_rc_t hal_uart_deinit(const hal_uart_t *uart);

/**
 * @brief Enable UART HW block
 * @details uart must be inited before using this function
 * @param[in] uart UART entity
 * @return hal_uart_rc_t
 */
hal_uart_rc_t hal_uart_enable(const hal_uart_t *uart);

/**
 * @brief Disable UART HW block
 * @details uart must be inited before using this function
 * @param[in] uart UART entity
 * @return hal_uart_rc_t
 */
hal_uart_rc_t hal_uart_disable(const hal_uart_t *uart);

/**
 * @brief Send buffer using UART 
 * @details uart must be inited before using this function
 * @param[in] uart UART entity
 * @param[in] buf Input buffer that should be sent
 * @param[in] buf_size Input buffer size
 * @return hal_uart_rc_t
 */
hal_uart_rc_t hal_uart_send(const hal_uart_t *uart, const char *buf, size_t buf_size);

#endif // HAL_UART_H

/** @} */
