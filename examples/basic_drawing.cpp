/**
 * @file basic_drawing.cpp
 * @brief Базовый пример рисования с ST7789V3
 * 
 * Этот пример демонстрирует основные возможности библиотеки ST7789V3:
 * - Инициализация дисплея
 * - Основные графические примитивы
 * - Вывод текста
 * - Работа с цветами
 */

#include "st7789v3.hpp"
#include "main.h"

// Внешние переменные из main.c
extern SPI_HandleTypeDef hspi1;

// Создание объекта дисплея
ST7789V3 display(&hspi1, 
                 ST7789_GPIO(GPIOA, GPIO_PIN_4),  // CS
                 ST7789_GPIO(GPIOA, GPIO_PIN_3),  // DC  
                 ST7789_GPIO(GPIOA, GPIO_PIN_2)); // RST

/**
 * @brief Инициализация дисплея
 */
void display_init() {
    display.init();
    display.fillScreen(ST7789_Colors::BLACK);
    HAL_Delay(100);
}

/**
 * @brief Демонстрация основных графических примитивов
 */
void demo_basic_drawing() {
    // Очистка экрана
    display.fillScreen(ST7789_Colors::BLACK);
    HAL_Delay(500);
    
    // Рисование точек
    for (int i = 0; i < 50; i++) {
        uint16_t x = rand() % 240;
        uint16_t y = rand() % 320;
        uint16_t color = rand() % 0xFFFF;
        display.drawPixel(x, y, color);
    }
    HAL_Delay(2000);
    
    // Рисование линий
    display.fillScreen(ST7789_Colors::BLACK);
    display.drawLine(0, 0, 239, 319, ST7789_Colors::WHITE);
    display.drawLine(239, 0, 0, 319, ST7789_Colors::WHITE);
    display.drawLine(120, 0, 120, 319, ST7789_Colors::RED);
    display.drawLine(0, 160, 239, 160, ST7789_Colors::GREEN);
    HAL_Delay(2000);
    
    // Рисование прямоугольников
    display.fillScreen(ST7789_Colors::BLACK);
    display.drawRect(20, 20, 200, 100, ST7789_Colors::BLUE);
    display.drawRect(40, 40, 160, 60, ST7789_Colors::GREEN);
    display.fillRect(60, 50, 120, 40, ST7789_Colors::RED);
    HAL_Delay(2000);
    
    // Рисование окружностей
    display.fillScreen(ST7789_Colors::BLACK);
    display.drawCircle(120, 160, 80, ST7789_Colors::CYAN);
    display.drawCircle(120, 160, 60, ST7789_Colors::MAGENTA);
    display.fillCircle(120, 160, 40, ST7789_Colors::YELLOW);
    display.fillCircle(120, 160, 20, ST7789_Colors::RED);
    HAL_Delay(2000);
}

/**
 * @brief Демонстрация работы с текстом
 */
void demo_text() {
    display.fillScreen(ST7789_Colors::BLACK);
    
    // ASCII текст
    display.drawString(10, 10, "Hello, World!", ST7789_Colors::WHITE);
    display.drawString(10, 30, "ST7789V3 Library", ST7789_Colors::GREEN);
    display.drawString(10, 50, "ASCII Text Support", ST7789_Colors::BLUE);
    
    // UTF-8 кириллица
    display.drawStringUTF8(10, 80, "Привет, мир!", ST7789_Colors::CYAN);
    display.drawStringUTF8(10, 100, "Поддержка кириллицы", ST7789_Colors::MAGENTA);
    display.drawStringUTF8(10, 120, "UTF-8 кодировка", ST7789_Colors::YELLOW);
    
    // Информация о дисплее
    display.drawString(10, 150, "Display: 240x320", ST7789_Colors::WHITE);
    display.drawString(10, 170, "Colors: RGB565", ST7789_Colors::WHITE);
    display.drawString(10, 190, "Font: 8x16 pixels", ST7789_Colors::WHITE);
    
    HAL_Delay(3000);
}

/**
 * @brief Демонстрация масштабированного текста
 */
void demo_scaled_text() {
    display.fillScreen(ST7789_Colors::BLACK);
    
    // Разные размеры текста
    display.drawStringScaled(10, 10, "Scale 1x", ST7789_Colors::WHITE, 1);
    display.drawStringScaled(10, 40, "Scale 2x", ST7789_Colors::GREEN, 2);
    display.drawStringScaled(10, 80, "3x", ST7789_Colors::BLUE, 3);
    display.drawStringScaled(10, 140, "4x", ST7789_Colors::RED, 4);
    
    // Кириллица масштабированная
    display.drawStringUTF8Scaled(10, 220, "Текст 2x", ST7789_Colors::CYAN, 2);
    display.drawStringUTF8Scaled(10, 260, "3x", ST7789_Colors::MAGENTA, 3);
    
    HAL_Delay(3000);
}

/**
 * @brief Демонстрация работы с цветами
 */
void demo_colors() {
    display.fillScreen(ST7789_Colors::BLACK);
    
    // Базовые цвета
    const char* color_names[] = {
        "RED", "GREEN", "BLUE", "YELLOW", 
        "CYAN", "MAGENTA", "WHITE"
    };
    
    uint16_t colors[] = {
        ST7789_Colors::RED, ST7789_Colors::GREEN, ST7789_Colors::BLUE, 
        ST7789_Colors::YELLOW, ST7789_Colors::CYAN, ST7789_Colors::MAGENTA, 
        ST7789_Colors::WHITE
    };
    
    for (int i = 0; i < 7; i++) {
        display.fillRect(10, 10 + i * 40, 60, 30, colors[i]);
        display.drawString(80, 20 + i * 40, color_names[i], ST7789_Colors::WHITE);
    }
    
    // Custom цвета через RGB565
    display.fillRect(150, 10, 80, 30, display.rgb565(255, 128, 0));   // Оранжевый
    display.drawString(150, 50, "Orange", ST7789_Colors::WHITE);
    
    display.fillRect(150, 70, 80, 30, display.rgb565(128, 0, 128));   // Фиолетовый
    display.drawString(150, 110, "Purple", ST7789_Colors::WHITE);
    
    display.fillRect(150, 130, 80, 30, display.rgb565(255, 192, 203)); // Розовый
    display.drawString(150, 170, "Pink", ST7789_Colors::WHITE);
    
    HAL_Delay(3000);
}

/**
 * @brief Демонстрация поворота дисплея
 */
void demo_rotation() {
    const char* rotation_names[] = {
        "Rotation: 0°", "Rotation: 90°", 
        "Rotation: 180°", "Rotation: 270°"
    };
    
    for (int rot = 0; rot < 4; rot++) {
        display.setRotation(rot);
        display.fillScreen(ST7789_Colors::BLACK);
        
        // Рисование рамки для ориентации
        display.drawRect(0, 0, 240, 320, ST7789_Colors::WHITE);
        display.drawRect(5, 5, 230, 310, ST7789_Colors::RED);
        
        // Текст с указанием поворота
        display.drawString(20, 20, rotation_names[rot], ST7789_Colors::WHITE);
        display.drawStringUTF8(20, 40, "Поворот дисплея", ST7789_Colors::GREEN);
        
        // Индикаторы углов
        display.fillCircle(20, 20, 10, ST7789_Colors::BLUE);   // Верхний левый
        display.fillCircle(220, 20, 10, ST7789_Colors::GREEN); // Верхний правый
        display.fillCircle(220, 300, 10, ST7789_Colors::RED);  // Нижний правый
        display.fillCircle(20, 300, 10, ST7789_Colors::YELLOW);// Нижний левый
        
        HAL_Delay(2000);
    }
    
    // Возврат к нормальной ориентации
    display.setRotation(0);
}

/**
 * @brief Демонстрация инверсии цветов
 */
void demo_color_inversion() {
    display.fillScreen(ST7789_Colors::BLACK);
    display.drawString(10, 10, "Normal Colors", ST7789_Colors::WHITE);
    display.drawString(10, 30, "White on Black", ST7789_Colors::WHITE);
    display.fillRect(10, 60, 100, 50, ST7789_Colors::RED);
    display.fillRect(120, 60, 100, 50, ST7789_Colors::GREEN);
    
    HAL_Delay(2000);
    
    // Включение инверсии
    display.invertOn();
    display.drawString(10, 130, "Inverted Colors", ST7789_Colors::WHITE);
    display.drawString(10, 150, "Black on White", ST7789_Colors::WHITE);
    display.fillRect(10, 180, 100, 50, ST7789_Colors::RED);
    display.fillRect(120, 180, 100, 50, ST7789_Colors::GREEN);
    
    HAL_Delay(2000);
    
    // Отключение инверсии
    display.invertOff();
}

/**
 * @brief Главная функция демонстрации
 */
void run_basic_drawing_demo() {
    // Инициализация
    display_init();
    
    while (1) {
        // Демонстрация базового рисования
        demo_basic_drawing();
        
        // Демонстрация текста
        demo_text();
        
        // Демонстрация масштабированного текста
        demo_scaled_text();
        
        // Демонстрация цветов
        demo_colors();
        
        // Демонстрация поворота
        demo_rotation();
        
        // Демонстрация инверсии цветов
        demo_color_inversion();
        
        // Пауза перед повтором
        HAL_Delay(2000);
    }
}

/**
 * @brief Простой пример для быстрого старта
 */
void quick_start_example() {
    // Инициализация дисплея
    display.init();
    
    // Заливка экрана синим цветом
    display.fillScreen(ST7789_Colors::BLUE);
    
    // Рисование белой линии
    display.drawLine(0, 0, 239, 319, ST7789_Colors::WHITE);
    
    // Рисование красного прямоугольника
    display.fillRect(50, 50, 140, 100, ST7789_Colors::RED);
    
    // Вывод текста
    display.drawString(10, 10, "Hello ST7789V3!", ST7789_Colors::WHITE);
    display.drawStringUTF8(10, 200, "Привет, дисплей!", ST7789_Colors::YELLOW);
    
    // Рисование зелёной окружности
    display.fillCircle(120, 160, 30, ST7789_Colors::GREEN);
}
