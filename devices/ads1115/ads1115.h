/******************************************************************************
 * @file        ads1115.h
 * @brief       Definiciones, registros y prototipos para el ADC ADS1115.
 * @author      [Paolo Urbina]
 * @date        16 de Marzo, 2026
 * @version     1.0
 * @platform    Orange Pi / Linux (I2C)
 * * @copyright   Copyright (c) 2026. Todos los derechos reservados.
 ******************************************************************************/

#ifndef ADS1115 
#define ADS1115

typedef struct
{
        uint32_t os;
        uint32_t mux;
        uint32_t pga;
        uint32_t mode;
        uint32_t dr;
        uint32_t comp_mode;
        uint32_t comp_pol;
        uint32_t comp_lat;
        uint32_t comp_que;
} ads_config_t;

/*
 * DETALLE DEL REGISTRO DE CONFIGURACIÓN (16 bits)
 * ----------------------------------------------------------------------------
 * Bit [15]    : OS (Operational Status / Single-shot conversion start)
 * Escribir 1 inicia una conversión. Leer 0 indica que está ocupado.
 *
 * * Bits [14:12]: MUX (Input multiplexer configuration)
 * Selecciona qué pines medir (Diferencial o Single-ended).
 *
 * * Bits [11:9] : PGA (Programmable gain amplifier configuration)
 * Ajusta la ganancia para cambiar el rango de voltaje (FSR).
 *
 * * Bit [8]     : MODE (Device operating mode)
 * 0 = Modo Continuo (siempre midiendo).
 * 1 = Modo Single-shot (mide una vez y se duerme).
 *
 * * Bits [7:5]  : DR (Data rate)
 * Velocidad de muestreo: de 8 SPS hasta 860 SPS.
 *
 * * Bit [4]     : COMP_MODE (Comparator mode)
 * 0 = Tradicional (con histéresis).
 * 1 = Comparador de ventana.
 *
 * * Bit [3]     : COMP_POL (Comparator polarity)
 * Define si el pin ALERT/RDY es activo en bajo (0) o alto (1).
 *
 * * Bit [2]     : COMP_LAT (Comparator latching)
 * 0 = Sin enclavamiento (la alerta se quita sola).
 * 1 = Con enclavamiento (hay que leer el registro para quitarla).
 *
 * * Bits [1:0]  : COMP_QUE (Comparator queue and disable)
 * Controla cuántas muestras fuera de rango activan la alerta,
 * o desactiva el pin ALERT/RDY (valor 11).
 * ----------------------------------------------------------------------------
 */


/*Bit 15: indica inicio de la conversion*/
#define ADS_OS_START    (0x01 << 15) //Inicia la conversión
#define ADS_OS_NOT      (0x00 << 15) //Ignorar, sin efectos.


/* Bits [14:12]: MUX (Input multiplexer configuration)*/
enum
{
        ADS_MUX_AIN0_AIN1 = (0x0U << 12), //A0 VS A1
        ADS_MUX_AIN0_AIN3 = (0x1U << 12), //A0 VS A3
        ADS_MUX_AIN1_AIN3 = (0x2U << 12), //A1 VS A3
        ADS_MUX_AIN2_AIN3 = (0x3U << 12), //A2 VS A3
        ADS_MUX_AIN0_GND  = (0x4U << 12), //A0 VS GND
        ADS_MUX_AIN1_GND  = (0x5U << 12), //A1 VS GND
        ADS_MUX_AIN2_GND  = (0x6U << 12), //A2 VS GND
        ADS_MUX_AIN3_GND  = (0x7U << 12)  //A3 VS GND
};


/* Bits [11:9]*/
enum
{
        ADS_PGA_6_144 = (0x0U << 9), //Volt: 6.144 V
        ADS_PGA_4_096 = (0x1U << 9), // 4.096 Volt
        ADS_PGA_2_048 = (0x2U << 9), // 2.048 Volt
        ADS_PGA_1_024 = (0x3U << 9), // 1.024 Volt
        ADS_PGA_0_512 = (0x4U << 9), // 0.512 Volt
        ADS_PGA_0_256 = (0x5U << 9)  // 0.256 Volt
};


/* Bit [8]*/
#define ADS_MODE_CONTINUOUS             (0x0U << 8) //Lectura continua
#define ADS_MODE_SINGLE                 (0x1U << 8) //1 lectura -> modo ahorro.


/* Bits [7:5]*/
enum
{
        ADS_DR_8_SPS   = (0x0U << 5),
        ADS_DR_16_SPS  = (0x1U << 5),
        ADS_DR_32_SPS  = (0x2U << 5),
        ADS_DR_64_SPS  = (0x3U << 5),
        ADS_DR_128_SPS = (0x4U << 5),
        ADS_DR_250_SPS = (0x5U << 5),
        ADS_DR_475_SPS = (0x6U << 5),
        ADS_DR_860_SPS = (0x7U << 5)
};


/* Bit [4]*/
#define ADS_COMP_MODE_TRAD              (0U << 4) //COMPARACIÓN TRADICIONAL
#define ADS_COMP_MODE_WINDOW            (1U << 4) //COMPARADOR DE VENTANA


/* Bit [3]*/
#define ADS_COMP_POL_LOW                (0U << 3) //COMPARADOR EN ALTO
#define ADS_COMP_POL_HIGH               (1U << 3) //COMPARADOR EN BAJO


/* Bit [2]*/
#define ADS_COMP_LATCH                  (0U << 2) //LATCH DEL COMPARADOR
#define ADS_COMP_NO_LATCH               (1U << 2) //SIN LATCH


/* Bits [1:0]/
/*ACTIVA EL PIN ALERT SI EL VALOR LEIDO PASA DEL LÍMITE*/
enum
{
        ADS_COMP_QUE_ONE        = (0x00 << 0), //ACTIVA DE INMEDIATO
        ADS_COMP_QUE_TWO        = (0x01 << 0), //ESPERA 2 MUESTRAS SEGUIDAS FUERA DE RANGO
        ADS_COMP_QUE_FOUR       = (0x02 << 0), //ESPERA 4 MUESTRAS SEGUIDAS FUERA DE RANGO
        ADS_COMP_QUE_DISABLE    = (0x03 << 0)  //DESACTIVADO
};



int ads1115_init(const uint8_t addr, ads_config_t *ads_cfg);
int ads_read(const uint8_t addr, int16_t *valor);

#endif
