/**
 * @file framebuffer_demo.cpp
 * @brief Демонстрация работы с буфером кадра ST7789V3
 * @author ST7789V3 Library
 * @version 1.0.0
 */

#include "st7789v3.hpp"
#include "framebuffer.hpp"
#include "stm32f1xx_hal.h"
#include <cmath>

// Внешние объявления SPI
extern SPI_HandleTypeDef hspi1;

// Создание объектов
ST7789V3 display(&hspi1, 
                 ST7789_GPIO(ST7789_CS_PORT, ST7789_CS_PIN),
                 ST7789_GPIO(ST7789_DC_PORT, ST7789_DC_PIN),
                 ST7789_GPIO(ST7789_RST_PORT, ST7789_RST_PIN));

Framebuffer framebuffer(240, 320);

/**
 * @brief Инициализация дисплея и буфера кадра
 */
bool framebuffer_init() {
    // Инициализация дисплея
    display.init();
    display.fillScreen(ST7789_Colors::BLACK);
    
    // Инициализация буфера кадра
    if (!framebuffer.init()) {
        // Ошибка выделения памяти
        display.drawString(10, 10, "FB Init Error!", ST7789_Colors::RED);
        return false;
    }
    
    // Привязка буфера к дисплею
    display.setFramebuffer(&framebuffer);
    
    return true;
}

/**
 * @brief Демонстрация базового рисования в буфер
 */
void basic_framebuffer_demo() {
    // Очистка буфера
    framebuffer.clear(ST7789_Colors::BLUE);
    
    // Рисование в буфер
    framebuffer.drawString(10, 10, "Framebuffer Demo", ST7789_Colors::WHITE);
    framebuffer.drawString(10, 30, "Drawing to buffer", ST7789_Colors::YELLOW);
    
    // Геометричесие фигуры
    framebuffer.drawRect(20, 60, 100, 80, ST7789_Colors::RED);
    framebuffer.fillRect(25, 65, 90, 70, ST7789_Colors::GREEN);
    
    framebuffer.drawCircle(180, 100, 40, ST7789_Colors::CYAN);
    framebuffer.fillCircle(180, 100, 25, ST7789_Colors::MAGENTA);
    
    // Передача буфера на дисплей
    display.flushFramebuffer();
    
    HAL_Delay(3000);
}

/**
 * @brief Демонстрация анимации с буфером кадра
 */
void animation_demo() {
    const uint16_t center_x = 120;
    const uint16_t center_y = 160;
    const uint16_t radius = 80;
    
    for (uint16_t angle = 0; angle < 720; angle += 5) {
        // Очистка буфера
        framebuffer.clear(ST7789_Colors::BLACK);
        
        // Вычисление позиции объекта
        float rad = angle * 3.14159f / 180.0f;
        uint16_t x = center_x + (uint16_t)(radius * cosf(rad));
        uint16_t y = center_y + (uint16_t)(radius * sinf(rad));
        
        // Рисование траектории
        framebuffer.drawCircle(center_x, center_y, radius, ST7789_Colors::GREEN);
        
        // Рисование движущегося объекта
        framebuffer.fillCircle(x, y, 15, ST7789_Colors::RED);
        framebuffer.drawCircle(x, y, 15, ST7789_Colors::WHITE);
        
        // Информация о кадре
        framebuffer.drawString(10, 10, "Animation Demo", ST7789_Colors::WHITE);
        
        // Обновление дисплея
        display.flushFramebuffer();
        
        // Задержка для плавной анимации
        HAL_Delay(50);
    }
}

/**
 * @brief Демонстрация частичного обновления буфера
 */
void partial_update_demo() {
    // Статический фон
    framebuffer.clear(ST7789_Colors::BLACK);
    framebuffer.drawString(10, 10, "Partial Update Demo", ST7789_Colors::WHITE);
    framebuffer.drawRect(50, 50, 140, 220, ST7789_Colors::GREEN);
    
    // Полная передача буфера
    display.flushFramebuffer();
    
    HAL_Delay(1000);
    
    // Анимация в определенной области
    for (uint16_t i = 0; i < 100; i++) {
        // Очистка области анимации в буфере
        framebuffer.fillRect(60, 60, 120, 200, ST7789_Colors::BLACK);
        
        // Рисование движущегося объекта
        uint16_t y = 70 + (i * 180) / 100;
        framebuffer.fillRect(110, y, 20, 20, ST7789_Colors::RED);
        
        // Частичное обновление только области анимации
        display.flushFramebufferRegion(60, 60, 120, 200);
        
        HAL_Delay(50);
    }
}

/**
 * @brief Тест производительности буфера кадра
 */
void performance_test() {
    uint32_t start_time, end_time;
    
    framebuffer.clear(ST7789_Colors::BLACK);
    framebuffer.drawString(10, 10, "Performance Test", ST7789_Colors::WHITE);
    display.flushFramebuffer();
    
    HAL_Delay(1000);
    
    // Тест рисования множества пикселей
    start_time = HAL_GetTick();
    
    for (uint16_t i = 0; i < 10000; i++) {
        uint16_t x = i % 240;
        uint16_t y = (i / 240) % 320;
        uint16_t color = i % 65536;
        framebuffer.setPixel(x, y, color);
    }
    
    end_time = HAL_GetTick();
    
    // Отображение результата
    char result[50];
    sprintf(result, "10K pixels: %lu ms", end_time - start_time);
    framebuffer.drawString(10, 30, result, ST7789_Colors::YELLOW);
    
    // Тест передачи буфера
    start_time = HAL_GetTick();
    
    for (uint8_t i = 0; i < 10; i++) {
        display.flushFramebuffer();
    }
    
    end_time = HAL_GetTick();
    
    sprintf(result, "10 flushes: %lu ms", end_time - start_time);
    framebuffer.drawString(10, 50, result, ST7789_Colors::CYAN);
    
    display.flushFramebuffer();
    HAL_Delay(3000);
}

/**
 * @brief Демонстрация сложной графики
 */
void complex_graphics_demo() {
    framebuffer.clear(ST7789_Colors::BLACK);
    
    // Рисование сложного паттерна
    for (uint16_t x = 0; x < 240; x += 2) {
        for (uint16_t y = 0; y < 320; y += 2) {
            uint8_t r = (x * 255) / 240;
            uint8_t g = (y * 255) / 320;
            uint8_t b = ((x + y) * 255) / (240 + 320);
            uint16_t color = framebuffer.rgb565(r, g, b);
            framebuffer.setPixel(x, y, color);
        }
    }
    
    // Наложение текста
    framebuffer.drawStringScaled(20, 140, "COMPLEX", ST7789_Colors::WHITE, 3);
    framebuffer.drawStringScaled(20, 180, "GRAPHICS", ST7789_Colors::WHITE, 3);
    
    display.flushFramebuffer();
    HAL_Delay(3000);
}

/**
 * @brief Демонстрация работы с UTF-8 текстом
 */
void utf8_text_demo() {
    framebuffer.clear(ST7789_Colors::BLUE);
    
    // Различные языки
    framebuffer.drawString(10, 20, "English Text", ST7789_Colors::WHITE);
    framebuffer.drawStringUTF8(10, 40, "Русский текст", ST7789_Colors::YELLOW);
    framebuffer.drawStringUTF8(10, 60, "Кириллица", ST7789_Colors::GREEN);
    
    // Масштабированный текст
    framebuffer.drawStringUTF8Scaled(10, 100, "БОЛЬШОЙ", ST7789_Colors::RED, 2);
    framebuffer.drawStringUTF8Scaled(10, 140, "ТЕКСТ", ST7789_Colors::RED, 2);
    
    // Цветной текст
    framebuffer.drawStringUTF8(10, 180, "Цветной", ST7789_Colors::CYAN, ST7789_Colors::MAGENTA);
    framebuffer.drawStringUTF8(10, 200, "фон текста", ST7789_Colors::WHITE, ST7789_Colors::RED);
    
    display.flushFramebuffer();
    HAL_Delay(3000);
}

/**
 * @brief Основная функция демонстрации буфера кадра
 */
void framebuffer_demo_main() {
    // Инициализация
    if (!framebuffer_init()) {
        // Ошибка инициализации - бесконечный цикл
        while (1) {
            HAL_Delay(1000);
        }
    }
    
    // Запуск демонстраций
    while (1) {
        basic_framebuffer_demo();
        animation_demo();
        partial_update_demo();
        performance_test();
        complex_graphics_demo();
        utf8_text_demo();
    }
}

/**
 * @brief Функция для вызова из main()
 * @note Добавьте вызов этой функции в ваш main()
 * @warning Требует минимум 160 КБ свободной RAM
 */
void run_framebuffer_example() {
    framebuffer_demo_main();
}
