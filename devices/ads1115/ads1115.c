#include <stdio.h>
#include <stdint.h>
#include "ads1115.h"
#include "../i2c_driver/i2c_driver.h"

/**
 * @brief Inicializa el ADS1115 con la configuración deseada.
 * * Escribe en el "Config Register" (0x01) los 16 bits de configuración
 * divididos en dos bytes (MSB primero).
 */

int ads1115_init(const uint8_t addr, ads_config_t *ads_cfg)
{
        if(ads_cfg == NULL) return -1;
        uint32_t config =       ads_cfg -> os           |
                                ads_cfg -> mux          |
                                ads_cfg -> pga          |
                                ads_cfg -> mode         |
                                ads_cfg -> dr           |
                                ads_cfg -> comp_mode    |
                                ads_cfg -> comp_pol     |
                                ads_cfg -> comp_lat     |
                                ads_cfg -> comp_que;

        uint8_t cfg_reg[3] = {0};
        cfg_reg[0] = 0x01;
        cfg_reg[1] = (config >> 8);
        cfg_reg[2] = (config & 0xFF);

        if(i2c_write(addr, cfg_reg, 3) < 0 ) return -1;

        return 0;

}



/**
 * @brief Lee el valor de la última conversión del ADC.
 * * Primero apunta al "Conversion Register" (0x00) y luego lee 2 bytes.
 */
int ads_read(const uint8_t addr, int16_t *valor)
{
        uint8_t write = 0x00;
        if(i2c_write(addr, &write, 1) < 0) return -1;

        uint8_t msg[2] = {0};

        if(i2c_read(addr, msg, 2) < 0) return -1;

         *valor = (int16_t)((msg[0] << 8) | msg[1]);

        return 0;
}
