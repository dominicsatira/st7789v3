cmake_minimum_required(VERSION 3.16)

# Информация о проекте
project(ST7789V3_Library
    VERSION 1.0.0
    DESCRIPTION "ST7789V3 TFT LCD Display Library for STM32"
    LANGUAGES C CXX ASM
)

# Настройки C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Настройки C
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Опции компиляции
option(ST7789V3_BUILD_EXAMPLES "Build examples" OFF)
option(ST7789V3_BUILD_TESTS "Build tests" OFF)

# Создание библиотеки
add_library(st7789v3 STATIC
    # Основные файлы библиотеки
    src/st7789v3.cpp
    framebuffer/framebuffer.cpp
    fonts/font8x16.cpp
)

# Псевдоним для библиотеки
add_library(ST7789V3::st7789v3 ALIAS st7789v3)

# Заголовочные файлы
target_include_directories(st7789v3 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/framebuffer>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/fonts>
    $<INSTALL_INTERFACE:include>
)

# Компиляторные флаги для оптимизации
target_compile_options(st7789v3 PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -Wpedantic>
    $<$<COMPILE_LANGUAGE:C>:-Wall -Wextra -Wpedantic>
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:Debug>:-O0 -g>
)

# Определения препроцессора
target_compile_definitions(st7789v3 PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# Поддержка STM32 HAL (требуется для интеграции с проектом)
# Пользователь должен определить путь к HAL библиотекам
if(DEFINED STM32_HAL_PATH)
    target_include_directories(st7789v3 PUBLIC ${STM32_HAL_PATH}/Inc)
    target_include_directories(st7789v3 PUBLIC ${STM32_HAL_PATH}/Inc/Legacy)
endif()

# Установка библиотеки
include(GNUInstallDirs)

# Установка файлов
install(TARGETS st7789v3
    EXPORT ST7789V3Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Установка заголовочных файлов
install(DIRECTORY inc/ 
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/st7789v3
    FILES_MATCHING PATTERN "*.hpp"
)

install(DIRECTORY framebuffer/ 
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/st7789v3
    FILES_MATCHING PATTERN "*.hpp"
)

install(DIRECTORY fonts/ 
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/st7789v3
    FILES_MATCHING PATTERN "*.hpp"
)

# Экспорт целей
install(EXPORT ST7789V3Targets
    FILE ST7789V3Targets.cmake
    NAMESPACE ST7789V3::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ST7789V3
)

# Создание файла конфигурации
include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ST7789V3Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ST7789V3Config.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ST7789V3
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ST7789V3ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Установка файлов конфигурации
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ST7789V3Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ST7789V3ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ST7789V3
)

# Создание файла pkg-config
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/st7789v3.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/st7789v3.pc"
    @ONLY
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/st7789v3.pc"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# Примеры (опционально)
if(ST7789V3_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Тесты (опционально)
if(ST7789V3_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Отображение информации о конфигурации
message(STATUS "ST7789V3 Library Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Build examples: ${ST7789V3_BUILD_EXAMPLES}")
message(STATUS "  Build tests: ${ST7789V3_BUILD_TESTS}")
if(DEFINED STM32_HAL_PATH)
    message(STATUS "  STM32 HAL path: ${STM32_HAL_PATH}")
endif()
