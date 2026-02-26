// src/main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "my_string.h"
#include "fieldinfo.h"

int main(void) {
    // Настройка кодировки для русского текста
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    int choice;
    
    do {
        // Показываем меню
        printf("\n");
        printf("====================================\n");
        printf("  Лабораторная работа N1, Вариант 7\n");
        printf("  Полиморфная коллекция: СТРОКА\n");
        printf("====================================\n");
        printf("  1. Создать строку\n");
        printf("  2. Конкатенация строк\n");
        printf("  3. Получить подстроку\n");
        printf("  4. Поиск подстроки\n");
        printf("  5. Запустить автотесты\n");
        printf("  0. Выход\n");
        printf("====================================\n");
        printf("  Ваш выбор: ");
        
        // Ввод выбора
        if (scanf("%d", &choice) != 1) {
            choice = -1;
        }
        while (getchar() != '\n');  // Очистка буфера
        
        // ==================== 1. Создание строки ====================
        if (choice == 1) {
            char input[256];
            printf("\nВведите строку: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;  // Удалить Enter
            
            String* s = String_Create(input);
            if (s != NULL) {
                printf("Результат: \"%s\"\n", String_ToCString(s));
                printf("Длина: %zu\n", String_Length(s));
                String_Destroy(s);
            } else {
                printf("Ошибка создания строки\n");
            }
        }
        
        // ==================== 2. Конкатенация ====================
        else if (choice == 2) {
            char s1[256], s2[256];
            printf("\nВведите первую строку: ");
            fgets(s1, sizeof(s1), stdin);
            s1[strcspn(s1, "\n")] = 0;
            
            printf("Введите вторую строку: ");
            fgets(s2, sizeof(s2), stdin);
            s2[strcspn(s2, "\n")] = 0;
            
            String* str1 = String_Create(s1);
            String* str2 = String_Create(s2);
            String* result = String_Concat(str1, str2);
            
            if (result != NULL) {
                printf("Результат: \"%s\"\n", String_ToCString(result));
                String_Destroy(result);
            } else {
                printf("Ошибка конкатенации\n");
            }
            
            String_Destroy(str1);
            String_Destroy(str2);
        }
        
        // ==================== 3. Подстрока ====================
        else if (choice == 3) {
            char input[256];
            size_t start, end;
            
            printf("\nВведите строку: ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = 0;
            
            printf("Введите начальный индекс: ");
            scanf("%zu", &start);
            
            printf("Введите конечный индекс: ");
            scanf("%zu", &end);
            while (getchar() != '\n');
            
            String* s = String_Create(input);
            String* sub = String_Substring(s, start, end);
            
            if (sub != NULL) {
                printf("Результат: \"%s\"\n", String_ToCString(sub));
                String_Destroy(sub);
            } else {
                printf("Ошибка: некорректный диапазон\n");
            }
            
            String_Destroy(s);
        }
        
        // ==================== 4. Поиск ====================
        else if (choice == 4) {
            char text[256], pattern[256];
            int case_sensitive;
            
            printf("\nВведите строку: ");
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = 0;
            
            printf("Введите искомую подстроку: ");
            fgets(pattern, sizeof(pattern), stdin);
            pattern[strcspn(pattern, "\n")] = 0;
            
            printf("Учитывать регистр? (1 - да, 0 - нет): ");
            scanf("%d", &case_sensitive);
            while (getchar() != '\n');
            
            String* s_text = String_Create(text);
            String* s_pattern = String_Create(pattern);
            
            size_t count = 0;
            int* indices = String_Find(s_text, s_pattern, case_sensitive, &count);
            
            if (indices != NULL) {
                printf("Найдено совпадений: %zu\n", count);
                printf("Позиции: ");
                for (size_t i = 0; i < count; i++) {
                    printf("%d ", indices[i]);
                }
                printf("\n");
                free(indices);
            } else {
                printf("Совпадений не найдено\n");
            }
            
            String_Destroy(s_text);
            String_Destroy(s_pattern);
        }
        
        // ==================== 5. Автотесты ====================
        else if (choice == 5) {
            printf("\nЗапуск тестов...\n\n");
            system("build\\tests\\run_tests.exe");
        }
        
        // ==================== 0. Выход ====================
        else if (choice == 0) {
            printf("\nДо свидания!\n");
        }
        
        // ==================== Неверный выбор ====================
        else {
            printf("\nНеверный выбор. Попробуйте снова.\n");
        }
        
    } while (choice != 0);
    
    return 0;
}