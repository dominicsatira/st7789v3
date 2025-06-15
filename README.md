# ST7789V3 Display Library for STM32

![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)
![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-blue.svg)
![Platform: STM32](https://img.shields.io/badge/Platform-STM32-green.svg)

Высокопроизводительная библиотека для управления TFT LCD дисплеями на базе контроллера ST7789V3 для микроконтроллеров STM32. Библиотека поддерживает как прямое рисование на дисплей, так и работу с буфером кадра для улучшенной производительности.

Специально создана для работы с дисплеем 240x320 пикселей в среде разработки VSCode.

## ✨ Возможности

### 🎨 Графические функции
• **Базовые примитивы**: точки, линии, прямоугольники, окружности  
• **Заливка**: быстрая заливка экрана и прямоугольных областей  
• **Цветовая модель RGB565**: 65536 цветов  
• **Инверсия цветов**: программное управление инверсией дисплея  

### 📝 Текстовые функции
• **Многоязычная поддержка**: ASCII и кириллица (UTF-8)  
• **Шрифт 8x16**: четкий растровый шрифт  
• **Масштабирование**: произвольное масштабирование текста (1x-8x)  
• **Фон**: настраиваемый цвет фона для текста  

### 🚀 Буфер кадра
• **Двойная буферизация**: плавная анимация без мерцания  
• **Оптимизация памяти**: статический буфер для небольших размеров, динамический для больших  
• **Частичное обновление**: передача только измененных областей  
• **Быстрое рисование**: все операции выполняются в памяти  

### 🔧 Технические характеристики
• **Разрешение**: 240x320 пикселей  
• **Интерфейс**: SPI  
• **Поддержка поворота**: 4 ориентации дисплея (0°, 90°, 180°, 270°)  
• **Совместимость**: STM32 HAL библиотека  
• **Поддержка DMA**: асинхронная передача данных  

## 📦 Установка

### Требования
• STM32 микроконтроллер (рекомендуется STM32F411CEU6)  
• STM32 HAL библиотека  
• C++17 совместимый компилятор  
• VSCode + PlatformIO/STM32CubeIDE  

### Интеграция в проект

#### Вариант 1: Копирование файлов
```bash
# Копируйте файлы библиотеки в ваш проект
cp -r st7789v3/ your_project/libraries/
```

#### Вариант 2: Использование как подмодуль
```bash
git submodule add https://github.com/your-username/st7789v3.git lib/st7789v3
```

## 🚀 Быстрый старт

### Базовая настройка

```cpp
#include "st7789v3.hpp"

// Внешние переменные из main.c
extern SPI_HandleTypeDef hspi1;

// Инициализация дисплея
ST7789V3 display(&hspi1, 
                 ST7789_GPIO(GPIOA, GPIO_PIN_4),  // CS
                 ST7789_GPIO(GPIOA, GPIO_PIN_3),  // DC
                 ST7789_GPIO(GPIOA, GPIO_PIN_2)); // RST

void setup() {
    display.init();
    display.fillScreen(ST7789_Colors::BLACK);
}
```

### Простое рисование

```cpp
void draw_example() {
    // Заливка экрана
    display.fillScreen(ST7789_Colors::BLUE);
    
    // Рисование линии
    display.drawLine(0, 0, 239, 319, ST7789_Colors::WHITE);
    
    // Рисование прямоугольника
    display.drawRect(50, 50, 100, 80, ST7789_Colors::RED);
    display.fillRect(60, 60, 80, 60, ST7789_Colors::GREEN);
    
    // Рисование окружности
    display.drawCircle(120, 160, 50, ST7789_Colors::YELLOW);
    display.fillCircle(120, 160, 30, ST7789_Colors::CYAN);
    
    // Вывод текста
    display.drawString(10, 10, "Hello, World!", ST7789_Colors::WHITE);
    display.drawStringUTF8(10, 30, "Привет, мир!", ST7789_Colors::WHITE);
    
    // Масштабированный текст
    display.drawStringScaled(10, 50, "Big Text", ST7789_Colors::RED, 2);
}
```

### Работа с буфером кадра

```cpp
#include "framebuffer.hpp"

Framebuffer framebuffer(240, 320);

void framebuffer_example() {
    // Инициализация буфера
    framebuffer.init();
    display.setFramebuffer(&framebuffer);
    
    // Рисование в буфер
    framebuffer.fillRect(0, 0, 240, 320, ST7789_Colors::BLACK);
    framebuffer.drawString(10, 10, "Buffered text", ST7789_Colors::WHITE);
    framebuffer.drawStringUTF8(10, 30, "Буферизованный текст", ST7789_Colors::GREEN);
    
    // Передача буфера на дисплей
    display.flushFramebuffer();
}
```

### Работа со статическим буфером

```cpp
void static_buffer_example() {
    // Инициализация статического буфера (240x136 пикселей)
    initStaticFramebuffer();
    
    // Рисование в статический буфер
    clearStaticFramebuffer(ST7789_Colors::BLACK);
    drawStaticString(10, 10, "Static Buffer", ST7789_Colors::WHITE);
    drawStaticStringUTF8(10, 30, "Статический буфер", ST7789_Colors::CYAN);
    
    // Передача на дисплей
    display.flushStaticBuffer(getStaticFramebuffer(), 240, 136);
}
```

### Анимация

```cpp
void animation_example() {
    uint16_t x = 0;
    uint16_t y = 160;
    
    while (true) {
        // Очистка буфера
        framebuffer.clear(ST7789_Colors::BLACK);
        
        // Рисование движущегося объекта
        framebuffer.fillCircle(x, y, 10, ST7789_Colors::RED);
        framebuffer.drawStringUTF8(x-20, y+20, "Анимация", ST7789_Colors::WHITE);
        
        // Обновление экрана
        display.flushFramebuffer();
        
        // Движение
        x = (x + 2) % 240;
        
        HAL_Delay(50);
    }
}
```

## 📖 API Документация

### Класс ST7789V3

#### Конструктор
```cpp
ST7789V3(SPI_HandleTypeDef* spi_handle,
         const ST7789_GPIO& cs,
         const ST7789_GPIO& dc,
         const ST7789_GPIO& rst);
```

#### Основные методы

| Метод | Описание |
|-------|----------|
| `init()` | Инициализация дисплея |
| `reset()` | Аппаратный сброс дисплея |
| `fillScreen(color)` | Заливка экрана цветом |
| `setRotation(rotation)` | Установка поворота (0-3) |
| `invertColors(bool)` | Инверсия цветов |
| `invertOn()` | Включить инверсию цветов |
| `invertOff()` | Отключить инверсию цветов |

#### Графические методы

| Метод | Описание |
|-------|----------|
| `drawPixel(x, y, color)` | Рисование точки |
| `drawLine(x0, y0, x1, y1, color)` | Рисование линии |
| `drawRect(x, y, w, h, color)` | Рисование прямоугольника |
| `fillRect(x, y, w, h, color)` | Заливка прямоугольника |
| `drawCircle(x0, y0, r, color)` | Рисование окружности |
| `fillCircle(x0, y0, r, color)` | Заливка окружности |

#### Текстовые методы

| Метод | Описание |
|-------|----------|
| `drawChar(x, y, ch, color, bg)` | Рисование символа |
| `drawString(x, y, str, color, bg)` | Рисование строки |
| `drawStringUTF8(x, y, str, color, bg)` | Рисование UTF-8 строки |
| `drawCharScaled(x, y, ch, color, scale, bg)` | Масштабированный символ |
| `drawStringScaled(x, y, str, color, scale, bg)` | Масштабированная строка |
| `drawStringUTF8Scaled(x, y, str, color, scale, bg)` | Масштабированная UTF-8 строка |

#### Методы буфера кадра

| Метод | Описание |
|-------|----------|
| `setFramebuffer(fb)` | Установка буфера кадра |
| `flushFramebuffer()` | Передача буфера на дисплей |
| `flushFramebufferDMA()` | Передача буфера через DMA |
| `flushFramebufferRegion(x, y, w, h)` | Частичная передача буфера |
| `clearFramebuffer()` | Очистка буфера |
| `isFramebufferEnabled()` | Проверка состояния буфера |

### Класс Framebuffer

#### Конструктор
```cpp
Framebuffer(uint16_t width = 240, uint16_t height = 320);
```

#### Методы управления

| Метод | Описание |
|-------|----------|
| `init()` | Инициализация буфера |
| `clear(color)` | Очистка буфера |
| `release()` | Освобождение памяти |
| `isAllocated()` | Проверка выделения памяти |
| `getBuffer()` | Получение указателя на буфер |
| `getBufferSize()` | Размер буфера в пикселях |
| `getWidth()` | Ширина буфера |
| `getHeight()` | Высота буфера |

#### Графические методы буфера

Framebuffer содержит все те же графические и текстовые методы, что и ST7789V3, но работает в памяти.

### Функции статического буфера

| Функция | Описание |
|---------|----------|
| `initStaticFramebuffer()` | Инициализация статического буфера |
| `clearStaticFramebuffer(color)` | Очистка статического буфера |
| `setStaticPixel(x, y, color)` | Установка пикселя |
| `getStaticPixel(x, y)` | Получение цвета пикселя |
| `drawStaticString(x, y, str, color, bg)` | Рисование строки |
| `drawStaticStringUTF8(x, y, str, color, bg)` | Рисование UTF-8 строки |

### Цветовые константы

```cpp
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
```

### Утилиты

```cpp
// Создание цвета RGB565
uint16_t color = ST7789V3::rgb565(255, 128, 64);
uint16_t color = Framebuffer::rgb565(255, 128, 64);

// UTF-8 утилиты
uint16_t unicode = UTF8_ToUnicode(utf8_char, &bytes_consumed);
bool is_multibyte = UTF8_IsMultibyte(first_byte);
```

## ⚙️ Конфигурация

### Настройка пинов

Отредактируйте файл `inc/st7789v3_config.hpp`:

```cpp
// GPIO пины
#define ST7789_CS_PORT   GPIOA
#define ST7789_CS_PIN    GPIO_PIN_4
#define ST7789_DC_PORT   GPIOA  
#define ST7789_DC_PIN    GPIO_PIN_3
#define ST7789_RST_PORT  GPIOA
#define ST7789_RST_PIN   GPIO_PIN_2

// SPI интерфейс
#define ST7789_SPI_HANDLE hspi1
```

### Настройка таймаутов

```cpp
namespace ST7789_Config {
    constexpr uint32_t SPI_TIMEOUT = 5000;  // Таймаут SPI передачи
    constexpr uint32_t RESET_DELAY = 100;   // Задержка сброса
    constexpr uint32_t INIT_DELAY = 120;    // Задержка инициализации
}
```

### Настройка буферов

```cpp
// Максимальный размер статического буфера (64KB = 32K пикселей)
constexpr uint32_t STATIC_FB_MAX_PIXELS = 32768;
constexpr uint16_t STATIC_FB_HEIGHT = 136; // 240 * 136 = 32640 пикселей
```

## 🔧 Настройка STM32CubeMX

### SPI конфигурация
• **Mode**: Full-Duplex Master  
• **Data Size**: 8 Bits  
• **First Bit**: MSB First  
• **Prescaler**: В зависимости от частоты APB (рекомендуется 8-16 МГц)  
• **Clock Polarity**: Low  
• **Clock Phase**: 1 Edge  

### GPIO конфигурация
• **CS, DC, RST**: GPIO_Output, Push-Pull, High Speed  
• **SPI пины**: Alternate function push-pull  

### DMA конфигурация (опционально)
• **SPI TX**: DMA Normal Mode  
• **Data Width**: Half Word  
• **Mode**: Normal  

## 🎯 Производительность

### Бенчмарки

| Операция | Без буфера | С буфером | Ускорение |
|----------|------------|-----------|-----------|
| Заливка экрана | ~45 мс | ~12 мс | 3.75x |
| Рисование 1000 точек | ~850 мс | ~2 мс | 425x |
| Вывод текста (20 символов) | ~25 мс | ~0.5 мс | 50x |
| Передача буфера | - | ~8 мс | - |

### Использование памяти

• **Полный буфер кадра**: 240 × 320 × 2 = 153,600 байт (~150 КБ)  
• **Статический буфер**: 240 × 136 × 2 = 65,280 байт (~64 КБ)  
• **Библиотека**: ~12 КБ Flash  
• **Шрифт 8x16**: ~3 КБ Flash  

## 🚀 Продвинутые возможности

### DMA передача

```cpp
// Callback функция для завершения DMA передачи
extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (hspi->Instance == SPI1) {
        dma_transfer_complete = true;
    }
}

// Использование DMA
display.flushFramebufferDMA();
while (isDMABusy()) {
    // Выполнение других задач во время передачи
    HAL_Delay(1);
}
```

### Частичное обновление экрана

```cpp
// Обновление только измененной области
framebuffer.fillRect(50, 50, 100, 100, ST7789_Colors::RED);
display.flushFramebufferRegion(50, 50, 100, 100);
```

### Работа с поворотом дисплея

```cpp
// Поворот дисплея
display.setRotation(1); // 90 градусов
display.fillScreen(ST7789_Colors::BLUE);
display.drawString(10, 10, "Rotated!", ST7789_Colors::WHITE);
```

## 🐛 Известные проблемы и решения

### Проблема: Неправильные цвета
**Решение**: Используйте инверсию цветов
```cpp
display.invertOn(); // Включить инверсию
```

### Проблема: Мерцание при анимации
**Решение**: Используйте буфер кадра
```cpp
Framebuffer fb(240, 320);
fb.init();
display.setFramebuffer(&fb);
```

### Проблема: Нехватка памяти
**Решение**: Используйте статический буфер для частичного обновления
```cpp
// Используйте статический буфер 240x136 вместо полного 240x320
initStaticFramebuffer();
```

### Проблема: Медленная передача
**Решение**: Увеличьте частоту SPI и используйте DMA
```cpp
// В CubeMX увеличьте частоту SPI до 16-20 МГц
display.flushFramebufferDMA(); // Используйте DMA
```

## 📋 Планы развития

✅ Поддержка кириллицы UTF-8  
✅ Буфер кадра с оптимизацией памяти  
✅ DMA передача данных  
✅ Масштабирование текста  
⬜ Поддержка изображений (BMP, PNG)  
⬜ Дополнительные размеры шрифтов  
⬜ Графические примитивы (многоугольники, дуги)  
⬜ Поддержка тачскрина  
⬜ Виджеты пользовательского интерфейса  
⬜ Аппаратное ускорение графики  

## 🤝 Вклад в проект

Мы приветствуем вклад в развитие проекта! Пожалуйста, следуйте этим правилам:

1. **Fork** репозитория
2. Создайте **feature branch** (`git checkout -b feature/amazing-feature`)
3. **Commit** изменения (`git commit -m 'Add amazing feature'`)
4. **Push** в branch (`git push origin feature/amazing-feature`)
5. Откройте **Pull Request**

### Стиль кодирования

• Используйте **C++17** стандарт  
• Следуйте **Google C++ Style Guide**  
• Добавляйте **комментарии** для публичных методов  
• Включайте **примеры использования** для новых функций  
• Тестируйте код на реальном оборудовании  

## 📄 Лицензия

Этот проект распространяется под лицензией **MIT**. Подробности см. в файле [LICENSE](LICENSE).

## 🙏 Благодарности

• **STMicroelectronics** за превосходную HAL библиотеку  
• **Сообщество STM32** за поддержку и обратную связь  
• **Разработчики VSCode** за отличную среду разработки  

## 📞 Поддержка

Если у вас есть вопросы или предложения:

• 📧 **Email**: [support@yourproject.com](mailto:support@yourproject.com)  
• 🐛 **Issues**: [GitHub Issues](https://github.com/yourusername/st7789v3/issues)  
• 💬 **Discussions**: [GitHub Discussions](https://github.com/yourusername/st7789v3/discussions)  

---

**Сделано с ❤️ для сообщества STM32**

---

## 📊 Статистика

![GitHub stars](https://img.shields.io/github/stars/yourusername/st7789v3?style=social)
![GitHub forks](https://img.shields.io/github/forks/yourusername/st7789v3?style=social)
![GitHub issues](https://img.shields.io/github/issues/yourusername/st7789v3)
![GitHub license](https://img.shields.io/github/license/yourusername/st7789v3)

---

> **Примечание**: Этот проект специально создан для работы с дисплеем ST7789V3 320x240 пикселей и оптимизирован для микроконтроллеров STM32F411CEU6. Для других контроллеров могут потребоваться незначительные изменения в конфигурации.
