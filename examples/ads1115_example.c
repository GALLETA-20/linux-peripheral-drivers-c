/******************************************************************************
 * @file        example.c
 * @brief       Ejemplo de uso del driver ADS1115 en Orange Pi.
 * @author      Paolo Urbina
 * @date        16 de Marzo, 2026
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "../devices/ads1115.h"
#include <signal.h>
#include "../i2c_driver/i2c_driver.h"

/*ARCHIVO DEL PUERTO I2C*/
#define DEVICE_I2C "/dev/i2c-3"

/*DIRECCIÓN DEL ADS1115*/
const uint8_t addr_ads = 0x48;

/*ETIQUETA DEL VALOR LEIDO*/
int16_t value = 0;

/*Valor para parar el proceso, 1 = sigue, 0 = parar*/
volatile sig_atomic_t ejecutar = 1;


/**
 * @brief Manejador de señal para cerrar el programa limpiamente con Ctrl+C
 */
void flag1(int sig)
{
        printf("\n[SISTEMA]: Cerrando recursos...\n");

        //Mata el while
        ejecutar = 0;
}

ads_config_t configuracion =
{
        .os             = ADS_OS_START, //Iniciar conversión
        .mux            = ADS_MUX_AIN0_GND, //A1 vs GND
        .pga            = ADS_PGA_4_096, //Voltaje: 4.096 V
        .mode           = ADS_MODE_CONTINUOUS, //Modo Continuo
        .dr             = ADS_DR_32_SPS, //Tasa de muestreo de 32 SPS
        .comp_mode      = ADS_COMP_MODE_TRAD, //Comparador Tradicional
        .comp_pol       = ADS_COMP_POL_LOW, //Polaridad activa en bajo
        .comp_lat       = ADS_COMP_NO_LATCH, //Sin Latch
        .comp_que       = ADS_COMP_QUE_DISABLE //Desactivado el pin Alert
};


int main(void)
{
        //Abre el puerto I2C
        i2c_open(DEVICE_I2C);

        //Envia la configuración de funcionamiento al ads1115
        if(ads1115_init(addr_ads, &configuracion) < 0) return -1;

        //Levanta la función de interrupción de Ctrl + c
        signal(SIGINT, flag1);

        printf("Lectura iniciada, Presiona Ctrl + c para detener.\n");
        sleep(1);

        while(ejecutar)
        {
                //Lee el valor del adc
                if(ads_read(addr_ads, &value) < 0) break;

                //2^15 = 32 768 -> Full scale
                float voltaje = (float)(4.096 / 32768.0) * value;

                //Se imprime el valor leido
                printf("El valor del ads es: %d.\n", value);
                sleep(1);
        }

        //Cierra el puerto i2c. 
        i2c_close();
        return 0;
}
