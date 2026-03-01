#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include "test_macros.h"

extern void run_core_tests(void);
extern void run_string_tests(void);

int main(void) {
    // Настройка консоли для UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    printf("╔════════════════════════════════════════╗\n");
    printf("║   AUTOMATED TEST SUITE — VARIANT 7     ║\n");
    printf("║   DynamicArray + полиморфный String    ║\n");
    printf("╚════════════════════════════════════════╝\n");
    
    // Запуск тестов
    run_core_tests();
    run_string_tests();
    
    // Статистика
    PRINT_STATS();
    
    return 0;
}