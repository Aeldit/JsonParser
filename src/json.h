#ifndef JSON_H
#define JSON_H

/*******************************************************************************
**                                  INCLUDES                                  **
*******************************************************************************/
#include "json_types.h"
#include "linked_lists.hpp"

/*******************************************************************************
**                                 STRUCTURES                                 **
*******************************************************************************/
/**************************************
**               JSON                **
**************************************/
struct json_array
{};

struct json_dict
{};

struct JSON
{
    char is_array;
    struct json_array ja;
    struct json_dict jd;
};

/**************************************
**           TYPED VALUES            **
**************************************/
class StringValue : public Value
{
private:
    String *value;

public:
    StringValue(String *value);
    ~StringValue();

    void printNoFlush();
    String *getValue();
};

class IntValue : public Value
{
private:
    int_fast64_t value;

public:
    IntValue(int_fast64_t value);

    void printNoFlush();
    int_fast64_t getValue();
};

class DoubleValue : public Value
{
private:
    double value;

public:
    DoubleValue(double value);

    void printNoFlush();
    double getValue();
};

class BoolValue : public Value
{
private:
    bool value;

public:
    BoolValue(bool value);

    void printNoFlush();
    bool getValue();
};

class NullValue : public Value
{
public:
    NullValue();

    void printNoFlush();
};

class ArrayValue : public Value
{
private:
    JSONArray *ja;

public:
    ArrayValue(JSONArray *ja_arg);
    virtual ~ArrayValue();

    void printNoFlush();
    void print();
    JSONArray *getValue();
};

class DictValue : public Value
{
private:
    JSONDict *jd;

public:
    DictValue(JSONDict *jd_arg);
    virtual ~DictValue();

    void printNoFlush();
    void print();
    JSONDict *getValue();
};

/**************************************
**               ITEMS               **
**************************************/
class StringItem : public Item
{
private:
    String *value;

public:
    StringItem(String *key, String *value);
    ~StringItem();

    void printNoFlush();
    String *getValue();
};

class IntItem : public Item
{
private:
    int64_t value;

public:
    IntItem(String *key, int64_t value);

    void printNoFlush();
    int64_t getValue();
};

class DoubleItem : public Item
{
private:
    double value;

public:
    DoubleItem(String *key, double value);

    void printNoFlush();
    double getValue();
};

class BoolItem : public Item
{
private:
    bool value;

public:
    BoolItem(String *key, bool value);

    void printNoFlush();
    bool getValue();
};

class NullItem : public Item
{
public:
    NullItem(String *key);

    void printNoFlush();
};

class ArrayItem : public Item
{
private:
    JSONArray *ja;

public:
    ArrayItem(String *key, JSONArray *ja_arg);
    virtual ~ArrayItem();

    void printNoFlush();
    void print();
    JSONArray *getValue();
};

class DictItem : public Item
{
private:
    JSONDict *jd;

public:
    DictItem(String *key, JSONDict *jd_arg);
    virtual ~DictItem();

    void printNoFlush();
    void print();
    JSONDict *getValue();
};

#endif // !JSON_H
