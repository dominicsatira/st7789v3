# ST7789V3 Library Examples

Эта папка содержит примеры использования библиотеки ST7789V3 для микроконтроллеров STM32.

## 📁 Список примеров

### 1. `basic_drawing.cpp` - Базовое рисование
Демонстрирует основные возможности библиотеки:
- Инициализация дисплея
- Основные графические примитивы (точки, линии, прямоугольники, окружности)  
- Вывод текста (ASCII и UTF-8)
- Работа с цветами и инверсией
- Поворот дисплея

**Функции для вызова:**
- `run_basic_drawing_demo()` - полная демонстрация
- `quick_start_example()` - быстрый пример

### 2. `framebuffer_demo.cpp` - Работа с буфером кадра
Показывает преимущества использования буфера кадра:
- Плавная анимация без мерцания
- Сложные анимации с несколькими объектами
- Работа со статическим буфером
- Сравнение производительности
- Частичное обновление экрана

**Функции для вызова:**
- `run_framebuffer_demo()` - полная демонстрация
- `quick_framebuffer_example()` - простой пример

### 3. `text_demo.cpp` - Работа с текстом
Демонстрирует текстовые возможности:
- ASCII и кириллические символы
- Масштабирование текста (1x-8x)
- UTF-8 кодировка
- Цветной текст и фон
- Анимированный текст
- Форматированный вывод

**Функции для вызова:**
- `run_text_demo()` - полная демонстрация
- `quick_text_example()` - простой пример

## 🚀 Как использовать

### Вариант 1: Включение в main.cpp

```cpp
#include "main.h"
#include "examples/basic_drawing.cpp"
#include "examples/framebuffer_demo.cpp"  
#include "examples/text_demo.cpp"

int main(void) {
    // Инициализация системы
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    
    // Запуск демонстрации
    run_basic_drawing_demo();
    // или
    // run_framebuffer_demo();
    // или  
    // run_text_demo();
    
    while (1) {
        // Основной цикл
    }
}
```

### Вариант 2: Отдельные функции

```cpp
#include "examples/basic_drawing.cpp"

int main(void) {
    // Инициализация...
    
    // Быстрый пример
    quick_start_example();
    
    while (1) {
        // Ваш код
    }
}
```

### Вариант 3: Выборочное использование

```cpp
// Скопируйте нужные функции из примеров в ваш код
void my_display_function() {
    ST7789V3 display(&hspi1, 
                     ST7789_GPIO(GPIOA, GPIO_PIN_4),
                     ST7789_GPIO(GPIOA, GPIO_PIN_3),
                     ST7789_GPIO(GPIOA, GPIO_PIN_2));
    
    display.init();
    display.fillScreen(ST7789_Colors::BLUE);
    display.drawString(10, 10, "My App", ST7789_Colors::WHITE);
}
```

## ⚙️ Настройка

Убедитесь, что у вас правильно настроены:

### В CubeMX:
- **SPI1**: Full-Duplex Master, 8-bit, MSB First
- **GPIO**: PA2 (RST), PA3 (DC), PA4 (CS) как GPIO_Output
- **Частота SPI**: 8-16 МГц (рекомендуется)

### В коде:
```cpp
// Проверьте пины в файле st7789v3_config.hpp
#define ST7789_CS_PORT   GPIOA
#define ST7789_CS_PIN    GPIO_PIN_4
#define ST7789_DC_PORT   GPIOA  
#define ST7789_DC_PIN    GPIO_PIN_3
#define ST7789_RST_PORT  GPIOA
#define ST7789_RST_PIN   GPIO_PIN_2
```

## 📋 Требования к памяти

| Пример | RAM | Flash | Примечание |
|---------|-----|-------|------------|
| basic_drawing | ~2 КБ | ~15 КБ | Прямое рисование |
| framebuffer_demo | ~150 КБ | ~20 КБ | Полный буфер кадра |
| text_demo | ~150 КБ | ~18 КБ | Буфер + текст |

**Примечание**: Если у вас недостаточно RAM для полного буфера кадра, используйте статический буфер (64 КБ) или прямое рисование.

## 🎨 Модификация примеров

### Изменение пинов подключения:
```cpp
ST7789V3 display(&hspi1, 
                 ST7789_GPIO(GPIOB, GPIO_PIN_12),  // CS -> PB12
                 ST7789_GPIO(GPIOB, GPIO_PIN_13),  // DC -> PB13  
                 ST7789_GPIO(GPIOB, GPIO_PIN_14)); // RST -> PB14
```

### Изменение размера буфера:
```cpp
// Для дисплея другого размера
Framebuffer framebuffer(320, 240);  // 320x240 вместо 240x320
```

### Добавление собственных функций:
```cpp
void my_custom_demo() {
    framebuffer.clear(ST7789_Colors::BLACK);
    
    // Ваш код рисования
    framebuffer.drawString(10, 10, "My Demo", ST7789_Colors::WHITE);
    
    display.flushFramebuffer();
}
```

## 🐛 Решение проблем

### Дисплей не инициализируется:
- Проверьте подключение проводов
- Убедитесь, что SPI настроен правильно
- Проверьте питание дисплея (3.3V)

### Неправильные цвета:
```cpp
// Попробуйте инверсию цветов
display.invertOn();
```

### Мерцание при анимации:
```cpp
// Используйте буфер кадра
Framebuffer framebuffer(240, 320);
framebuffer.init();
display.setFramebuffer(&framebuffer);
```

### Нехватка памяти:
```cpp
// Используйте статический буфер
initStaticFramebuffer();
// или прямое рисование без буфера
display.setFramebuffer(nullptr);
```

## 📚 Дополнительные ресурсы

- [README.md](../README.md) - Основная документация
- [st7789v3.hpp](../inc/st7789v3.hpp) - API reference
- [framebuffer.hpp](../framebuffer/framebuffer.hpp) - Документация буфера кадра

## 💡 Советы по оптимизации

1. **Используйте буфер кадра** для сложной графики
2. **Группируйте операции рисования** перед обновлением дисплея
3. **Используйте частичное обновление** для небольших изменений
4. **Оптимизируйте частоту SPI** (не более 20 МГц)
5. **Используйте статический буфер** при нехватке RAM

---

**Удачи в изучении библиотеки ST7789V3! 🚀**
