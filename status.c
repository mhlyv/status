#include <stdio.h>
#include <stdlib.h>

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
    const char * format;
    String (*func) ();
};

static size_t size(char *);
static inline String string(size_t, char *);
#define STRING(str) \
    string(size(str), str)

static inline Number number(String, String, String);
#define NUMBER(integer, fraction, exponent) \
    number(STRING(integer),  \
           STRING(fraction), \
           STRING(exponent)  )

static inline Data string_data(String);
#define DATA_TYPE_STRING(data) \
    string_data(STRING(data))

static inline Data number_data(Number);
#define DATA_TYPE_NUMBER(data...) \
    number_data(NUMBER(data))

static inline Value value(Type, Data);
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
        .data = data
    };

    return str;
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

int
main()
{
    Value head = VALUE(TYPE_NUMBER, "1", "", "");
    return 0;
}
