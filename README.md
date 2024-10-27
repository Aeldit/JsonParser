# Json Parser

![logo](https://raw.githubusercontent.com/Aeldit/Aeldit/2c162a9bf611658c32247bd5bba500a30d1b6ad9/github_profile/json-parser.svg)

![nvim](https://raw.githubusercontent.com/Aeldit/Aeldit/267a0998a55b0462b042d3e471c02f137dbad551/images/nvim.svg)
![c](https://raw.githubusercontent.com/Aeldit/Aeldit/267a0998a55b0462b042d3e471c02f137dbad551/images/made_with_c.svg)

The goal of this project is to allow its user to give the script a `json` file and be able to store it and access its elements in C.

> [!WARNING]\
> This parser does not sanitize the content it reads (for now, at least). Use at your own risk.

## Description

For now, the best way to see the script working is to print the contents of the json in the terminal, which is what the script does when launched.

User friendly usage has not been thought about yet, but I will

## Build

To build the script, clone the repository on your computer :

```shell
git clone git@github.com:Aeldit/JsonParser.git
cd JsonParser
```

Then run the following commands :

> If you want to compile using `fsanitize`, use `./configure S`,
> and if you want to compile using `gdb`, use `./configure D`

```shell
./configure
make
./json-parser <your_file.json>
```

## Usage

If you want to use this parser in your C code, put the source and header files of this project in yours :

> The third line is to rename the directory

```shell
cp -r JsonParser/src/ <your_code_directory>
cd <your_code_directory>
mv src/ json-parser/
```

To read a json file in your code, call the `parse()` function :
```c
JSON *j = parse(file_path);
// Once you have this JSON instance, you first have to check whether it is an array or a dict :

// The macros 'IS_ARRAY' and 'IS_DICT' check if 'j' is NULL, if 'j' is an array
// and if j->array is not null
if (IS_ARRAY(j))
{
    Array *a = j->array;
    // Do stuff with the array
}
else if (IS_DICT(j))
{
    Dict *d = j->dict;
    // Do stuff with the dict
}
```

The arrays contains `Value` elements, while the dicts contain `Item` elements :

> Also note the usage of `String`, it is a simple typedef struct

<center>
<table>
<tr>
<th>Value</th>
<th>Item</th>
<th>String</th>
</tr>
<tr>
<td>

```c
typedef struct
{
    char type;
    String strv;
    int intv;
    double doublev;
    char boolv;
    Array *arrayv;
    Dict *dictv;
} Value;
```

</td>
<td>

```c
typedef struct
{
    char type;
    String key;
    String strv;
    int intv;
    double doublev;
    char boolv;
    Array *arrayv;
    Dict *dictv;
} Item;
```

</td>
<td>

```c
typedef struct
{
    char *str;
    uint_strlen_t length;
} String;
```

</td>
</tr>
</table>
</center>

To access the elements of the array or dict :

```c
// For arrays
Value v = array_get(a, index);

// For dicts
Item it = dict_get(d, key);

// The rest is the same for both, except that the arrays don't have keys
if (v.type == T_ERROR)
{
    // Handle error
}
// Next we have to check what the type of the value is :
switch (v.type)
{
case T_STR:
    // Do stuff with string
    break;
case T_INT:
    // Do stuff with integer
    break;
case T_DOUBLE:
    // Do stuff with double
    break;
case T_BOOL:
    // Do stuff with boolean
    break;
case T_NULL:
    // Do stuff with null
    break;
case T_ARR:
    // Do stuff with array
    break;
case T_DICT:
    // Do stuff with dict
    break;
}
```

