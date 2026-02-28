// main.c
#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>
#include <windows.h>  // Для SetConsoleOutputCP
#include "my_string.h"
#include "fieldinfo.h"

// Функция для настройки консоли Windows
void setup_console(void) {
    // Устанавливаем локаль для широких символов
    setlocale(LC_ALL, ".UTF-8");  // или ".1251" для кириллицы
    
    // Для Windows нужно также установить кодировку консоли
    SetConsoleOutputCP(CP_UTF8);      // Вывод в UTF-8
    SetConsoleCP(CP_UTF8);             // Ввод в UTF-8
    
    // Альтернатива: использовать OEM кодировку (русскую)
    // SetConsoleOutputCP(866);  // для кириллицы в старых консолях
}

int main(void) {
    setup_console();
    
    printf("=== String из char ===\n");
    char text[] = "Hello";
    String* char_str = String_Create(text, 5, GetCharFieldInfo());
    String_PrintLine(char_str);
    String_Destroy(char_str);
    
    printf("\n=== String из wchar_t ===\n");
    wchar_t wtext[] = L"Привет";
    String* wchar_str = String_Create(wtext, 6, GetWCharFieldInfo());
    String_PrintLine(wchar_str);
    
    printf("\n=== Конкатенация wchar_t ===\n");
    wchar_t wtext2[] = L" Мир";
    String* wchar_str2 = String_Create(wtext2, 4, GetWCharFieldInfo());
    String* concat = String_Concat(wchar_str, wchar_str2);
    String_PrintLine(concat);
    
    printf("\n=== Подстрока wchar_t ===\n");
    String* sub = String_Substring(wchar_str, 0, 5);
    String_PrintLine(sub);
    
    printf("\n=== Поиск в wchar_t ===\n");
    wchar_t wpattern[] = L"ив";
    String* pattern = String_Create(wpattern, 2, GetWCharFieldInfo());
    size_t count = 0;
    int* indices = String_Find(wchar_str, pattern, true, &count);
    if (indices != NULL) {
        printf("Найдено: %zu совпадений\n", count);
        free(indices);
    }
    
    printf("\n=== Проверка типов ===\n");
    printf("char и wchar_t одинаковый тип? %s\n", 
           String_SameType(char_str, wchar_str) ? "ДА" : "НЕТ");
    
    String_Destroy(wchar_str);
    String_Destroy(wchar_str2);
    String_Destroy(concat);
    String_Destroy(sub);
    String_Destroy(pattern);
    
    printf("\n✅ ПОЛИМОРФИЗМ РАБОТАЕТ!\n");
    return 0;
}