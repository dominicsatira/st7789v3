/**
 * @file framebuffer_demo.cpp
 * @brief Демонстрация работы с буфером кадра
 * 
 * Этот пример показывает:
 * - Создание и использование буфера кадра
 * - Плавную анимацию без мерцания
 * - Оптимизацию производительности
 * - Работу со статическим буфером
 */

#include "st7789v3.hpp"
#include "framebuffer.hpp"
#include "main.h"
#include <cmath>

// Внешние переменные из main.c
extern SPI_HandleTypeDef hspi1;

// Создание объектов
ST7789V3 display(&hspi1, 
                 ST7789_GPIO(GPIOA, GPIO_PIN_4),  // CS
                 ST7789_GPIO(GPIOA, GPIO_PIN_3),  // DC
                 ST7789_GPIO(GPIOA, GPIO_PIN_2)); // RST

Framebuffer framebuffer(240, 320);

/**
 * @brief Инициализация дисплея и буфера
 */
void init_framebuffer_demo() {
    // Инициализация дисплея
    display.init();
    display.fillScreen(ST7789_Colors::BLACK);
    
    // Инициализация буфера кадра
    if (framebuffer.init()) {
        display.setFramebuffer(&framebuffer);
        display.drawString(10, 10, "Framebuffer OK", ST7789_Colors::GREEN);
    } else {
        display.drawString(10, 10, "Framebuffer FAIL", ST7789_Colors::RED);
        display.drawString(10, 30, "Using static buffer", ST7789_Colors::YELLOW);
        initStaticFramebuffer();
    }
    
    HAL_Delay(1000);
}

/**
 * @brief Демонстрация простой анимации
 */
void demo_simple_animation() {
    uint16_t x = 20, y = 160;
    int16_t dx = 2, dy = 1;
    
    for (int frame = 0; frame < 500; frame++) {
        // Очистка буфера
        framebuffer.clear(ST7789_Colors::BLACK);
        
        // Рисование движущегося шарика
        framebuffer.fillCircle(x, y, 15, ST7789_Colors::RED);
        
        // Рисование следа
        if (frame > 10) {
            framebuffer.fillCircle(x - dx * 5, y - dy * 5, 8, ST7789_Colors::YELLOW);
        }
        if (frame > 20) {
            framebuffer.fillCircle(x - dx * 10, y - dy * 10, 4, ST7789_Colors::GREEN);
        }
        
        // Информация о кадре
        char frame_info[32];
        snprintf(frame_info, sizeof(frame_info), "Frame: %d", frame);
        framebuffer.drawString(10, 10, frame_info, ST7789_Colors::WHITE);
        framebuffer.drawStringUTF8(10, 30, "Анимация шарика", ST7789_Colors::CYAN);
        
        // Передача буфера на дисплей
        display.flushFramebuffer();
        
        // Обновление позиции
        x += dx;
        y += dy;
        
        // Отскок от границ
        if (x <= 15 || x >= 225) dx = -dx;
        if (y <= 15 || y >= 305) dy = -dy;
        
        HAL_Delay(20);
    }
}

/**
 * @brief Демонстрация сложной анимации с несколькими объектами
 */
void demo_complex_animation() {
    struct Ball {
        float x, y;
        float vx, vy;
        uint16_t color;
        uint8_t radius;
    };
    
    Ball balls[] = {
        {60, 80, 1.5f, 1.0f, ST7789_Colors::RED, 12},
        {120, 160, -1.2f, 1.5f, ST7789_Colors::GREEN, 10},
        {180, 240, 0.8f, -1.8f, ST7789_Colors::BLUE, 8},
        {100, 200, -1.0f, -1.2f, ST7789_Colors::YELLOW, 15},
        {200, 100, 1.8f, 0.5f, ST7789_Colors::CYAN, 7}
    };
    
    const int num_balls = sizeof(balls) / sizeof(Ball);
    
    for (int frame = 0; frame < 1000; frame++) {
        // Очистка буфера с эффектом затухания
        framebuffer.clear(ST7789_Colors::BLACK);
        
        // Обновление и рисование шариков
        for (int i = 0; i < num_balls; i++) {
            Ball* ball = &balls[i];
            
            // Обновление позиции
            ball->x += ball->vx;
            ball->y += ball->vy;
            
            // Отскоки от границ
            if (ball->x <= ball->radius || ball->x >= 240 - ball->radius) {
                ball->vx = -ball->vx;
            }
            if (ball->y <= ball->radius || ball->y >= 320 - ball->radius) {
                ball->vy = -ball->vy;
            }
            
            // Ограничение в границах
            if (ball->x < ball->radius) ball->x = ball->radius;
            if (ball->x > 240 - ball->radius) ball->x = 240 - ball->radius;
            if (ball->y < ball->radius) ball->y = ball->radius;
            if (ball->y > 320 - ball->radius) ball->y = 320 - ball->radius;
            
            // Рисование шарика
            framebuffer.fillCircle((uint16_t)ball->x, (uint16_t)ball->y, 
                                 ball->radius, ball->color);
            
            // Рисование контура
            framebuffer.drawCircle((uint16_t)ball->x, (uint16_t)ball->y, 
                                 ball->radius + 1, ST7789_Colors::WHITE);
        }
        
        // Информация о производительности
        char info[64];
        snprintf(info, sizeof(info), "Balls: %d, Frame: %d", num_balls, frame);
        framebuffer.drawString(5, 5, info, ST7789_Colors::WHITE, ST7789_Colors::BLACK);
        framebuffer.drawStringUTF8(5, 25, "Множественная анимация", 
                                 ST7789_Colors::WHITE, ST7789_Colors::BLACK);
        
        // Передача буфера
        display.flushFramebuffer();
        
        HAL_Delay(30);
    }
}

/**
 * @brief Демонстрация синусоидальной анимации
 */
void demo_sine_wave_animation() {
    float phase = 0;
    
    for (int frame = 0; frame < 628; frame++) { // 2*PI * 100
        framebuffer.clear(ST7789_Colors::BLACK);
        
        // Рисование синусоиды
        for (int x = 0; x < 240; x++) {
            float sine_val = sin((x * 0.05f) + phase);
            int y = 160 + (int)(sine_val * 80);
            
            if (y >= 0 && y < 320) {
                framebuffer.setPixel(x, y, ST7789_Colors::GREEN);
                framebuffer.setPixel(x, y + 1, ST7789_Colors::GREEN);
            }
        }
        
        // Движущаяся точка по синусоиде
        int marker_x = 120;
        float marker_sine = sin((marker_x * 0.05f) + phase);
        int marker_y = 160 + (int)(marker_sine * 80);
        
        framebuffer.fillCircle(marker_x, marker_y, 8, ST7789_Colors::RED);
        
        // Информация
        char phase_info[32];
        snprintf(phase_info, sizeof(phase_info), "Phase: %.2f", phase);
        framebuffer.drawString(10, 10, phase_info, ST7789_Colors::WHITE);
        framebuffer.drawStringUTF8(10, 30, "Синусоида", ST7789_Colors::CYAN);
        
        display.flushFramebuffer();
        
        phase += 0.1f;
        HAL_Delay(50);
    }
}

/**
 * @brief Демонстрация работы со статическим буфером
 */
void demo_static_buffer() {
    // Инициализация статического буфера
    initStaticFramebuffer();
    
    for (int i = 0; i < 200; i++) {
        // Очистка статического буфера
        clearStaticFramebuffer(ST7789_Colors::BLACK);
        
        // Рисование в статический буфер
        int x = 120 + (int)(100 * cos(i * 0.1f));
        int y = 68 + (int)(50 * sin(i * 0.1f));
        
        // Ограничение координат размерами статического буфера
        if (x < 0) x = 0;
        if (x >= 240) x = 239;
        if (y < 0) y = 0;
        if (y >= 136) y = 135;
        
        drawStaticRect(x - 10, y - 10, 20, 20, ST7789_Colors::BLUE);
        drawStaticLine(0, y, 239, y, ST7789_Colors::GREEN);
        drawStaticLine(x, 0, x, 135, ST7789_Colors::RED);
        
        // Текст в статическом буфере
        drawStaticString(10, 10, "Static Buffer", ST7789_Colors::WHITE);
        drawStaticStringUTF8(10, 30, "Статический буфер", ST7789_Colors::YELLOW);
        
        char pos_info[32];
        snprintf(pos_info, sizeof(pos_info), "X:%d Y:%d", x, y);
        drawStaticString(10, 50, pos_info, ST7789_Colors::CYAN);
        
        // Передача статического буфера на дисплей
        display.flushStaticBuffer(getStaticFramebuffer(), 240, 136);
        
        HAL_Delay(50);
    }
}

/**
 * @brief Сравнение производительности с буфером и без
 */
void demo_performance_comparison() {
    uint32_t start_time, end_time;
    
    // Тест без буфера (прямое рисование)
    display.setFramebuffer(nullptr);
    
    start_time = HAL_GetTick();
    
    for (int i = 0; i < 100; i++) {
        display.drawPixel(rand() % 240, rand() % 320, rand() % 0xFFFF);
    }
    
    end_time = HAL_GetTick();
    uint32_t time_without_buffer = end_time - start_time;
    
    // Тест с буфером
    display.setFramebuffer(&framebuffer);
    
    start_time = HAL_GetTick();
    
    framebuffer.clear(ST7789_Colors::BLACK);
    for (int i = 0; i < 100; i++) {
        framebuffer.setPixel(rand() % 240, rand() % 320, rand() % 0xFFFF);
    }
    display.flushFramebuffer();
    
    end_time = HAL_GetTick();
    uint32_t time_with_buffer = end_time - start_time;
    
    // Отображение результатов
    framebuffer.clear(ST7789_Colors::BLACK);
    
    framebuffer.drawStringUTF8(10, 10, "Тест производительности", ST7789_Colors::WHITE);
    framebuffer.drawString(10, 40, "100 pixels drawing:", ST7789_Colors::YELLOW);
    
    char result1[32], result2[32], speedup[32];
    snprintf(result1, sizeof(result1), "Direct: %lu ms", time_without_buffer);
    snprintf(result2, sizeof(result2), "Buffered: %lu ms", time_with_buffer);
    snprintf(speedup, sizeof(speedup), "Speedup: %.1fx", 
             (float)time_without_buffer / time_with_buffer);
    
    framebuffer.drawString(10, 70, result1, ST7789_Colors::RED);
    framebuffer.drawString(10, 90, result2, ST7789_Colors::GREEN);
    framebuffer.drawString(10, 110, speedup, ST7789_Colors::CYAN);
    
    display.flushFramebuffer();
    HAL_Delay(3000);
}

/**
 * @brief Демонстрация частичного обновления экрана
 */
void demo_partial_update() {
    framebuffer.clear(ST7789_Colors::BLACK);
    
    // Рисование статичного фона
    framebuffer.drawRect(0, 0, 240, 320, ST7789_Colors::WHITE);
    framebuffer.drawStringUTF8(10, 10, "Частичное обновление", ST7789_Colors::WHITE);
    framebuffer.drawString(10, 30, "Static background", ST7789_Colors::GREEN);
    
    // Полное обновление фона
    display.flushFramebuffer();
    
    // Анимация в определенной области
    for (int i = 0; i < 100; i++) {
        // Очистка только области анимации
        framebuffer.fillRect(50, 100, 140, 100, ST7789_Colors::BLACK);
        
        // Рисование анимированного объекта
        int x = 120 + (int)(60 * cos(i * 0.2f));
        int y = 150 + (int)(40 * sin(i * 0.2f));
        
        framebuffer.fillCircle(x, y, 15, ST7789_Colors::RED);
        framebuffer.drawCircle(x, y, 20, ST7789_Colors::YELLOW);
        
        // Обновление только измененной области
        display.flushFramebufferRegion(50, 100, 140, 100);
        
        HAL_Delay(50);
    }
}

/**
 * @brief Главная функция демонстрации буфера кадра
 */
void run_framebuffer_demo() {
    // Инициализация
    init_framebuffer_demo();
    
    while (1) {
        // Простая анимация
        demo_simple_animation();
        HAL_Delay(1000);
        
        // Сложная анимация
        demo_complex_animation();
        HAL_Delay(1000);
        
        // Синусоидальная анимация
        demo_sine_wave_animation();
        HAL_Delay(1000);
        
        // Работа со статическим буфером
        demo_static_buffer();
        HAL_Delay(1000);
        
        // Сравнение производительности
        demo_performance_comparison();
        HAL_Delay(2000);
        
        // Частичное обновление
        demo_partial_update();
        HAL_Delay(2000);
    }
}

/**
 * @brief Простой пример для быстрого старта с буфером
 */
void quick_framebuffer_example() {
    // Инициализация
    display.init();
    framebuffer.init();
    display.setFramebuffer(&framebuffer);
    
    // Анимация
    for (int i = 0; i < 100; i++) {
        // Очистка буфера
        framebuffer.clear(ST7789_Colors::BLACK);
        
        // Рисование в буфер
        framebuffer.drawString(10, 10, "Framebuffer Demo", ST7789_Colors::WHITE);
        framebuffer.fillCircle(120 + i, 160, 20, ST7789_Colors::RED);
        
        // Передача на дисплей
        display.flushFramebuffer();
        
        HAL_Delay(50);
    }
}
