/******************************************************************************
 * @file        i2c_driver.c
 * @brief       Abstracción del bus I2C para sistemas Linux (I2C-dev).
 * Permite la apertura, cierre, lectura y escritura de 
 * dispositivos esclavos mediante File Descriptors.
 * @author      Paolo Urbina
 * @date        16 de Marzo, 2026
 * @version     1.0
 ******************************************************************************/

#include "i2c_driver.h"

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

/*CABECERAS PARA I2C*/
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>

/* File Descriptor global para el acceso al bus */
int i2c_fd;

/*FUNCIONES ESTATICAS NO PUBLICAS*/
/**
 * @brief Configura la dirección del esclavo I2C en el Kernel.
 * @param fd   Descriptor del archivo del bus I2C.
 * @param addr Dirección de 7 bits del dispositivo esclavo.
 * @return int 0 si tiene éxito, -1 si falla.
 */

static int i2c_addr(const int i2c_fd, const uint8_t addr)
{
        if(ioctl(i2c_fd, I2C_SLAVE, addr))
        {
                perror("!ERROR DEL REGISTRO");
                return -1;
        }

        return 0;

}


/**
 * @brief Abre el dispositivo I2C especificado (ej: /dev/i2c-1).
 * @param device_i2c Ruta al archivo del dispositivo en /dev.
 * @return int 0 si abre correctamente, -1 si falla.
 */
int i2c_open(const char *device_i2c)
{
        if((i2c_fd = open(device_i2c, O_RDWR)) < 0)
        {
                perror("Error al abrir el archivo i2c");
                return -1;
        }
        return 0;
}



/**
 * @brief Escribe datos en el bus I2C hacia un esclavo específico.
 * @param addr Dirección del esclavo.
 * @param buff Puntero al buffer de datos a enviar.
 * @param len  Cantidad de bytes a escribir.
 * @return int 0 si tiene éxito, -1 si falla.
 */
int i2c_write(const uint8_t addr, const uint8_t *buff, size_t len)
{
        if(i2c_addr(i2c_fd, addr) < 0)
        {
                perror("ERROR DE ESCLAVO");
                return -1;
        }

        if(write(i2c_fd, buff, len) != len)
        {
                perror("!ERROR AL ESRIBIR");
                return -1;
        }


        return 0;
}


/**
 * @brief Lee datos del bus I2C desde un esclavo específico.
 * @param addr Dirección del esclavo.
 * @param buff Puntero al buffer donde se guardarán los datos.
 * @param len  Cantidad de bytes a leer.
 * @return int 0 si tiene éxito, -1 si falla.
 */
int i2c_read(const uint8_t addr, uint8_t *buff, size_t len)
{
        if(i2c_addr(i2c_fd, addr) < 0)
        {
                perror("ERROR DEL ESCLAVO");
                return -1;
        }

        if(read(i2c_fd, buff, len) != (ssize_t)len)
        {
                perror("ERROR AL LEER LOS DATOS");
                return -1;
        }

        return 0;
}


/**
 * @brief Libera el bus I2C cerrando el file descriptor.
 */
void i2c_close()
{
        if(i2c_fd >= 0)
        {
                close(i2c_fd);
                i2c_fd = -1;
        }

}
