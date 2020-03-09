#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

typedef enum Type Type;
typedef struct String String;
typedef struct Number Number;
typedef union Data Data;
typedef struct Value Value;
typedef struct Arg Arg;
typedef struct Format Format;

enum Type {
    TYPE_NUMBER,
    TYPE_STRING
};

struct String {
    size_t size;
    int heap;
    char * data;
};

struct Number {
    String integer;
    String fraction;
    String exponent;
};

union Data {
    String string;
    Number number;
};

struct Value {
    Type type;
    Data data;
};

struct Arg {
    Value data;
    Arg * next;
};

struct Format {
    char * format;
    String (*func) ();
};

static size_t size(char *);
static inline String string(size_t, char *);
static inline void print_string(String);
static String concatenate_strings(String, String); // uses malloc, don't forget to free()
static String concatenate_layout();                // uses malloc, don't forget to free()
static String format_expand(Format);               // uses malloc, don't forget to free()
static inline Number number(String, String, String);
static inline Data string_data(String);
static inline Data number_data(Number);
static inline Value value(Type, Data);

#define STRING(str) \
    string(size(str), str)

#define NUMBER(integer, fraction, exponent) \
    number(STRING(integer),  \
           STRING(fraction), \
           STRING(exponent)  )

#define DATA_TYPE_STRING(data) \
    string_data(STRING(data))

#define DATA_TYPE_NUMBER(data...) \
    number_data(NUMBER(data))

#define VALUE(type, data...) \
    value(type, DATA_##type(data))

#include "config.h"

size_t
size(char * str)
{
    if (!str) return 0;
    for (size_t size = 0;;size++) {
        // stops at first '\0'
        if (!*(str + size)) return size;
    }
}

String
string(size_t size, char * data)
{
    String str = {
        .size = size,
        .data = data,
        .heap = 0
    };

    return str;
}

void
print_string(String str)
{
    printf("size: %ld\ndata: \"%s\"\n", str.size, str.data);
}

Number
number(String integer,
       String fraction,
       String exponent)
{
    Number num = {
        .integer = integer,
        .fraction = fraction,
        .exponent = exponent
    };

    return num;
}

Data
string_data(String data)
{
    Data d = {
        .string = data
    };

    return d;
}

Data
number_data(Number data)
{
    Data d = {
        .number = data
    };

    return d;
}

Value
value(Type type, Data data)
{
    Value val = {
        .type = type,
        .data = data
    };

    return val;
}

String
concatenate_strings(String first, String second)
{
    char * buffer = malloc(first.size + second.size + 1);

    for (long unsigned int i = 0; i < first.size; i++) {
        buffer[i] = first.data[i];
    }

    for (long unsigned int i = first.size; i < first.size + second.size; i++) {
        buffer[i] = second.data[i - first.size];
    }

    buffer[first.size + second.size] = '\0';

    if (first.heap) free(first.data);
    if (second.heap) free(second.data);

    String str = STRING(buffer);
    str.heap = 1;

    return str;
}

String
format_expand(Format form)
{
    String func_result = form.func();

    char * buffer = malloc(size(form.format) + func_result.size);

    sprintf(buffer, form.format, func_result.data);

    if (func_result.heap) free(func_result.data);

    String str = STRING(buffer);
    str.heap = 1;

    return str;
}

String
concatenate_layout()
{
    String result = STRING("");

    for (long unsigned int i = 0; i < ARRAY_SIZE(layout); i++) {
        String tmp = concatenate_strings(
            result,
            format_expand(
                layout[i]
            )
        );

        result = tmp;
    }

    print_string(result);
    return result;
}

int
main()
{
    while (1) {
        String str = concatenate_layout();
        print_string(str);
        if (str.heap) free(str.data);
    }
    return 0;
}
