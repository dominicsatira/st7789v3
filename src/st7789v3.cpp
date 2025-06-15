#include "st7789v3.hpp"
#include "font8x16.hpp"
#include "../framebuffer/framebuffer.hpp"
#include <algorithm>
#include <cmath>

ST7789V3::ST7789V3(SPI_HandleTypeDef* spi_handle,
                   const ST7789_GPIO& cs,
                   const ST7789_GPIO& dc,
                   const ST7789_GPIO& rst)
    : hspi_(spi_handle), cs_pin_(cs), dc_pin_(dc), rst_pin_(rst), framebuffer_(nullptr) {
}

void ST7789V3::init() {
    reset();
    HAL_Delay(ST7789_Config::INIT_DELAY);
    
    // Выход из режима сна
    writeCommand(ST7789_Commands::SLPOUT);
    HAL_Delay(ST7789_Config::INIT_DELAY);
    
    // Настройка цветового режима (RGB565)
    writeCommand(ST7789_Commands::COLMOD);
    writeData8(0x55);
    
    // Настройка ориентации
    writeCommand(ST7789_Commands::MADCTL);
    writeData8(0x00);
    
    // Включение дисплея
    writeCommand(ST7789_Commands::DISPON);
    HAL_Delay(100);
    
    // Автоматическое включение инверсии для правильных цветов
    writeCommand(ST7789_Commands::INVON);
    
    // Очистка экрана
    fillScreen(ST7789_Colors::BLACK);
}

void ST7789V3::reset() {
    HAL_GPIO_WritePin(rst_pin_.port, rst_pin_.pin, GPIO_PIN_RESET);
    HAL_Delay(ST7789_Config::RESET_DELAY);
    HAL_GPIO_WritePin(rst_pin_.port, rst_pin_.pin, GPIO_PIN_SET);
    HAL_Delay(ST7789_Config::RESET_DELAY);
}

void ST7789V3::writeCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(dc_pin_.port, dc_pin_.pin, GPIO_PIN_RESET); // DC = 0 для команды
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_RESET); // CS = 0
    HAL_SPI_Transmit(hspi_, &cmd, 1, ST7789_Config::SPI_TIMEOUT);
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_SET);   // CS = 1
}

void ST7789V3::writeData8(uint8_t data) {
    HAL_GPIO_WritePin(dc_pin_.port, dc_pin_.pin, GPIO_PIN_SET);   // DC = 1 для данных
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_RESET); // CS = 0
    HAL_SPI_Transmit(hspi_, &data, 1, ST7789_Config::SPI_TIMEOUT);
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_SET);   // CS = 1
}

void ST7789V3::writeData16(uint16_t data) {
    uint8_t buffer[2] = {static_cast<uint8_t>((data >> 8) & 0xFF), 
                         static_cast<uint8_t>(data & 0xFF)};
    HAL_GPIO_WritePin(dc_pin_.port, dc_pin_.pin, GPIO_PIN_SET);   // DC = 1 для данных
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_RESET); // CS = 0
    HAL_SPI_Transmit(hspi_, buffer, 2, ST7789_Config::SPI_TIMEOUT);
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_SET);   // CS = 1
}

void ST7789V3::setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // Установка колонок
    writeCommand(ST7789_Commands::CASET);
    writeData16(x0);
    writeData16(x1);
    
    // Установка строк
    writeCommand(ST7789_Commands::RASET);
    writeData16(y0);
    writeData16(y1);
    
    // Начало записи в память
    writeCommand(ST7789_Commands::RAMWR);
}

void ST7789V3::fillScreen(uint16_t color) {
    fillRect(0, 0, ST7789_WIDTH, ST7789_HEIGHT, color);
}

void ST7789V3::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x + w > ST7789_WIDTH || y + h > ST7789_HEIGHT) return;
    
    setWindow(x, y, x + w - 1, y + h - 1);
    
    uint32_t pixel_count = static_cast<uint32_t>(w) * h;
    uint8_t color_buffer[2] = {static_cast<uint8_t>((color >> 8) & 0xFF), 
                               static_cast<uint8_t>(color & 0xFF)};
    
    HAL_GPIO_WritePin(dc_pin_.port, dc_pin_.pin, GPIO_PIN_SET);   // DC = 1
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_RESET); // CS = 0
    
    for (uint32_t i = 0; i < pixel_count; i++) {
        HAL_SPI_Transmit(hspi_, color_buffer, 2, ST7789_Config::SPI_TIMEOUT);
    }
    
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_SET);   // CS = 1
}

void ST7789V3::drawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x >= ST7789_WIDTH || y >= ST7789_HEIGHT) return;
    
    setWindow(x, y, x, y);
    writeData16(color);
}

void ST7789V3::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    int16_t dx = abs(static_cast<int16_t>(x1) - static_cast<int16_t>(x0));
    int16_t dy = abs(static_cast<int16_t>(y1) - static_cast<int16_t>(y0));
    int16_t sx = (x0 < x1) ? 1 : -1;
    int16_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx - dy;
    
    while (true) {
        drawPixel(x0, y0, color);
        
        if (x0 == x1 && y0 == y1) break;
        
        int16_t e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void ST7789V3::drawChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg_color) {
    if (x + ST7789_Font::CHAR_WIDTH > ST7789_WIDTH || 
        y + ST7789_Font::CHAR_HEIGHT > ST7789_HEIGHT) return;
    
    const uint8_t* font_data = Font8x16_GetChar(static_cast<uint8_t>(ch));
    
    for (uint8_t row = 0; row < ST7789_Font::CHAR_HEIGHT; row++) {
        uint8_t line = font_data[row];
        for (uint8_t col = 0; col < ST7789_Font::CHAR_WIDTH; col++) {
            if (line & (0x80 >> col)) {
                drawPixel(x + col, y + row, color);
            } else if (bg_color != ST7789_Colors::BLACK) {
                drawPixel(x + col, y + row, bg_color);
            }
        }
    }
}

void ST7789V3::drawString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg_color) {
    uint16_t current_x = x;
    const char* ptr = str;
    
    while (*ptr) {
        if (current_x + ST7789_Font::CHAR_WIDTH > ST7789_WIDTH) break;
        
        uint8_t bytes_consumed;
        const uint8_t* font_data = Font8x16_GetCharUTF8(ptr, &bytes_consumed);
        
        // Рисуем символ
        for (uint8_t row = 0; row < ST7789_Font::CHAR_HEIGHT; row++) {
            uint8_t line = font_data[row];
            for (uint8_t col = 0; col < ST7789_Font::CHAR_WIDTH; col++) {
                if (line & (0x80 >> col)) {
                    drawPixel(current_x + col, y + row, color);
                } else if (bg_color != ST7789_Colors::BLACK) {
                    drawPixel(current_x + col, y + row, bg_color);
                }
            }
        }
        
        current_x += ST7789_Font::CHAR_WIDTH;
        ptr += bytes_consumed;
    }
}

// Дополнительная функция для рисования строк с кириллицей
void ST7789V3::drawStringUTF8(uint16_t x, uint16_t y, const char* utf8_str, uint16_t color, uint16_t bg_color) {
    drawString(x, y, utf8_str, color, bg_color);
}

uint16_t ST7789V3::rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void ST7789V3::invertColors(bool invert) {
    if (invert) {
        writeCommand(ST7789_Commands::INVON);   // Включить инверсию
    } else {
        writeCommand(ST7789_Commands::INVOFF);  // Отключить инверсию
    }
}

void ST7789V3::invertOn() {
    writeCommand(ST7789_Commands::INVON);
}

void ST7789V3::invertOff() {
    writeCommand(ST7789_Commands::INVOFF);
}

void ST7789V3::setRotation(uint8_t rotation) {
    writeCommand(ST7789_Commands::MADCTL);
    
    switch (rotation) {
        case 0: // Портретная ориентация (0°)
            writeData8(0x00);
            break;
        case 1: // Альбомная ориентация (90°)
            writeData8(0x60);
            break;
        case 2: // Портретная ориентация, перевернутая (180°)
            writeData8(0xC0);
            break;
        case 3: // Альбомная ориентация, перевернутая (270°)
            writeData8(0xA0);
            break;
        default:
            writeData8(0x00); // По умолчанию 0°
            break;
    }
}

bool ST7789V3::setFramebuffer(Framebuffer* fb) {
    if (fb == nullptr) {
        framebuffer_ = nullptr;
        return true;
    }
    
    // Проверяем совместимость размеров
    if (fb->getWidth() != ST7789_WIDTH || fb->getHeight() != ST7789_HEIGHT) {
        return false; // Размеры не совпадают
    }
    
    framebuffer_ = fb;
    return true;
}

void ST7789V3::clearFramebuffer() {
    if (framebuffer_ != nullptr && framebuffer_->isAllocated()) {
        framebuffer_->clear(ST7789_Colors::BLACK);
    }
}

Framebuffer* ST7789V3::getFramebuffer() const {
    return framebuffer_;
}

void ST7789V3::flushFramebuffer() {
    if (framebuffer_ == nullptr || !framebuffer_->isAllocated()) {
        return; // Буфер не установлен или не инициализирован
    }
    
    // Устанавливаем окно на весь экран
    setWindow(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);
    
    // Начинаем запись в память дисплея
    writeCommand(ST7789_Commands::RAMWR);
    
    // Получаем указатель на буфер
    const uint16_t* buffer = framebuffer_->getBuffer();
    uint32_t buffer_size = framebuffer_->getBufferSize();
    
    // Статический буфер для конвертации (избегаем new)
    const uint32_t chunk_size = 512; // Уменьшенный размер чанка
    static uint8_t byte_buffer[chunk_size * 2]; // Статический буфер для байтов
    
    HAL_GPIO_WritePin(dc_pin_.port, dc_pin_.pin, GPIO_PIN_SET); // DC = 1 для данных
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_RESET); // CS = 0
    
    // Передаем данные по частям
    for (uint32_t i = 0; i < buffer_size; i += chunk_size) {
        uint32_t current_chunk = std::min(chunk_size, buffer_size - i);
        
        // Конвертируем чанк в байты с правильным порядком
        for (uint32_t j = 0; j < current_chunk; j++) {
            uint16_t pixel = buffer[i + j];
            byte_buffer[j * 2] = (pixel >> 8) & 0xFF;     // Старший байт
            byte_buffer[j * 2 + 1] = pixel & 0xFF;        // Младший байт
        }
        
        // Передаем чанк
        HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi_, byte_buffer, current_chunk * 2, ST7789_Config::SPI_TIMEOUT);
        if (status != HAL_OK) {
            // Если передача не удалась, прерываем
            break;
        }
    }
    
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_SET); // CS = 1
}

void ST7789V3::flushFramebufferRegion(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    if (framebuffer_ == nullptr || !framebuffer_->isAllocated()) {
        return; // Буфер не установлен или не инициализирован
    }
    
    // Проверяем границы
    if (x >= ST7789_WIDTH || y >= ST7789_HEIGHT || 
        x + w > ST7789_WIDTH || y + h > ST7789_HEIGHT) {
        return; // Выход за границы экрана
    }
    
    // Устанавливаем окно для региона
    setWindow(x, y, x + w - 1, y + h - 1);
    
    // Начинаем запись в память дисплея
    writeCommand(ST7789_Commands::RAMWR);
    
    HAL_GPIO_WritePin(dc_pin_.port, dc_pin_.pin, GPIO_PIN_SET); // DC = 1 для данных
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_RESET); // CS = 0
    
    // Передаем данные построчно из буфера
    const uint16_t* buffer = framebuffer_->getBuffer();
    for (uint16_t row = 0; row < h; row++) {
        // Вычисляем позицию начала строки в буфере
        uint32_t buffer_offset = (y + row) * ST7789_WIDTH + x;
        
        // Передаем строку
        HAL_SPI_Transmit(hspi_, (uint8_t*)(buffer + buffer_offset), w * 2, ST7789_Config::SPI_TIMEOUT);
    }
    
    HAL_GPIO_WritePin(cs_pin_.port, cs_pin_.pin, GPIO_PIN_SET); // CS = 1
}

bool ST7789V3::isFramebufferEnabled() const {
    return (framebuffer_ != nullptr && framebuffer_->isAllocated());
}

// Методы масштабированного текста
void ST7789V3::drawCharScaled(uint16_t x, uint16_t y, char ch, uint16_t color, uint8_t scale, uint16_t bg_color) {
    // Если буфер кадра доступен, используем его для лучшей производительности
    if (framebuffer_ != nullptr && framebuffer_->isAllocated()) {
        framebuffer_->drawCharScaled(x, y, ch, color, scale, bg_color);
        return;
    }
    
    // Fallback на прямое рисование
    if (scale == 0) scale = 1; // Минимальный масштаб 1
    if (scale > 8) scale = 8;  // Максимальный масштаб 8
    
    uint16_t char_width = ST7789_Font::CHAR_WIDTH * scale;
    uint16_t char_height = ST7789_Font::CHAR_HEIGHT * scale;
    
    if (x + char_width > ST7789_WIDTH || y + char_height > ST7789_HEIGHT) return;
    
    const uint8_t* font_data = Font8x16_GetChar(static_cast<uint8_t>(ch));
    
    for (uint8_t row = 0; row < ST7789_Font::CHAR_HEIGHT; row++) {
        uint8_t line = font_data[row];
        for (uint8_t col = 0; col < ST7789_Font::CHAR_WIDTH; col++) {
            uint16_t pixel_color = (line & (0x80 >> col)) ? color : bg_color;
            
            // Рисуем увеличенный пиксель только если нужно
            if (pixel_color != ST7789_Colors::BLACK || bg_color != ST7789_Colors::BLACK) {
                for (uint8_t sy = 0; sy < scale; sy++) {
                    for (uint8_t sx = 0; sx < scale; sx++) {
                        drawPixel(x + col * scale + sx, y + row * scale + sy, pixel_color);
                    }
                }
            }
        }
    }
}

void ST7789V3::drawStringScaled(uint16_t x, uint16_t y, const char* str, uint16_t color, uint8_t scale, uint16_t bg_color) {
    // Если буфер кадра доступен, используем его для лучшей производительности
    if (framebuffer_ != nullptr && framebuffer_->isAllocated()) {
        framebuffer_->drawStringScaled(x, y, str, color, scale, bg_color);
        return;
    }
    
    // Fallback на прямое рисование
    if (scale == 0) scale = 1;
    if (scale > 8) scale = 8;
    
    uint16_t current_x = x;
    uint16_t char_width = ST7789_Font::CHAR_WIDTH * scale;
    
    const char* ptr = str;
    
    while (*ptr) {
        if (current_x + char_width > ST7789_WIDTH) break;
        
        uint8_t bytes_consumed;
        const uint8_t* font_data = Font8x16_GetCharUTF8(ptr, &bytes_consumed);
        
        // Рисуем увеличенный символ
        for (uint8_t row = 0; row < ST7789_Font::CHAR_HEIGHT; row++) {
            uint8_t line = font_data[row];
            for (uint8_t col = 0; col < ST7789_Font::CHAR_WIDTH; col++) {
                uint16_t pixel_color = (line & (0x80 >> col)) ? color : bg_color;
                
                if (pixel_color != ST7789_Colors::BLACK || bg_color != ST7789_Colors::BLACK) {
                    for (uint8_t sy = 0; sy < scale; sy++) {
                        for (uint8_t sx = 0; sx < scale; sx++) {
                            uint16_t px = current_x + col * scale + sx;
                            uint16_t py = y + row * scale + sy;
                            if (px < ST7789_WIDTH && py < ST7789_HEIGHT) {
                                drawPixel(px, py, pixel_color);
                            }
                        }
                    }
                }
            }
        }
        
        current_x += char_width;
        ptr += bytes_consumed;
    }
}

void ST7789V3::drawStringUTF8Scaled(uint16_t x, uint16_t y, const char* utf8_str, uint16_t color, uint8_t scale, uint16_t bg_color) {
    // Если буфер кадра доступен, используем его
    if (framebuffer_ != nullptr && framebuffer_->isAllocated()) {
        framebuffer_->drawStringUTF8Scaled(x, y, utf8_str, color, scale, bg_color);
        return;
    }
    
    // Fallback на drawStringScaled
    drawStringScaled(x, y, utf8_str, color, scale, bg_color);
}