/**
 * @file port_system.h
 * @brief Header for port_system.c file.
 * @author SDG2. Román Cárdenas (r.cardenas@upm.es) and Josué Pagán (j.pagan@upm.es)
 * @date 2025-01-01
 */

#ifndef PORT_SYSTEM_H_
#define PORT_SYSTEM_H_

/* Includes del sistema */
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"

/**
 * @brief Initializes the system.
 */
uint32_t port_system_init(void);

/**
 * @brief Returns the number of milliseconds since the system started.
 *
 * @retval number of milliseconds since the system started.
 */
uint32_t port_system_get_millis(void);

/**
 * @brief Sets the number of milliseconds since the system started.
 *
 * @param ms New number of milliseconds since the system started.
 */
void port_system_set_millis(uint32_t ms);

/**
 * @brief Delays the program execution for the specified number of milliseconds.
 *
 * @param ms Number of milliseconds to delay.
 */
void port_system_delay_ms(uint32_t ms);

/**
 * @brief Delays the program execution until the specified number of milliseconds since the system started.
 *
 * @param t Pointer to the variable that stores the number of milliseconds to delay until.
 * @param ms Number of milliseconds to delay until.
 *
 * @note This function modifies the value of the variable pointed by t to the number of milliseconds to delay until.
 * @note This function is useful to implement periodic tasks.
 */
void port_system_delay_until_ms(uint32_t *t, uint32_t ms);

/**
 * @brief Read the digital value of a GPIO
 * @param p_port Pointer to the port of the GPIO
 * @param pin  Pin of the GPIO
 *
 * @note 
 * @note 
 */
bool stm32f4_system_gpio_read(GPIO_TypeDef * p_port, uint8_t pin);

/**
 * @brief Write a digital value in a GPIO atomically
 * @param p_port Pointer to the port of the GPIO
 * @param pin  Pin of the GPIO
 *  @param value Boolean value to set the gpio to high
 * @note No return values
 * @note 
 */
void stm32f4_system_gpio_write(GPIO_TypeDef * p_port, uint8_t pin, bool value);
/**
 * @brief Toggle the value of a GPIO
 * @param p_port Pointer to the port of the GPIO
 * @param pin  Pin of the GPIO
 *  
 * @note 
 * @note 
 */
void stm32f4_system_gpio_toggle(GPIO_TypeDef *p_port, uint8_t pin);
#endif /* PORT_SYSTEM_H_ */

