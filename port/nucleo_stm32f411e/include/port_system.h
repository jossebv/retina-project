/**
 * @file port_system.h
 * @brief Header for port_system.c file.
 * @author Jose Manuel Bravo Pacheco
 * @date 04/05/2023
 */

#ifndef PORT_SYSTEM_H_
#define PORT_SYSTEM_H_

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* HW dependent includes */
#include "stm32f411e_discovery.h"
#include "picovoice.h"

/* Defines and enums ----------------------------------------------------------*/
/* Defines */
#define BIT_POS_TO_MASK(x) (0x01 << x)  /*!< Convert the index of a bit into a mask by left shifting */
#define BASE_MASK_TO_POS(m, p) (m << p) /*!< Move a mask defined in the LSBs to upper positions by shifting left p bits */
#define GET_PIN_IRQN(pin) (pin >= 10 ? EXTI15_10_IRQn : (pin >= 5 ? EXTI9_5_IRQn : (EXTI0_IRQn + pin))) /*!< Compute the IRQ number associated to a GPIO pin */
#define TRIGGER_RISING_EDGE 0x1                     /*!< Mask for rising edge trigger*/
#define TRIGGER_FALLING_EDGE 0x2                    /*!< Mask for falling edge trigger*/
#define TRIGGER_BOTH_EDGE 0x3                       /*!< Mask for both edge trigger*/
#define TRIGGER_ENABLE_EVENT_REQ 0x4                /*!< Mask for enabling event trigger*/
#define TRIGGER_ENABLE_INTERR_REQ 0x8               /*!< Mask for enabling interruption trigger*/
#define TRIGGER_ENABLE_EVENT_AND_INTERR_REQ 0xc     /*!< Mask for enabling both event/interruption triggers*/
#define BASE_ALTERNATE_MASK 0x0F                    /*!< Mask for alternate mode*/

#define BASE_MODER_MASK 0x03 /*!< Define MODER mask*/
#define BASE_PUPDR_MASK 0x03 /*!< Define PUPDR mask*/
#define BASE_EXTIX_MASK 0x0F /*!< Define EXTIX mask*/

/* Microcontroller STM32F446RE */
/* Timer configuration */
#define RCC_HSI_CALIBRATION_DEFAULT 0x10U            /*!< Default HSI calibration trimming value */
#define TICK_FREQ_1KHZ 1U                            /*!< Freqency in kHz of the System tick */
#define NVIC_PRIORITY_GROUP_0 ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority, \
                                                         4 bits for subpriority */
#define NVIC_PRIORITY_GROUP_4 ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority, \
                                                         0 bit  for subpriority */

/* Power */
#define POWER_REGULATOR_VOLTAGE_SCALE3 0x01 /*!< Scale 3 mode: the maximum value of fHCLK is 120 MHz. */

/* GPIOs */
#define HIGH true /*!< Logic 1 */
#define LOW false /*!< Logic 0 */

#define GPIO_MODE_IN 0x00        /*!< GPIO as input */
#define GPIO_MODE_OUT 0x01       /*!< GPIO as output */
#define GPIO_MODE_ALTERNATE 0x02 /*!< GPIO as alternate function */

#define GPIO_PUPDR_NOPULL 0x00 /*!< GPIO no pull up or down */
#define GPIO_PUPDR_PUP 0x01    /*!< GPIO no pull up */
#define GPIO_PUPDR_PDOWN 0x02  /*!< GPIO no pull down */

/* UUID */
#define UUID_ADDRESS (0x1FFF7A10)   /*!< UUID address*/
#define UUID_SIZE (12)              /*!< UUID size in bytes*/

/* SYSCLK FREQ */
#define SYSCLOCK_FREQ 100000000     /*!<SYSCLOCK frequency*/

/* Function prototypes and explanation -------------------------------------------------*/

/**
 * @brief  This function is based on the initialization of the HAL Library; it must be the first
 *         thing to be executed in the main program (before to call any other
 *          functions), it performs the following:
 *           - Configure the Flash prefetch, instruction and Data caches.
 *           - Configures the SysTick to generate an interrupt each 1 millisecond, which is clocked by the HSI (at this stage, the clock is not yet configured and thus the system is running from the internal HSI at 16 MHz).
 *           - Set NVIC Group Priority to 4.
 *             NVIC_PRIORITYGROUP_4: 4 bits for preemption priority
 *                                    0 bits for subpriority
 *           - Configure the system clock
 *
 * @note   SysTick is used as time base for the delay functions. When using the HAL, the application
 *         needs to ensure that the SysTick time base is always set to 1 millisecond
 *         to have correct HAL operation.
 *    When the NVIC_PRIORITYGROUP_0 is selected, IRQ preemption is no more possible.
 *         The pending IRQ priority will be managed only by the subpriority.
 * @retval Init status
 */
pv_status_t port_system_init(void);

/**
 * @brief Enable low power consumption in sleep mode. 
*/
void port_system_sleep();

/**
 * @brief Set the system in stop mode for low power consumption.

    The function sets the state of the power regulator in stop mode.
    After that, the function sets the system mode in sleep mode, to enter in stop mode when calling _WFI() (wait for interruption).
    The system remains in this line until an external interruption or timer interruption occurs.
    If an interruption occurs the system wakes up and resets the system mode.
*/
void port_system_power_stop();

/**
 * @brief Get the count of the System tick in milliseconds
 *
 * @return uint32_t
 */
uint32_t port_system_get_millis(void);

/**
 * @brief Fet the count of the System tick in microseconds
 * 
 * @return uint32_t 
 */
uint32_t port_system_get_micros(void);

/**
 * @brief Wait for some milliseconds
 *
 * @param ms Number of milliseconds to wait
 *
 * @retval None
 */
void port_system_delay_ms(uint32_t ms);

/**
 * @brief Wait for some milliseconds from a time reference.
 *
 * @note It also updates the time reference to the system time at return.
 *
 * @param p_t Pointer to the time reference
 * @param ms Number of milliseconds to wait
 *
 * @retval None
 */
void port_system_delay_until_ms(uint32_t *p_t, uint32_t ms);

/**
 * @brief Initializes the TIM5
 * 
 * @param arr The auto reload value
 * @param psc The prescaler value
 */
void port_system_timer5_init(uint16_t arr, uint16_t psc);

/**
 * @brief Wait for some microseconds
 * 
 * @param micros Number of decade of microseconds to wait
 * 
 * @note The wait delay will be micros*10 microseconds
 */
void port_system_delay_micros(uint32_t micros);

/**
 * @brief Suspend Tick increment.

The SysTick timer is the source of time base. It is used to generate interrupts at regular time intervals. Once this function is called, the SysTick interrupt will be disabled so it saves more energy and it does not generate any interruption.
*/
void port_system_systick_suspend();

/**
 * @brief Resume Tick increment.

The SysTick timer is the source of time base. It is used to generate interrupts at regular time intervals. Once this function is called, the SysTick interrupt will be enabled and so Tick increment is resumed.
*/
void port_system_systick_resume();

/** @verbatim
      ==============================================================================
                              ##### How to use GPIOs #####
      ==============================================================================
      [..]
        (#) Enable the GPIO AHB clock using the RCC->AHB1ENR register.

        (#) Configure the GPIO pin.
            (++) Configure the IO mode.
            (++) Activate Pull-up, Pull-down resistor.
            (++) In case of Output or alternate function mode, configure the speed if needed.
            (++) Configure digital or analog mode.
            (++) In case of external interrupt/event select the type (interrupt or event) and
                 the corresponding trigger event (rising or falling or both).

        (#) In case of external interrupt/event mode selection, configure NVIC IRQ priority
            mapped to the EXTI line and enable it using.

        (#) To get the level of a pin configured in input mode use the GPIOx_IDR register.

        (#) To set/reset the level of a pin configured in output mode use the GPIOx_BSRR register
            to SET (bits 0..15) or RESET (bits 16..31) the GPIO.

        @endverbatim
      ******************************************************************************
      */
/**
 * @brief Configure the mode and pull of a GPIO
 *
 * @note This function performs the GPIO Port Clock Enable. It may occur that a port clock is re-enabled,
 *       it does not matter if it was already enabled. *
 * @note This function enables the AHB1 peripheral clock. After reset, the peripheral clock (used for registers
 *       read/write access) is disabled and the application software has to enable this clock before using it.
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param mode Input, output, alternate, or analog
 * @param pupd Pull-up, pull-down, or no-pull
 *
 * @retval None
 */
void port_system_gpio_config(GPIO_TypeDef *port, uint8_t pin, uint8_t mode, uint8_t pupd);

/**
 * @brief Configure the alternate function of a GPIO
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param alternate Alternate function number (values from 0 to 15) according to table of the datasheet: "Table 11. Alternate function".
 *
 * @retval None
 */
void port_system_gpio_config_alternate(GPIO_TypeDef *port, uint8_t pin, uint8_t alternate);

/**
 * @brief Configure the external interruption or event of a GPIO
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param mode Trigger mode can be a combination (OR) of: (i) direction: rising edge (0x01), falling edge (0x02), (ii)  event request (0x04), or (iii) interrupt request (0x08).
 * @retval None
 */
void port_system_gpio_config_exti(GPIO_TypeDef *port, uint8_t pin, uint32_t mode);

/**
 * @brief Enable interrupts of a GPIO line (pin)
 *
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param priority Priority level (from highest priority: 0, to lowest priority: 15)
 * @param subpriority Subpriority level (from highest priority: 0, to lowest priority: 15)
 *
 * @retval None
 */
void port_system_gpio_exti_enable(uint8_t pin, uint8_t priority, uint8_t subpriority);

/**
 * @brief Disable interrupts of a GPIO line (pin)
 *
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 *
 * @retval None
 */
void port_system_gpio_exti_disable(uint8_t pin);

/**
 * @brief Read the digital value of a GPIO
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 *
 * @return `true` if the GPIO was HIGH
 * @return `false` if the GPIO was LOW
 */
bool port_system_gpio_read(GPIO_TypeDef *port, uint8_t pin);

/**
 * @brief Toggle the value of a GPIO
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 *
 * @retval None
 */
void port_system_gpio_toggle(GPIO_TypeDef *port, uint8_t pin);

/**
 * @brief Write a digital value in a GPIO atomically
 * 
 * @note You can use a +16 offset on the pin index and use the `BIT_POS_TO_MASK(pin)` macro to get the mask when you go to clear a GPIO. Otherwise, you can calculate the pin mask first and then use a 16-position left shift of the mask. 
 *
 * @param port Port of the GPIO (CMSIS struct like)
 * @param pin Pin/line of the GPIO (index from 0 to 15)
 * @param value Boolean value to set the GPIO to HIGH (1, `true`) or LOW (0, `false`)
 *
 * @retval None
 */
void port_system_gpio_write(GPIO_TypeDef *port, uint8_t pin, bool value);

/**
 * @brief Sets all the pins of a port to 0
 * 
 * @param port port to be cleared
 */
void port_system_gpio_clear(GPIO_TypeDef *port);

/**
 * @brief Gets the uuid buffer
 * 
 * @return const uint8_t* 
 */
const uint8_t *pv_get_uuid(void);

/**
 * @brief Gets the uuid buffer size
 * 
 * @return uint32_t 
 */
uint32_t pv_get_uuid_size(void);


#endif /* PORT_SYSTEM_H_ */
