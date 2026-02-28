// main.c - английская версия
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include "my_string.h"
#include "fieldinfo.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main(void) {
    setlocale(LC_ALL, "");
    
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    
    printf("=== POLYMORPHIC STRING (Variant 7) ===\n\n");
    
    // 1. char string
    printf("1. char string: ");
    char text[] = "Hello";
    String* char_str = String_Create(text, 5, GetCharFieldInfo());
    String_PrintLine(char_str);
    
    // 2. int string
    printf("\n2. int string: ");
    int numbers[] = {1, 2, 3, 4, 5};
    String* int_str = String_Create(numbers, 5, GetIntFieldInfo());
    String_PrintLine(int_str);
    
    // 3. Concatenation
    printf("\n3. int concatenation: ");
    int more_numbers[] = {6, 7, 8, 9, 10};
    String* int_str2 = String_Create(more_numbers, 5, GetIntFieldInfo());
    String* int_concat = String_Concat(int_str, int_str2);
    String_PrintLine(int_concat);
    
    // 4. Substring
    printf("\n4. int substring [2-5): ");
    String* int_sub = String_Substring(int_str, 2, 5);
    String_PrintLine(int_sub);
    
    // 5. Search
    printf("\n5. Search [3,4] in int string: ");
    int pattern_data[] = {3, 4};
    String* pattern = String_Create(pattern_data, 2, GetIntFieldInfo());
    size_t count;
    int* indices = String_Find(int_str, pattern, true, &count);
    if (indices != NULL) {
        printf("found %zu matches at positions: ", count);
        for (size_t i = 0; i < count; i++) {
            printf("%d ", indices[i]);
        }
        printf("\n");
        free(indices);
    }
    
    // 6. Type check
    printf("\n6. Type check:\n");
    printf("  char and int are %s\n", 
           String_SameType(char_str, int_str) ? "SAME" : "DIFFERENT");
    
    // Cleanup
    String_Destroy(char_str);
    String_Destroy(int_str);
    String_Destroy(int_str2);
    String_Destroy(int_concat);
    String_Destroy(int_sub);
    String_Destroy(pattern);
    
    printf("\n✅ POLYMORPHISM DEMONSTRATED SUCCESSFULLY!\n");
    return 0;
}