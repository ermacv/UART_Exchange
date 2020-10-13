#include <stdbool.h>
#include <string.h>

#include "uart_exchange.h"
#include "uart_exchange_conf.h"

#include "hal_uart.h"

/** @brief RX UART half of the block size in bytes for UART_1 HW block*/
#define kUART_1_RX_BUF_HALF_SIZE (kBUF_BLOCK_SIZE)

/** @brief RX UART full block size in bytes for UART_1 HW block*/
#define kUART_1_RX_BUF_FULL_SIZE (2 * kUART_1_RX_BUF_HALF_SIZE)

/** @brief Structure that describes placeholder for shared data (callback and main task) */
typedef struct
{
  /**
   * @brief Buffer write offset (RX) 
   * @details It's supposed that read operation is atomic
   */
  size_t buf_offset;
  /** @brief RX/TX shared buffer */
  char buf[kUART_1_RX_BUF_FULL_SIZE];
  /**
   * @brief Flag for TX process
   * @details Set inside RX callback
   * @details It's supposed that read operation is atomic
   */
  bool need_to_send;
}uart_exchange_meta_t;

/** @brief Module shared data placeholder */
static volatile uart_exchange_meta_t uart_exchange_meta = {0};

/**
 * @brief Callback function for UART 1 RX
 * @param[in] data Input byte from UART 1 RX callback
 * @param[in/out] param User defined parameter for RX callback
 */
static void uart_1_rx_cb(char data, void *param);

static void uart_1_rx_cb(char data, void *param)
{
  (void)param;

  if (uart_exchange_meta.buf_offset >= kUART_1_RX_BUF_FULL_SIZE)
  {
    uart_exchange_meta.buf_offset = 0;
  }

  uart_exchange_meta.buf[uart_exchange_meta.buf_offset++] = data;

  if ((kUART_1_RX_BUF_HALF_SIZE == uart_exchange_meta.buf_offset) ||
      (kUART_1_RX_BUF_FULL_SIZE == uart_exchange_meta.buf_offset))
  {
    uart_exchange_meta.need_to_send = true;
  }
}

int uart_exchange(void)
{
  const hal_uart_t uart_1 = 
  {
    .baudrate = UART_9600_BAUD,
    .entity = UART_1,
    .parity_bit = UART_PARITY_BIT_ODD,
    .stop_bits = UART_1_0_STOP_BIT,
    .rx_cb = uart_1_rx_cb,
    .tx_cb = NULL
  };

  const hal_uart_t uart_2 = 
  {
    .baudrate = UART_115200_BAUD,
    .entity = UART_2,
    .parity_bit = UART_PARITY_BIT_ODD,
    .stop_bits = UART_1_0_STOP_BIT,
    .rx_cb = NULL,
    .tx_cb = NULL
  };

  // Enable UART 2 at first (because we won't transmit anything)
  hal_uart_enable(&uart_2);
  // Enable UART 1
  hal_uart_enable(&uart_1);

  while (true)
  {
    // If we have something to send
    if (true == uart_exchange_meta.need_to_send)
    {
      // Reset need_to_send flag
      uart_exchange_meta.need_to_send = false;
      size_t buf_offset = 0;

      if (uart_exchange_meta.buf_offset > kUART_1_RX_BUF_HALF_SIZE)
      {
        buf_offset += kUART_1_RX_BUF_HALF_SIZE;
      }

      hal_uart_send(&uart_2, (const char *)(uart_exchange_meta.buf + buf_offset), kUART_1_RX_BUF_HALF_SIZE);
    }
  }

  return 0;
}
