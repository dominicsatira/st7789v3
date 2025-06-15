#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <cstdint>
#include "stm32f1xx_hal.h"

// Размеры дисплея
constexpr uint16_t FB_WIDTH = 240;
constexpr uint16_t FB_HEIGHT = 320;
constexpr uint32_t FB_SIZE = FB_WIDTH * FB_HEIGHT;

// Класс буфера кадра
class Framebuffer {
private:
    uint16_t* buffer_;
    uint16_t width_;
    uint16_t height_;
    bool allocated_;
    
public:
    // Конструктор и деструктор
    Framebuffer(uint16_t width = FB_WIDTH, uint16_t height = FB_HEIGHT);
    ~Framebuffer();
    
    // Управление буфером
    bool init();
    void clear(uint16_t color = 0x0000);
    void release();
    
    // Основные операции рисования
    void setPixel(uint16_t x, uint16_t y, uint16_t color);
    uint16_t getPixel(uint16_t x, uint16_t y) const;
    
    // Геометрические примитивы
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
    void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
    void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
    
    // Текст
    void drawChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg_color = 0x0000);
    void drawString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg_color = 0x0000);
    void drawStringUTF8(uint16_t x, uint16_t y, const char* utf8_str, uint16_t color, uint16_t bg_color = 0x0000);
    
    // Масштабированный текст
    void drawCharScaled(uint16_t x, uint16_t y, char ch, uint16_t color, uint8_t scale, uint16_t bg_color = 0x0000);
    void drawStringScaled(uint16_t x, uint16_t y, const char* str, uint16_t color, uint8_t scale, uint16_t bg_color = 0x0000);
    void drawStringUTF8Scaled(uint16_t x, uint16_t y, const char* utf8_str, uint16_t color, uint8_t scale, uint16_t bg_color = 0x0000);
    
    // Доступ к буферу
    const uint16_t* getBuffer() const { return buffer_; }
    uint16_t* getBuffer() { return buffer_; }
    uint32_t getBufferSize() const { return static_cast<uint32_t>(width_) * height_; }
    uint16_t getWidth() const { return width_; }
    uint16_t getHeight() const { return height_; }
    bool isAllocated() const { return allocated_; }
    
    // Утилиты
    static uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);
};

#endif
