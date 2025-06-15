#ifndef ST7789V3_CONFIG_HPP
#define ST7789V3_CONFIG_HPP

#include <cstdint>

// Таймауты
namespace ST7789_Config {
    constexpr uint32_t SPI_TIMEOUT = 5000;  // Увеличен таймаут для передачи фреймбуфера
    constexpr uint32_t RESET_DELAY = 100;
    constexpr uint32_t INIT_DELAY = 120;
}

// Настройки шрифта
namespace ST7789_Font {
    constexpr uint8_t CHAR_WIDTH = 8;
    constexpr uint8_t CHAR_HEIGHT = 16;
}

// Пины подключения (согласно CubeMX настройкам)
#define ST7789_SPI_HANDLE hspi1

// GPIO пины
#define ST7789_CS_PORT   GPIOA
#define ST7789_CS_PIN    GPIO_PIN_4
#define ST7789_DC_PORT   GPIOA  
#define ST7789_DC_PIN    GPIO_PIN_3
#define ST7789_RST_PORT  GPIOA
#define ST7789_RST_PIN   GPIO_PIN_2

#endif