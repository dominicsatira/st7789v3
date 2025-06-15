/**
 * @file basic_drawing.cpp
 * @brief Пример базовых функций рисования с ST7789V3
 * @author ST7789V3 Library
 * @version 1.0.0
 */

#include "st7789v3.hpp"
#include "stm32f1xx_hal.h"

// Внешние объявления SPI (должны быть определены в main.c)
extern SPI_HandleTypeDef hspi1;

// Создание объекта дисплея
ST7789V3 display(&hspi1, 
                 ST7789_GPIO(ST7789_CS_PORT, ST7789_CS_PIN),
                 ST7789_GPIO(ST7789_DC_PORT, ST7789_DC_PIN),
                 ST7789_GPIO(ST7789_RST_PORT, ST7789_RST_PIN));

/**
 * @brief Инициализация дисплея
 */
void display_init() {
    display.init();
    display.fillScreen(ST7789_Colors::BLACK);
}

/**
 * @brief Демонстрация базовых примитивов
 */
void draw_primitives() {
    // Очистка экрана
    display.fillScreen(ST7789_Colors::BLACK);
    
    // Рисование линий
    display.drawLine(0, 0, 239, 0, ST7789_Colors::RED);        // Верхняя
    display.drawLine(0, 319, 239, 319, ST7789_Colors::RED);    // Нижняя
    display.drawLine(0, 0, 0, 319, ST7789_Colors::RED);        // Левая
    display.drawLine(239, 0, 239, 319, ST7789_Colors::RED);    // Правая
    
    // Диагональные линии
    display.drawLine(0, 0, 239, 319, ST7789_Colors::GREEN);
    display.drawLine(239, 0, 0, 319, ST7789_Colors::GREEN);
    
    // Прямоугольники
    display.drawRect(50, 50, 100, 80, ST7789_Colors::BLUE);
    display.fillRect(60, 60, 80, 60, ST7789_Colors::CYAN);
    
    // Окружности
    display.drawCircle(120, 200, 50, ST7789_Colors::YELLOW);
    display.fillCircle(120, 200, 30, ST7789_Colors::MAGENTA);
    
    HAL_Delay(3000);
}

/**
 * @brief Демонстрация цветовой палитры
 */
void color_demo() {
    display.fillScreen(ST7789_Colors::BLACK);
    
    // Цветные полосы
    const uint16_t colors[] = {
        ST7789_Colors::RED,
        ST7789_Colors::GREEN,
        ST7789_Colors::BLUE,
        ST7789_Colors::YELLOW,
        ST7789_Colors::CYAN,
        ST7789_Colors::MAGENTA,
        ST7789_Colors::WHITE
    };
    
    const uint8_t num_colors = sizeof(colors) / sizeof(colors[0]);
    const uint16_t strip_height = 320 / num_colors;
    
    for (uint8_t i = 0; i < num_colors; i++) {
        display.fillRect(0, i * strip_height, 240, strip_height, colors[i]);
    }
    
    // Градиент
    for (uint16_t x = 0; x < 240; x++) {
        uint8_t red = (x * 255) / 240;
        uint16_t color = display.rgb565(red, 128, 255 - red);
        display.drawLine(x, 280, x, 319, color);
    }
    
    HAL_Delay(3000);
}

/**
 * @brief Демонстрация пикселей
 */
void pixel_demo() {
    display.fillScreen(ST7789_Colors::BLACK);
    
    // Случайные пиксели
    for (uint16_t i = 0; i < 1000; i++) {
        uint16_t x = rand() % 240;
        uint16_t y = rand() % 320;
        uint16_t color = rand() % 65536;
        display.drawPixel(x, y, color);
    }
    
    HAL_Delay(2000);
}

/**
 * @brief Тест скорости заливки
 */
void fill_speed_test() {
    uint32_t start_time, end_time;
    
    // Тест заливки экрана
    start_time = HAL_GetTick();
    for (uint8_t i = 0; i < 10; i++) {
        display.fillScreen(ST7789_Colors::RED);
        display.fillScreen(ST7789_Colors::BLACK);
    }
    end_time = HAL_GetTick();
    
    // Вывод результата (требует подключения отладочного вывода)
    // printf("Fill screen test: %lu ms for 20 operations\n", end_time - start_time);
    
    HAL_Delay(1000);
}

/**
 * @brief Геометрические паттерны
 */
void geometric_patterns() {
    display.fillScreen(ST7789_Colors::BLACK);
    
    // Концентрические окружности
    for (uint8_t r = 10; r < 100; r += 10) {
        uint16_t color = display.rgb565(r * 2, 255 - r * 2, 128);
        display.drawCircle(120, 160, r, color);
    }
    
    HAL_Delay(2000);
    
    // Сетка
    display.fillScreen(ST7789_Colors::BLACK);
    for (uint16_t x = 0; x < 240; x += 20) {
        display.drawLine(x, 0, x, 319, ST7789_Colors::GREEN);
    }
    for (uint16_t y = 0; y < 320; y += 20) {
        display.drawLine(0, y, 239, y, ST7789_Colors::GREEN);
    }
    
    HAL_Delay(2000);
}

/**
 * @brief Основная функция демонстрации
 */
void basic_drawing_demo() {
    // Инициализация
    display_init();
    
    // Запуск демонстраций
    while (1) {
        draw_primitives();
        color_demo();
        pixel_demo();
        fill_speed_test();
        geometric_patterns();
    }
}

/**
 * @brief Функция для вызова из main()
 * @note Добавьте вызов этой функции в ваш main()
 */
void run_basic_drawing_example() {
    basic_drawing_demo();
}
