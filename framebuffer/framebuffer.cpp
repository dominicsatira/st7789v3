#include "framebuffer.hpp"
#include "../fonts/font8x16.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>

Framebuffer::Framebuffer(uint16_t width, uint16_t height)
    : buffer_(nullptr), width_(width), height_(height), allocated_(false) {
}

Framebuffer::~Framebuffer() {
    release();
}

bool Framebuffer::init() {
    if (allocated_) {
        return true; // Уже инициализирован
    }
    
    uint32_t buffer_size = static_cast<uint32_t>(width_) * height_ * sizeof(uint16_t);
    buffer_ = static_cast<uint16_t*>(malloc(buffer_size));
    
    if (buffer_ == nullptr) {
        return false; // Не удалось выделить память
    }
    
    allocated_ = true;
    clear(); // Очищаем буфер
    return true;
}

void Framebuffer::clear(uint16_t color) {
    if (!allocated_ || buffer_ == nullptr) {
        return;
    }
    
    uint32_t total_pixels = static_cast<uint32_t>(width_) * height_;
    for (uint32_t i = 0; i < total_pixels; i++) {
        buffer_[i] = color;
    }
}

void Framebuffer::release() {
    if (allocated_ && buffer_ != nullptr) {
        free(buffer_);
        buffer_ = nullptr;
        allocated_ = false;
    }
}

void Framebuffer::setPixel(uint16_t x, uint16_t y, uint16_t color) {
    if (!allocated_ || buffer_ == nullptr || x >= width_ || y >= height_) {
        return;
    }
    
    uint32_t index = static_cast<uint32_t>(y) * width_ + x;
    buffer_[index] = color;
}

uint16_t Framebuffer::getPixel(uint16_t x, uint16_t y) const {
    if (!allocated_ || buffer_ == nullptr || x >= width_ || y >= height_) {
        return 0x0000; // Черный по умолчанию
    }
    
    uint32_t index = static_cast<uint32_t>(y) * width_ + x;
    return buffer_[index];
}

void Framebuffer::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    int16_t dx = abs(static_cast<int16_t>(x1) - static_cast<int16_t>(x0));
    int16_t dy = abs(static_cast<int16_t>(y1) - static_cast<int16_t>(y0));
    int16_t sx = (x0 < x1) ? 1 : -1;
    int16_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx - dy;
    
    int16_t x = x0, y = y0;
    
    while (true) {
        setPixel(x, y, color);
        
        if (x == x1 && y == y1) break;
        
        int16_t e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void Framebuffer::drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // Верхняя и нижняя горизонтальные линии
    for (uint16_t i = 0; i < w; i++) {
        setPixel(x + i, y, color);
        setPixel(x + i, y + h - 1, color);
    }
    
    // Левая и правая вертикальные линии
    for (uint16_t i = 0; i < h; i++) {
        setPixel(x, y + i, color);
        setPixel(x + w - 1, y + i, color);
    }
}

void Framebuffer::fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (x + w > width_ || y + h > height_) {
        return; // Выход за границы
    }
    
    for (uint16_t row = 0; row < h; row++) {
        for (uint16_t col = 0; col < w; col++) {
            setPixel(x + col, y + row, color);
        }
    }
}

void Framebuffer::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    int16_t x = r;
    int16_t y = 0;
    int16_t err = 0;
    
    while (x >= y) {
        setPixel(x0 + x, y0 + y, color);
        setPixel(x0 + y, y0 + x, color);
        setPixel(x0 - y, y0 + x, color);
        setPixel(x0 - x, y0 + y, color);
        setPixel(x0 - x, y0 - y, color);
        setPixel(x0 - y, y0 - x, color);
        setPixel(x0 + y, y0 - x, color);
        setPixel(x0 + x, y0 - y, color);
        
        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

void Framebuffer::fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {
    for (int16_t y = -r; y <= r; y++) {
        for (int16_t x = -r; x <= r; x++) {
            if (x*x + y*y <= r*r) {
                setPixel(x0 + x, y0 + y, color);
            }
        }
    }
}

void Framebuffer::drawChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg_color) {
    if (x + 8 > width_ || y + 16 > height_) return;
    
    const uint8_t* font_data = Font8x16_GetChar(static_cast<uint8_t>(ch));
    
    for (uint8_t row = 0; row < 16; row++) {
        uint8_t line = font_data[row];
        for (uint8_t col = 0; col < 8; col++) {
            if (line & (0x80 >> col)) {
                setPixel(x + col, y + row, color);
            } else if (bg_color != 0x0000) {
                setPixel(x + col, y + row, bg_color);
            }
        }
    }
}

void Framebuffer::drawString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg_color) {
    uint16_t current_x = x;
    const char* ptr = str;
    
    while (*ptr) {
        if (current_x + 8 > width_) break;
        
        drawChar(current_x, y, *ptr, color, bg_color);
        current_x += 8;
        ptr++;
    }
}

void Framebuffer::drawStringUTF8(uint16_t x, uint16_t y, const char* utf8_str, uint16_t color, uint16_t bg_color) {
    uint16_t current_x = x;
    const char* ptr = utf8_str;
    
    while (*ptr) {
        if (current_x + 8 > width_) break;
        
        uint8_t bytes_consumed;
        const uint8_t* font_data = Font8x16_GetCharUTF8(ptr, &bytes_consumed);
        
        // Рисуем символ
        for (uint8_t row = 0; row < 16; row++) {
            uint8_t line = font_data[row];
            for (uint8_t col = 0; col < 8; col++) {
                if (line & (0x80 >> col)) {
                    setPixel(current_x + col, y + row, color);
                } else if (bg_color != 0x0000) {
                    setPixel(current_x + col, y + row, bg_color);
                }
            }
        }
        
        current_x += 8;
        ptr += bytes_consumed;
    }
}

uint16_t Framebuffer::rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

// Реализация масштабированного текста
void Framebuffer::drawCharScaled(uint16_t x, uint16_t y, char ch, uint16_t color, uint8_t scale, uint16_t bg_color) {
    if (scale == 0) scale = 1; // Минимальный масштаб 1
    if (scale > 8) scale = 8;  // Максимальный масштаб 8
    
    uint16_t char_width = 8 * scale;
    uint16_t char_height = 16 * scale;
    
    if (x + char_width > width_ || y + char_height > height_) return;
    
    const uint8_t* font_data = Font8x16_GetChar(static_cast<uint8_t>(ch));
    
    for (uint8_t row = 0; row < 16; row++) {
        uint8_t line = font_data[row];
        for (uint8_t col = 0; col < 8; col++) {
            uint16_t pixel_color = (line & (0x80 >> col)) ? color : bg_color;
            
            // Рисуем увеличенный пиксель только если нужно
            if (pixel_color != 0x0000 || bg_color != 0x0000) {
                for (uint8_t sy = 0; sy < scale; sy++) {
                    for (uint8_t sx = 0; sx < scale; sx++) {
                        uint16_t px = x + col * scale + sx;
                        uint16_t py = y + row * scale + sy;
                        if (px < width_ && py < height_) {
                            setPixel(px, py, pixel_color);
                        }
                    }
                }
            }
        }
    }
}

void Framebuffer::drawStringScaled(uint16_t x, uint16_t y, const char* str, uint16_t color, uint8_t scale, uint16_t bg_color) {
    if (scale == 0) scale = 1;
    if (scale > 8) scale = 8;
    
    uint16_t current_x = x;
    uint16_t char_width = 8 * scale;
    
    const char* ptr = str;
    
    while (*ptr) {
        if (current_x + char_width > width_) break;
        
        uint8_t bytes_consumed;
        const uint8_t* font_data = Font8x16_GetCharUTF8(ptr, &bytes_consumed);
        
        // Рисуем увеличенный символ
        for (uint8_t row = 0; row < 16; row++) {
            uint8_t line = font_data[row];
            for (uint8_t col = 0; col < 8; col++) {
                uint16_t pixel_color = (line & (0x80 >> col)) ? color : bg_color;
                
                if (pixel_color != 0x0000 || bg_color != 0x0000) {
                    for (uint8_t sy = 0; sy < scale; sy++) {
                        for (uint8_t sx = 0; sx < scale; sx++) {
                            uint16_t px = current_x + col * scale + sx;
                            uint16_t py = y + row * scale + sy;
                            if (px < width_ && py < height_) {
                                setPixel(px, py, pixel_color);
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

void Framebuffer::drawStringUTF8Scaled(uint16_t x, uint16_t y, const char* utf8_str, uint16_t color, uint8_t scale, uint16_t bg_color) {
    drawStringScaled(x, y, utf8_str, color, scale, bg_color);
}
