/**
 * @file text_demo.cpp
 * @brief Демонстрация текстовых возможностей библиотеки ST7789V3
 * 
 * Этот пример демонстрирует:
 * - Поддержку ASCII и кириллицы
 * - Масштабирование текста
 * - Работу с UTF-8 кодировкой
 * - Различные стили текста
 */

#include "st7789v3.hpp"
#include "framebuffer.hpp"
#include "main.h"
#include <cstring>

// Внешние переменные из main.c
extern SPI_HandleTypeDef hspi1;

// Создание объектов
ST7789V3 display(&hspi1, 
                 ST7789_GPIO(GPIOA, GPIO_PIN_4),  // CS
                 ST7789_GPIO(GPIOA, GPIO_PIN_3),  // DC
                 ST7789_GPIO(GPIOA, GPIO_PIN_2)); // RST

Framebuffer framebuffer(240, 320);

/**
 * @brief Инициализация для демонстрации текста
 */
void init_text_demo() {
    display.init();
    framebuffer.init();
    display.setFramebuffer(&framebuffer);
}

/**
 * @brief Демонстрация базового ASCII текста
 */
void demo_ascii_text() {
    framebuffer.clear(ST7789_Colors::BLACK);
    
    framebuffer.drawString(10, 10, "ASCII Text Demo", ST7789_Colors::WHITE);
    framebuffer.drawString(10, 30, "Basic Latin Characters", ST7789_Colors::GREEN);
    
    // Алфавит
    framebuffer.drawString(10, 60, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", ST7789_Colors::CYAN);
    framebuffer.drawString(10, 80, "abcdefghijklmnopqrstuvwxyz", ST7789_Colors::CYAN);
    
    // Цифры
    framebuffer.drawString(10, 110, "0123456789", ST7789_Colors::YELLOW);
    
    // Специальные символы
    framebuffer.drawString(10, 140, "!@#$%^&*()_+-=[]{}|;':\",./<>?", ST7789_Colors::MAGENTA);
    
    // Многострочный текст
    framebuffer.drawString(10, 170, "Line 1: Hello World!", ST7789_Colors::WHITE);
    framebuffer.drawString(10, 190, "Line 2: STM32 + ST7789V3", ST7789_Colors::RED);
    framebuffer.drawString(10, 210, "Line 3: Embedded Graphics", ST7789_Colors::BLUE);
    
    display.flushFramebuffer();
    HAL_Delay(3000);
}

/**
 * @brief Демонстрация кириллического текста UTF-8
 */
void demo_cyrillic_text() {
    framebuffer.clear(ST7789_Colors::BLACK);
    
    framebuffer.drawStringUTF8(10, 10, "Демонстрация кириллицы", ST7789_Colors::WHITE);
    framebuffer.drawStringUTF8(10, 30, "UTF-8 кодировка", ST7789_Colors::GREEN);
    
    // Русский алфавит
    framebuffer.drawStringUTF8(10, 60, "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ", ST7789_Colors::CYAN);
    framebuffer.drawStringUTF8(10, 80, "абвгдеёжзийклмнопрстуфхцчшщъыьэюя", ST7789_Colors::CYAN);
    
    // Примеры слов
    framebuffer.drawStringUTF8(10, 110, "Микроконтроллер STM32", ST7789_Colors::YELLOW);
    framебuffer.drawStringUTF8(10, 130, "Графическая библиотека", ST7789_Colors::MAGENTA);
    framebuffer.drawStringUTF8(10, 150, "Цветной дисплей", ST7789_Colors::RED);
    
    // Смешанный текст
    framebuffer.drawStringUTF8(10, 180, "Mixed: Hello Привет 123", ST7789_Colors::WHITE);
    framebuffer.drawString(10, 200, "ASCII: English text", ST7789_Colors::GREEN);
    framebuffer.drawStringUTF8(10, 220, "UTF-8: Русский текст", ST7789_Colors::BLUE);
    
    // Специальные символы кириллицы
    framebuffer.drawStringUTF8(10, 250, "Спецсимволы: №°±×÷≈≠≤≥", ST7789_Colors::CYAN);
    
    display.flushFramebuffer();
    HAL_Delay(4000);
}

/**
 * @brief Демонстрация масштабирования текста
 */
void demo_scaled_text() {
    framebuffer.clear(ST7789_Colors::BLACK);
    
    // Разные масштабы ASCII
    framebuffer.drawStringScaled(10, 10, "Scale 1x", ST7789_Colors::WHITE, 1);
    framebuffer.drawStringScaled(10, 35, "Scale 2x", ST7789_Colors::GREEN, 2);
    framebuffer.drawStringScaled(10, 70, "3x", ST7789_Colors::BLUE, 3);
    framebuffer.drawStringScaled(10, 120, "4x", ST7789_Colors::RED, 4);
    
    display.flushFramebuffer();
    HAL_Delay(2000);
    
    framebuffer.clear(ST7789_Colors::BLACK);
    
    // Разные масштабы кириллицы
    framebuffer.drawStringUTF8Scaled(10, 10, "Масштаб 1x", ST7789_Colors::WHITE, 1);
    framebuffer.drawStringUTF8Scaled(10, 35, "Масштаб 2x", ST7789_Colors::CYAN, 2);
    framebuffer.drawStringUTF8Scaled(10, 70, "3x", ST7789_Colors::YELLOW, 3);
    framebuffer.drawStringUTF8Scaled(10, 120, "4x", ST7789_Colors::MAGENTA, 4);
    
    display.flushFramebuffer();
    HAL_Delay(3000);
}

/**
 * @brief Демонстрация различных цветов текста
 */
void demo_colored_text() {
    framebuffer.clear(ST7789_Colors::BLACK);
    
    const char* colors_en[] = {"RED", "GREEN", "BLUE", "YELLOW", "CYAN", "MAGENTA", "WHITE"};
    const char* colors_ru[] = {"КРАСНЫЙ", "ЗЕЛЁНЫЙ", "СИНИЙ", "ЖЁЛТЫЙ", "ГОЛУБОЙ", "ФИОЛЕТОВЫЙ", "БЕЛЫЙ"};
    
    uint16_t colors[] = {
        ST7789_Colors::RED, ST7789_Colors::GREEN, ST7789_Colors::BLUE,
        ST7789_Colors::YELLOW, ST7789_Colors::CYAN, ST7789_Colors::MAGENTA,
        ST7789_Colors::WHITE
    };
    
    framebuffer.drawString(10, 10, "Color Text Demo", ST7789_Colors::WHITE);
    framebuffer.drawStringUTF8(10, 30, "Цветной текст", ST7789_Colors::WHITE);
    
    for (int i = 0; i < 7; i++) {
        framebuffer.drawString(10, 60 + i * 18, colors_en[i], colors[i]);
        framebuffer.drawStringUTF8(120, 60 + i * 18, colors_ru[i], colors[i]);
    }
    
    // Демонстрация фона
    framebuffer.drawString(10, 220, "With background:", ST7789_Colors::WHITE);
    framebuffer.drawString(10, 240, "BLACK ON WHITE", ST7789_Colors::BLACK, ST7789_Colors::WHITE);
    framebuffer.drawString(10, 260, "WHITE ON RED", ST7789_Colors::WHITE, ST7789_Colors::RED);
    framebuffer.drawStringUTF8(10, 280, "ЖЁЛТЫЙ НА СИНЕМ", ST7789_Colors::YELLOW, ST7789_Colors::BLUE);
    
    display.flushFramebuffer();
    HAL_Delay(4000);
}

/**
 * @brief Демонстрация различных стилей текста
 */
void demo_text_styles() {
    framebuffer.clear(ST7789_Colors::BLACK);
    
    // Заголовок
    framebuffer.drawStringScaled(10, 10, "TEXT STYLES", ST7789_Colors::WHITE, 2);
    framebuffer.drawStringUTF8Scaled(10, 45, "СТИЛИ ТЕКСТА", ST7789_Colors::WHITE, 2);
    
    // Обычный текст
    framebuffer.drawString(10, 90, "Normal text", ST7789_Colors::GREEN);
    framebuffer.drawStringUTF8(10, 110, "Обычный текст", ST7789_Colors::GREEN);
    
    // Выделенный текст
    framebuffer.drawString(10, 140, "BOLD TEXT", ST7789_Colors::YELLOW, ST7789_Colors::RED);
    framebuffer.drawStringUTF8(10, 160, "ЖИРНЫЙ ТЕКСТ", ST7789_Colors::YELLOW, ST7789_Colors::RED);
    
    // Инвертированный текст
    framebuffer.fillRect(10, 185, 200, 16, ST7789_Colors::WHITE);
    framebuffer.drawString(10, 185, "Inverted text", ST7789_Colors::BLACK, ST7789_Colors::WHITE);
    
    framebuffer.fillRect(10, 205, 200, 16, ST7789_Colors::CYAN);
    framebuffer.drawStringUTF8(10, 205, "Инвертированный", ST7789_Colors::BLACK, ST7789_Colors::CYAN);
    
    // Большой текст
    framebuffer.drawStringScaled(10, 240, "BIG", ST7789_Colors::RED, 3);
    framebuffer.drawStringUTF8Scaled(80, 240, "БОЛЬШОЙ", ST7789_Colors::BLUE, 3);
    
    display.flushFramebuffer();
    HAL_Delay(4000);
}

/**
 * @brief Демонстрация форматированного текста
 */
void demo_formatted_text() {
    framebuffer.clear(ST7789_Colors::BLACK);
    
    // Заголовок
    framebuffer.drawStringUTF8Scaled(10, 10, "ДАННЫЕ СИСТЕМЫ", ST7789_Colors::WHITE, 2);
    
    // Системная информация
    char buffer[64];
    
    // Время работы
    uint32_t uptime = HAL_GetTick() / 1000;
    snprintf(buffer, sizeof(buffer), "Uptime: %lu sec", uptime);
    framebuffer.drawString(10, 50, buffer, ST7789_Colors::GREEN);
    
    snprintf(buffer, sizeof(buffer), "Время работы: %lu сек", uptime);
    framebuffer.drawStringUTF8(10, 70, buffer, ST7789_Colors::GREEN);
    
    // Разрешение дисплея
    framebuffer.drawString(10, 100, "Display: 240x320 px", ST7789_Colors::CYAN);
    framebuffer.drawStringUTF8(10, 120, "Дисплей: 240x320 пикс", ST7789_Colors::CYAN);
    
    // Информация о памяти
    framebuffer.drawString(10, 150, "RAM: STM32F411CEU6", ST7789_Colors::YELLOW);
    framebuffer.drawStringUTF8(10, 170, "ОЗУ: Framebuffer 150KB", ST7789_Colors::YELLOW);
    
    // Версия библиотеки
    framebuffer.drawString(10, 200, "Library: ST7789V3 v1.0", ST7789_Colors::MAGENTA);
    framebuffer.drawStringUTF8(10, 220, "Библиотека: ST7789V3", ST7789_Colors::MAGENTA);
    
    // Счетчик кадров
    static uint32_t frame_count = 0;
    frame_count++;
    snprintf(buffer, sizeof(buffer), "Frame: %lu", frame_count);
    framebuffer.drawString(10, 250, buffer, ST7789_Colors::WHITE);
    
    snprintf(buffer, sizeof(buffer), "Кадр: %lu", frame_count);
    framebuffer.drawStringUTF8(10, 270, buffer, ST7789_Colors::WHITE);
    
    display.flushFramebuffer();
    HAL_Delay(1000);
}

/**
 * @brief Демонстрация анимированного текста
 */
void demo_animated_text() {
    const char* text_en = "ANIMATED TEXT DEMO";
    const char* text_ru = "АНИМИРОВАННЫЙ ТЕКСТ";
    
    // Эффект появления
    for (int i = 0; i <= strlen(text_en); i++) {
        framebuffer.clear(ST7789_Colors::BLACK);
        
        char temp[32];
        strncpy(temp, text_en, i);
        temp[i] = '\0';
        
        framebuffer.drawStringScaled(10, 100, temp, ST7789_Colors::GREEN, 2);
        
        display.flushFramebuffer();
        HAL_Delay(100);
    }
    
    HAL_Delay(1000);
    
    // Эффект появления кириллицы
    const char* ptr = text_ru;
    int y_pos = 150;
    framebuffer.drawStringUTF8Scaled(10, y_pos, "", ST7789_Colors::CYAN, 2);
    
    char temp_utf8[64] = "";
    int pos = 0;
    
    while (*ptr) {
        // Копируем следующий UTF-8 символ
        if ((*ptr & 0x80) == 0) {
            // ASCII символ
            temp_utf8[pos++] = *ptr++;
        } else {
            // UTF-8 многобайтовый символ
            temp_utf8[pos++] = *ptr++;
            if ((*ptr & 0xC0) == 0x80) temp_utf8[pos++] = *ptr++;
        }
        temp_utf8[pos] = '\0';
        
        framebuffer.fillRect(10, y_pos, 220, 32, ST7789_Colors::BLACK);
        framebuffer.drawStringUTF8Scaled(10, y_pos, temp_utf8, ST7789_Colors::CYAN, 2);
        
        display.flushFramebuffer();
        HAL_Delay(150);
    }
    
    HAL_Delay(2000);
    
    // Эффект мигания
    for (int i = 0; i < 6; i++) {
        framebuffer.clear(ST7789_Colors::BLACK);
        if (i % 2 == 0) {
            framebuffer.drawStringScaled(10, 100, text_en, ST7789_Colors::RED, 2);
            framebuffer.drawStringUTF8Scaled(10, 150, text_ru, ST7789_Colors::YELLOW, 2);
        }
        display.flushFramebuffer();
        HAL_Delay(500);
    }
}

/**
 * @brief Демонстрация таблицы текста
 */
void demo_text_table() {
    framebuffer.clear(ST7789_Colors::BLACK);
    
    // Заголовок таблицы
    framebuffer.drawStringUTF8(10, 10, "ТАБЛИЦА СИМВОЛОВ", ST7789_Colors::WHITE);
    framebuffer.drawString(10, 30, "ASCII | UTF-8 | HEX", ST7789_Colors::CYAN);
    
    // Рисование линии
    framebuffer.drawLine(10, 45, 230, 45, ST7789_Colors::WHITE);
    
    // Примеры символов
    struct Symbol {
        char ascii;
        const char* utf8;
        uint16_t hex;
    };
    
    Symbol symbols[] = {
        {'A', "А", 0x0410},
        {'B', "В", 0x0412},
        {'E', "Е", 0x0415},
        {'P', "Р", 0x0420},
        {'C', "С", 0x0421},
        {'T', "Т", 0x0422},
        {'X', "Х", 0x0425}
    };
    
    for (int i = 0; i < 7; i++) {
        char line[32];
        snprintf(line, sizeof(line), " %c   |  ", symbols[i].ascii);
        framebuffer.drawString(10, 60 + i * 18, line, ST7789_Colors::GREEN);
        
        framebuffer.drawStringUTF8(50, 60 + i * 18, symbols[i].utf8, ST7789_Colors::YELLOW);
        
        snprintf(line, sizeof(line), "  | %04X", symbols[i].hex);
        framebuffer.drawString(70, 60 + i * 18, line, ST7789_Colors::MAGENTA);
    }
    
    display.flushFramebuffer();
    HAL_Delay(4000);
}

/**
 * @brief Главная функция демонстрации текста
 */
void run_text_demo() {
    init_text_demo();
    
    while (1) {
        // ASCII текст
        demo_ascii_text();
        
        // Кириллица
        demo_cyrillic_text();
        
        // Масштабирование
        demo_scaled_text();
        
        // Цветной текст  
        demo_colored_text();
        
        // Стили текста
        demo_text_styles();
        
        // Форматированный текст
        for (int i = 0; i < 10; i++) {
            demo_formatted_text();
        }
        
        // Анимированный текст
        demo_animated_text();
        
        // Таблица символов
        demo_text_table();
        
        HAL_Delay(2000);
    }
}

/**
 * @brief Простой пример текста для быстрого старта
 */
void quick_text_example() {
    display.init();
    framebuffer.init();
    display.setFramebuffer(&framebuffer);
    
    framebuffer.clear(ST7789_Colors::BLACK);
    
    // Английский текст
    framebuffer.drawString(10, 10, "Hello, World!", ST7789_Colors::WHITE);
    framebuffer.drawStringScaled(10, 40, "Big Text", ST7789_Colors::RED, 2);
    
    // Русский текст
    framebuffer.drawStringUTF8(10, 80, "Привет, мир!", ST7789_Colors::GREEN);
    framebuffer.drawStringUTF8Scaled(10, 110, "Большой", ST7789_Colors::BLUE, 2);
    
    display.flushFramebuffer();
}
