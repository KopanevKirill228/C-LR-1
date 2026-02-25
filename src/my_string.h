#ifndef my_string_h
#define my_string_h

#include <stddef.h>
#include <stdbool.h>

typedef struct _string String;


String* String_Create(const char* c_str);
String* String_CreateEmpty(size_t capacity);
void String_Destroy(String* s);


String* String_Concat(const String* s1, const String* s2);
String* String_Substring(const String* s, size_t start, size_t end);
int* String_Find(const String* s, const String* pattern, bool case_sensitive, size_t* out_count);


size_t String_Length(const String* s);
const char* String_ToCString(String* s);

#endif