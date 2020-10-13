#include <stdio.h>
#include <stddef.h>

#include "hal_uart_mock.h"

/** @brief Placeholder for UART 1 RX callback */
static hal_uart_rx_cb saved_rx_cb = NULL;

/** @brief Placeholder for UART 2 TX buffer */
static const char *saved_tx_buf = NULL;

/** @brief Placeholder for UART 2 TX buffer size */
static size_t saved_tx_buf_size = 0;

hal_uart_rx_cb hal_uart_mock_get_rx_cb(void)
{
  return saved_rx_cb;
}

size_t hal_uart_mock_get_tx_data(const char **tx_buf)
{
  if (NULL == tx_buf)
  {
    return 0;
  }

  *tx_buf = (const char*)saved_tx_buf;
  size_t curr_saved_tx_buf_size = saved_tx_buf_size;
  saved_tx_buf_size = 0;

  return curr_saved_tx_buf_size;
}

hal_uart_rc_t hal_uart_init(const hal_uart_t *uart)
{
  if (NULL == uart)
  {
    return kHAL_UART_RC_ERR_INPUT;
  }

  if ((UART_1_0_STOP_BIT != uart->stop_bits) &&
      (UART_1_5_STOP_BIT != uart->stop_bits) &&
      (UART_2_0_STOP_BIT != uart->stop_bits))
  {
    return kHAL_UART_RC_ERR_INPUT;
  }

  if ((UART_115200_BAUD != uart->baudrate) && (UART_9600_BAUD != uart->baudrate))
  {
    return kHAL_UART_RC_ERR_INPUT;
  }

  if ((UART_PARITY_BIT_ODD != uart->parity_bit) &&
      (UART_PARITY_BIT_EVEN != uart->parity_bit) &&
      (UART_PARITY_BIT_DISABLED != uart->parity_bit))
  {
    return kHAL_UART_RC_ERR_INPUT;
  }

  if ((UART_1 != uart->entity) &&
      (UART_2 != uart->entity) &&
      (UART_3 != uart->entity))
  {
    return kHAL_UART_RC_ERR_INPUT;
  }

  return kHAL_UART_RC_OK;
}

hal_uart_rc_t hal_uart_deinit(const hal_uart_t *uart)
{
  if (NULL == uart)
  {
    return kHAL_UART_RC_ERR_INPUT;
  }

  return kHAL_UART_RC_OK;
}

hal_uart_rc_t hal_uart_enable(const hal_uart_t *uart)
{
  if (NULL == uart)
  {
    return kHAL_UART_RC_ERR_INPUT;
  }

  if (UART_1 == uart->entity)
  {
    saved_rx_cb = uart->rx_cb;
  }

  return kHAL_UART_RC_OK;
}

hal_uart_rc_t hal_uart_disable(const hal_uart_t *uart)
{
  if (NULL == uart)
  {
    return kHAL_UART_RC_ERR_INPUT;
  }

  return kHAL_UART_RC_OK;
}

hal_uart_rc_t hal_uart_send(const hal_uart_t *uart, const char *buf, size_t buf_size)
{
  if ((NULL == uart) || (NULL == buf) || (0 == buf_size))
  {
    return kHAL_UART_RC_ERR_INPUT;
  }

  saved_tx_buf = buf;
  saved_tx_buf_size = buf_size;

  return kHAL_UART_RC_OK;
}

const char *hal_uart_stringify_rc(hal_uart_rc_t rc)
{
  const char *rc_str;
  switch (rc)
  {
    case kHAL_UART_RC_OK:
      rc_str = "kHAL_UART_RC_OK";
      break;
    case kHAL_UART_RC_ERR_INPUT:
      rc_str = "kHAL_UART_RC_ERR_INPUT";
      break;
    case kHAL_UART_RC_ERR_INTERNAL:
      rc_str = "kHAL_UART_RC_ERR_INTERNAL";
      break;
    default:
    // No break
    case kHAL_UART_RC_UNKNOWN:
      rc_str = "kHAL_UART_RC_UNKNOWN";
      break;
  }

  return rc_str;
}
