/******************************************************************************
 * @file        i2c_driver.h
 * @brief       Cabecera de la abstracción del bus I2C para sistemas Linux.
 * Define la interfaz para la apertura, cierre y transferencia de datos.
 * @author      Paolo Urbina
 * @date        16 de Marzo, 2026
 * @version     1.0
 ******************************************************************************/

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>


/**
 * @brief Descriptor de archivo global para el bus I2C.
 * Se declara como extern para que otros módulos (como ads1115.c)
 * puedan conocer el estado del bus si fuera necesario.
 */
extern int i2c_fd;

/**
 * @brief Abre el dispositivo I2C especificado.
 * @param device_i2c Ruta al archivo (ej: "/dev/i2c-3").
 * @return 0 si éxito, -1 si falla.
 */
int i2c_open(const char *device_i2c);


/**
 * @brief Escribe datos hacia un esclavo I2C.
 * @param addr Dirección de 7 bits del esclavo.
 * @param buff Puntero al buffer de datos.
 * @param len  Cantidad de bytes a escribir.
 * @return 0 si éxito, -1 si falla.
 */
int i2c_write(const uint8_t addr, const uint8_t *buffer, size_t len);


/**
 * @brief Lee datos desde un esclavo I2C.
 * @param addr Dirección de 7 bits del esclavo.
 * @param buff Puntero al buffer de destino.
 * @param len  Cantidad de bytes a leer.
 * @return 0 si éxito, -1 si falla.
 */
int i2c_read(const uint8_t addr, uint8_t *buffer, size_t size);


/**
 * @brief Cierra el puerto I2C y limpia el descriptor.
 */
void i2c_close();
#endif
