#ifndef ST7789V3_HPP
#define ST7789V3_HPP

#include "stm32f4xx_hal.h"
#include "st7789v3_config.hpp"
#include <cstdint>

// Предварительное объявление класса Framebuffer
class Framebuffer;

// Размеры дисплея
constexpr uint16_t ST7789_WIDTH = 240;
constexpr uint16_t ST7789_HEIGHT = 320;

// Цвета RGB565
namespace ST7789_Colors {
    constexpr uint16_t BLACK   = 0x0000;
    constexpr uint16_t WHITE   = 0xFFFF;
    constexpr uint16_t RED     = 0xF800;
    constexpr uint16_t GREEN   = 0x07E0;
    constexpr uint16_t BLUE    = 0x001F;
    constexpr uint16_t YELLOW  = 0xFFE0;
    constexpr uint16_t CYAN    = 0x07FF;
    constexpr uint16_t MAGENTA = 0xF81F;
}

// Команды ST7789
namespace ST7789_Commands {
    constexpr uint8_t SWRESET = 0x01;
    constexpr uint8_t SLPOUT  = 0x11;
    constexpr uint8_t COLMOD  = 0x3A;
    constexpr uint8_t MADCTL  = 0x36;
    constexpr uint8_t CASET   = 0x2A;
    constexpr uint8_t RASET   = 0x2B;
    constexpr uint8_t RAMWR   = 0x2C;
    constexpr uint8_t DISPON  = 0x29;
    constexpr uint8_t INVOFF  = 0x20;  // Отключить инверсию цветов
    constexpr uint8_t INVON   = 0x21;  // Включить инверсию цветов
}

// Структура конфигурации GPIO
struct ST7789_GPIO {
    GPIO_TypeDef* port;
    uint16_t pin;
    
    ST7789_GPIO(GPIO_TypeDef* p, uint16_t pin_num) : port(p), pin(pin_num) {}
};

// Класс дисплея ST7789V3
class ST7789V3 {
private:
    SPI_HandleTypeDef* hspi_;
    ST7789_GPIO cs_pin_;
    ST7789_GPIO dc_pin_;
    ST7789_GPIO rst_pin_;
    Framebuffer* framebuffer_;  // Указатель на буфер кадра
    
    void writeCommand(uint8_t cmd);
    void writeData8(uint8_t data);
    void writeData16(uint16_t data);
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    
public:
    // Конструктор
    ST7789V3(SPI_HandleTypeDef* spi_handle,
             const ST7789_GPIO& cs,
             const ST7789_GPIO& dc,
             const ST7789_GPIO& rst);
    
    // Основные функции
    void init();
    void reset();
    void invertColors(bool invert = true);  // Инверсия цветов
    void invertOn();                        // Включить инверсию
    void invertOff();                       // Отключить инверсию
    
    // Функции рисования
    void fillScreen(uint16_t color);
    void drawPixel(uint16_t x, uint16_t y, uint16_t color);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
    void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
      // Функции текста
    void drawChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg_color = ST7789_Colors::BLACK);
    void drawString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg_color = ST7789_Colors::BLACK);
    void drawStringUTF8(uint16_t x, uint16_t y, const char* utf8_str, uint16_t color, uint16_t bg_color = ST7789_Colors::BLACK);
    
    // Функции масштабированного текста
    void drawCharScaled(uint16_t x, uint16_t y, char ch, uint16_t color, uint8_t scale, uint16_t bg_color = ST7789_Colors::BLACK);
    void drawStringScaled(uint16_t x, uint16_t y, const char* str, uint16_t color, uint8_t scale, uint16_t bg_color = ST7789_Colors::BLACK);
    void drawStringUTF8Scaled(uint16_t x, uint16_t y, const char* utf8_str, uint16_t color, uint8_t scale, uint16_t bg_color = ST7789_Colors::BLACK);      // Функции работы с буфером кадра
    bool setFramebuffer(Framebuffer* fb);   // Установить буфер кадра
    void clearFramebuffer();                // Очистить буфер кадра
    Framebuffer* getFramebuffer() const;    // Получить указатель на буфер кадра
    void flushFramebuffer();                // Передать буфер кадра на дисплей
    void flushFramebufferDMA();             // Передать буфер кадра на дисплей через DMA
    void flushFramebufferRegion(uint16_t x, uint16_t y, uint16_t w, uint16_t h); // Передать регион буфера
    bool isFramebufferEnabled() const;      // Проверить, включен ли буфер кадра
    
    // Функции для работы со статическим буфером
    void flushStaticBuffer(uint16_t* buffer, uint16_t width, uint16_t height);
    void flushStaticBufferDMA(uint16_t* buffer, uint16_t width, uint16_t height);
    
    // Utility функции
    static uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);
    void setRotation(uint8_t rotation);
};

#endif