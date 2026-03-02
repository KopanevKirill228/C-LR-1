#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <windows.h>
#include "my_string.h"
#include "fieldinfo.h"
#include "tokenizer.h"

void clear_input(void);

int main(void) {
    // Настройка
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    
    int choice;
    String* current = NULL;
    
    do {
        printf("\n====================================\n");
        printf("  ЛАБОРАТОРНАЯ РАБОТА №1 ВАРИАНТ 7\n");
        printf("====================================\n");
        printf("1. Создать строку (char)\n");
        printf("2. Создать строку (int)\n");
        printf("3. Конкатенация\n");
        printf("4. Подстрока\n");
        printf("5. Поиск подстроки\n");
        printf("6. Показать текущую строку\n");
        printf("7. Разбиение на лексемы\n");
        printf("0. Выход\n");
        printf("====================================\n");
        printf("Выбор: ");
        
        scanf("%d", &choice);
        clear_input();
        
        switch(choice) {
            case 1: { // Создать char строку
                printf("\n--- СОЗДАНИЕ char СТРОКИ ---\n");
                char buffer[256];
                printf("Введите строку: ");
                fgets(buffer, sizeof(buffer), stdin);
                
                size_t len = strlen(buffer);
                if (len > 0 && buffer[len-1] == '\n') {
                    buffer[len-1] = '\0';
                    len--;
                }
                
                if (current) String_Destroy(current);
                current = String_Create(buffer, len, GetCharFieldInfo());
                
                printf("Строка создана: ");
                String_PrintLine(current);  // Универсальная печать
                break;
            }
            
            case 2: { // Создать int строку
                printf("\n--- СОЗДАНИЕ int СТРОКИ ---\n");
                int count;
                printf("Сколько чисел? ");
                scanf("%d", &count);
                
                int* numbers = (int*)malloc(count * sizeof(int));
                printf("Введите %d чисел: ", count);
                for (int i = 0; i < count; i++) {
                    scanf("%d", &numbers[i]);
                }
                clear_input();
                
                if (current) String_Destroy(current);
                current = String_Create(numbers, count, GetIntFieldInfo());
                
                printf("Строка создана: ");
                String_PrintLine(current);  // Универсальная печать через полиморфизм
                free(numbers);
                break;
            }
            
            case 3: { // Конкатенация
                printf("\n--- КОНКАТЕНАЦИЯ ---\n");
                if (!current) {
                    printf("Сначала создайте строку\n");
                    break;
                }
                
                const FieldInfo* type = String_GetType(current);
                printf("Текущая строка: ");
                if (type == GetCharFieldInfo()) {
                    for (size_t i = 0; i < String_Length(current); i++) {
                        const char* c = (const char*)String_GetElement(current, i);
                        printf("%c", *c);
                    }
                } else {
                    for (size_t i = 0; i < String_Length(current); i++) {
                        const int* n = (const int*)String_GetElement(current, i);
                        printf("%d ", *n);
                    }
                }
                printf("\n");
                
                if (type == GetCharFieldInfo()) {
                    char buffer[256];
                    printf("Введите строку для добавления: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    
                    size_t len = strlen(buffer);
                    if (len > 0 && buffer[len-1] == '\n') {
                        buffer[len-1] = '\0';
                        len--;
                    }
                    
                    String* second = String_Create(buffer, len, type);
                    
                    if (!String_SameType(current, second)) {
                        printf("ОШИБКА: Нельзя конкатенировать строки разных типов!\n");
                        String_Destroy(second);
                        break;
                    }

                    String* result = String_Concat(current, second);
                    
                    if (result) {
                        String_Destroy(current);
                        current = result;
                        printf("Результат добавлен\n");
                    }
                    String_Destroy(second);
                    
                } else {
                    int count;
                    printf("Сколько чисел добавить? ");
                    scanf("%d", &count);
                    
                    int* numbers = (int*)malloc(count * sizeof(int));
                    printf("Введите %d чисел: ", count);
                    for (int i = 0; i < count; i++) {
                        scanf("%d", &numbers[i]);
                    }
                    clear_input();
                    
                    String* second = String_Create(numbers, count, type);
                    String* result = String_Concat(current, second);
                    
                    if (result) {
                        String_Destroy(current);
                        current = result;
                        printf("Результат добавлен\n");
                    }
                    
                    String_Destroy(second);
                    free(numbers);
                }
                break;
            }
            
            case 4: { // Подстрока
                printf("\n--- ПОДСТРОКА ---\n");
                if (!current) {
                    printf("Сначала создайте строку\n");
                    break;
                }
                
                int start, end;
                printf("Введите start и end: ");
                scanf("%d %d", &start, &end);
                clear_input();
                
                String* sub = String_Substring(current, start, end);
                
                if (sub) {
                    printf("Подстрока: ");
                    const FieldInfo* type = String_GetType(sub);
                    if (type == GetCharFieldInfo()) {
                        for (size_t i = 0; i < String_Length(sub); i++) {
                            const char* c = (const char*)String_GetElement(sub, i);
                            printf("%c", *c);
                        }
                    } else {
                        for (size_t i = 0; i < String_Length(sub); i++) {
                            const int* n = (const int*)String_GetElement(sub, i);
                            printf("%d ", *n);
                        }
                    }
                    printf("\n");
                    String_Destroy(sub);
                } else {
                    printf("Ошибка: неверный диапазон\n");
                }
                break;
            }
            
            case 5: { // Поиск
                printf("\n--- ПОИСК ПОДСТРОКИ ---\n");
                if (!current) {
                    printf("Сначала создайте строку\n");
                    break;
                }
                
                const FieldInfo* type = String_GetType(current);
                
                if (type == GetCharFieldInfo()) {
                    char buffer[256];
                    printf("Введите что искать: ");
                    fgets(buffer, sizeof(buffer), stdin);
                    
                    size_t len = strlen(buffer);
                    if (len > 0 && buffer[len - 1] == '\n') {
                        buffer[len - 1] = '\0';
                        len--;
                    }
                    
                    String* pattern = String_Create(buffer, len, type);
                    
                    int case_sensitive;
                    printf("Учитывать регистр? (1-да, 0-нет): ");
                    scanf("%d", &case_sensitive);
                    clear_input();
                    
                    size_t count;
                    int* indices = String_Find(current, pattern, case_sensitive, &count);
                    
                    if (indices) {
                        printf("Найдено %zu совпадений: ", count);
                        for (size_t i = 0; i < count; i++) {
                            printf("%d ", indices[i]);
                        }
                        printf("\n");
                        free(indices);
                    } else {
                        printf("Совпадений не найдено\n");
                    }
                    
                    String_Destroy(pattern);
                    
                } else {
                    int count;
                    printf("Сколько чисел искать? ");
                    scanf("%d", &count);
                    
                    int* numbers = (int*)malloc(count * sizeof(int));
                    printf("Введите %d чисел: ", count);
                    for (int i = 0; i < count; i++) {
                        scanf("%d", &numbers[i]);
                    }
                    clear_input();
                    
                    String* pattern = String_Create(numbers, count, type);
                    
                    size_t result_count;
                    int* indices = String_Find(current, pattern, 1, &result_count);
                    
                    if (indices) {
                        printf("Найдено %zu совпадений: ", result_count);
                        for (size_t i = 0; i < result_count; i++) {
                            printf("%d ", indices[i]);
                        }
                        printf("\n");
                        free(indices);
                    } else {
                        printf("Совпадений не найдено\n");
                    }
                    
                    String_Destroy(pattern);
                    free(numbers);
                }
                break;
            }
            
            case 6: // Показать текущую строку
                printf("\n--- ТЕКУЩАЯ СТРОКА ---\n");
                if (!current) {
                    printf("Строка не создана\n");
                } else {
                    const FieldInfo* type = String_GetType(current);
                    printf("Тип: %s\n", type == GetCharFieldInfo() ? "char" : "int");
                    printf("Содержимое: ");
                    if (type == GetCharFieldInfo()) {
                        for (size_t i = 0; i < String_Length(current); i++) {
                            const char* c = (const char*)String_GetElement(current, i);
                            printf("%c", *c);
                        }
                    } else {
                        for (size_t i = 0; i < String_Length(current); i++) {
                            const int* n = (const int*)String_GetElement(current, i);
                            printf("%d ", *n);
                        }
                    }
                    printf("\n");
                }
                break;

            case 7: {
                printf("\n--- РАЗБИЕНИЕ НА ЛЕКСЕМЫ ---\n");
                if (!current) {
                    printf("Сначала создайте строку\n");
                    break;
                }
                
                if (!Tokenize_Supported(current)) {
                    printf("Токенизация доступна только для char строк\n");
                    break;
                }
                
                printf("Исходная строка: ");
                String_PrintLine(current);
                
                DynamicArray* tokens = Tokenize(current);
                if (tokens) {
                    printf("Лексемы: ");
                    Tokenize_Print(tokens, stdout);
                    printf("\n");
                    Tokenize_Destroy(tokens);
                } else {
                    printf("Не удалось разбить на лексемы\n");
                }
                break;
            }
            
            
            case 0:
                printf("До свидания!\n");
                break;
                
            default:
                printf("Неверный выбор\n");
        }
        
    } while (choice != 0);
    
    if (current) String_Destroy(current);
    return 0;
}

void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}