#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "hal_uart.h"
#include "hal_uart_mock.h"
#include "uart_exchange.h"

/** @brief UART 2 TX block size. Copied from _conf file */
#define kTX_BLOCK_SIZE (128)

/** @brief Time delay after we detached the exchange thread from main */
#define kTHREAD_DETACH_DELAY_US (10000)

/** @brief Time delay between RX and TX function processing */
#define kRX_TX_DELAY_US (100)

/** @brief Test return codes */
typedef enum
{
  /** @brief Test finished successfully */
  kTEST_OK,
  /** @brief Test finished with error */
  kTEST_ERR
} uart_exchange_test_res_t;

/**
 * @brief Special module for testing UART Exchange Task
 * @defgroup uart_exchange_test UART Exchange Test
 */

/**
 * @brief UART Exchange Task Test 1
 * @details Test runs uart_exchange() function in a standalone thread and simulates
 * interrupt callback from a different context
 * @param[in] is_verbose_debug If true then enables additional debug print
 * @return uart_exchange_test_res_t test result
 */
static uart_exchange_test_res_t test_1(bool is_verbose_debug);

/**
 * @brief UART Exchange Task Test 2
 * @details Test makes some simple checks for HAL UART module
 * @param[in] is_verbose_debug If true then enables additional debug print
 * @return uart_exchange_test_res_t test result
 */
static uart_exchange_test_res_t test_2(bool is_verbose_debug);

/**
 * @brief Thread for uart_exchange() function
 * @details Test makes some simple checks for HAL UART module
 * @param[in] args Input argument for a thread
 * @return NULL
 */
static void *uart_exchange_thread(void *args);

static void *uart_exchange_thread(void *args)
{
  (void)args;
  uart_exchange();
  return NULL;
}

static uart_exchange_test_res_t test_1(bool is_verbose_debug)
{
  pthread_t uart_exchange_thread_handle;

  // Let's create a thread for the exchange task to simulate different contexts
  int status = pthread_create(&uart_exchange_thread_handle, NULL, uart_exchange_thread, NULL);

  if (0 != status)
  {
    printf("Can't create uart_exchange_thread thread. Status: %d\r\n", status);
    return kTEST_ERR;
  }

  pthread_detach(uart_exchange_thread_handle);

  // Give some time to the exchange thread to init variables and callbacks
  usleep(kTHREAD_DETACH_DELAY_US);
  // Try to get RX callback function
  hal_uart_rx_cb saved_rx_cb = hal_uart_mock_get_rx_cb();

  if (NULL == saved_rx_cb)
  {
    return kTEST_ERR;
  };

  const char test_data[] =
      "    See pthread_self(3) for further information on the thread ID returned\
       in *thread by pthread_create().  Unless real-time scheduling policies\
       are being employed, after a call to pthread_create(), it is\
       indeterminate which thread—the caller or the new thread—will next\
       execute.\
\
       A thread may either be joinable or detached.  If a thread is\
       joinable, then another thread can call pthread_join(3) to wait for\
       the thread to terminate and fetch its exit status.  Only when a\
       terminated joinable thread has been joined are the last of its\
       resources released back to the system.  When a detached thread\
       terminates, its resources are automatically released back to the\
       system: it is not possible to join with the thread in order to obtain\
       its exit status.  Making a thread detached is useful for some types\
       of daemon threads whose exit status the application does not need to\
       care about.  By default, a new thread is created in a joinable state,\
       unless attr was set to create the thread in a detached state (using\
       pthread_attr_setdetachstate(3)).\
\
       Under the NPTL threading implementation, if the RLIMIT_STACK soft\
       resource limit at the time the program started has any value other\
       than \"unlimited\", then it determines the default stack size of new\
       threads.  Using pthread_attr_setstacksize(3), the stack size\
       attribute can be explicitly set in the attr argument used to create a\
       thread, in order to obtain a stack size other than the default.  If\
       the RLIMIT_STACK resource limit is set to \"unlimited\", a per-\
       architecture value is used for the stack size.  Here is the value for\
       a few architectures:";

  size_t test_data_size = ((sizeof(test_data) - 1) / kTX_BLOCK_SIZE) * kTX_BLOCK_SIZE;

  // For this test we need an addditional output buffer with the input buffer size
  char *test_data_from_tx = malloc(test_data_size);

  if (NULL == test_data_from_tx)
  {
    return kTEST_ERR;
  }

  // Set index offset for incoming data to 0
  size_t tx_data_offset = 0;

  for (size_t i = 0; i < test_data_size; i++)
  {
    // Send data using saved RX callback
    saved_rx_cb(test_data[i], NULL);
    // Make small delay for TX
    usleep(kRX_TX_DELAY_US);
    // Try to receive data 
    const char *tx_buf;
    size_t tx_buf_size = hal_uart_mock_get_tx_data(&tx_buf);

    // Process if we have any incoming data
    if (0 != tx_buf_size)
    {
      if (true == is_verbose_debug)
      {
        printf("%.*s\r\n", (int)tx_buf_size, tx_buf);
      }

      // Should end our test if we don't have enough memory for incoming data
      if (tx_data_offset + tx_buf_size > test_data_size)
      {
        free(test_data_from_tx);
        return kTEST_ERR;
      }

      // Save received data to allocated buffer
      strncpy(test_data_from_tx + tx_data_offset, tx_buf, tx_buf_size);
      // Update write pointer
      tx_data_offset += tx_buf_size;
    }
  }

  // Compare transmitted and received data
  int res = strncmp(test_data, test_data_from_tx, test_data_size);
  // Don't forget to free memory
  free(test_data_from_tx);

  if (0 != res)
  {
    return kTEST_ERR;
  }

  return kTEST_OK;
}

static uart_exchange_test_res_t test_2(bool is_verbose_debug)
{
  (void)is_verbose_debug;

  hal_uart_rc_t rc = hal_uart_init(NULL);

  if (kHAL_UART_RC_ERR_INPUT != rc)
  {
    return kTEST_ERR;
  }

  hal_uart_t uart;

  memset(&uart, 0, sizeof(uart));
  uart.baudrate = 99;

  rc = hal_uart_init(&uart);

  if (true == is_verbose_debug)
  {
    puts(hal_uart_stringify_rc(rc));
  }

  if (kHAL_UART_RC_ERR_INPUT != rc)
  {
    return kTEST_ERR;
  }

  memset(&uart, 0, sizeof(uart));
  uart.entity = 99;

  rc = hal_uart_init(&uart);
  if (true == is_verbose_debug)
  {
    puts(hal_uart_stringify_rc(rc));
  }

  if (kHAL_UART_RC_ERR_INPUT != rc)
  {
    return kTEST_ERR;
  }

  memset(&uart, 0, sizeof(uart));
  uart.parity_bit = 99;

  rc = hal_uart_init(&uart);
  if (true == is_verbose_debug)
  {
    puts(hal_uart_stringify_rc(rc));
  }

  if (kHAL_UART_RC_ERR_INPUT != rc)
  {
    return kTEST_ERR;
  }

  memset(&uart, 0, sizeof(uart));
  uart.stop_bits = 99;

  rc = hal_uart_init(&uart);
  if (true == is_verbose_debug)
  {
    puts(hal_uart_stringify_rc(rc));
  }

  if (kHAL_UART_RC_ERR_INPUT != rc)
  {
    return kTEST_ERR;
  }

  memset(&uart, 0, sizeof(uart));
  uart.rx_cb = NULL;

  rc = hal_uart_init(&uart);
  if (true == is_verbose_debug)
  {
    puts(hal_uart_stringify_rc(rc));
  }

  if (kHAL_UART_RC_OK != rc)
  {
    return kTEST_ERR;
  }

  memset(&uart, 0, sizeof(uart));
  uart.tx_cb = NULL;

  rc = hal_uart_init(&uart);
  if (true == is_verbose_debug)
  {
    puts(hal_uart_stringify_rc(rc));
  }

  if (kHAL_UART_RC_OK != rc)
  {
    return kTEST_ERR;
  }

  return kTEST_OK;
}

int main(void)
{
  printf("Test[%d] = %s\r\n", 1, (kTEST_OK == test_1(false)) ? "kTEST_OK" : "kTEST_ERR");
  printf("Test[%d] = %s\r\n", 2, (kTEST_OK == test_2(false)) ? "kTEST_OK" : "kTEST_ERR");
  return 0;
}
