# Json Parser

![logo](https://raw.githubusercontent.com/Aeldit/Aeldit/2c162a9bf611658c32247bd5bba500a30d1b6ad9/github_profile/json-parser.svg)
![c](https://raw.githubusercontent.com/Aeldit/Aeldit/7abcf209fcfe3cbc0f29ffdc22668087fc9cea67/images/made-with-c11.svg)

This parser has 2 modes: **read-only** and **read-write**, which will be refered to respectively as `ro` and `rw`


## Usage

If you want to use this parser in your C code, put the source and header files of this project in yours :

> The third line is to rename the directory

```shell
cp -r JsonParser/src/ <your_code_directory>
cd <your_code_directory>
mv src/ json-parser/
```


> Notice how each struct (json, array, dict, value, item) or the parse function is prefixed with `ro`. For read-write mode, they will be prefixed by `rw`

To read a json file in `ro` mode, call the `ro_parse()` function :
```c
ro_json_t j = ro_parse(file_path);
// Once you have this ro_json_t struct pointer, you first have to check whether it is an array or a dict :
if (j.is_array)
{
    ro_array_t a = j.array;
    // Do stuff with the array
}
else
{
    ro_dict_t d = j.dict;
    // Do stuff with the dict
}
```

Arrays contain `ro_value_t` elements, while dicts contain `ro_item_t` elements :

<center>
<table>
<tr>
<th>ro_value_t</th>
<th>ro_item_t</th>
<th>string_t</th>
</tr>
<tr>
<td>

```c
typedef struct
{
  u8 type;
  union
  {
    string_t strv;
    i64 longv;
    double doublev;
    exp_long_t exp_longv;
    exp_double_t exp_doublev;
    bool boolv;
    ro_array_t arrayv;
    ro_dict_t dictv;
  };
} ro_value_t;
```

</td>
<td>

```c
typedef struct
{
  u8 type;
  string_t key;
  union
  {
    string_t strv;
    i64 longv;
    double doublev;
    exp_long_t exp_longv;
    exp_double_t exp_doublev;
    bool boolv;
    ro_array_t arrayv;
    ro_dict_t dictv;
  };
} ro_item_t;
```

</td>
<td>

```c
typedef struct
{
  char *str;
  size_t len;
  bool needs_freeing;
} string_t;
```

</td>
</tr>
</table>
</center>

To access the elements of the array or dict :

```c
ro_value_t v = ro_array_get(a, index); // For arrays
ro_item_t it = ro_dict_get(d, key); // For dicts

// Next we have to check what the type of the value is :
switch (v.type)
{
case T_ERROR:
    // Handle error
    break;
case T_STR:
    // Do stuff with string
    break;
case T_LONG:
    // Do stuff with number
    break;
case T_DOUBLE:
    // Do stuff with double
    break;
case T_EXP_LONG:
    // Do stuff with exponent number
    break;
case T_EXP_DOUBLE:
    // Do stuff with exponent double
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


## Build

To build the program, first clone it from GitHub :

```shell
git clone https://github.com/Aeldit/JsonParser.git
```

Then, enter the following commands

```shell
./configure
make json-parser
./json-parser <your_json_file.json>
```

The configure script accepts the following options :
- `S` : Runs the script with the `-fsanitize=address` gcc flag (checks for memory leaks)
- `D` : Displays some debug informations
- `SD` or `DS` : Use both options


## Makefile rules

Base rules :
- `all` : compiles and runs the program in `ro` mode with the file `t.json`
- `rw` : same as `all` but  in `rw` mode (the main file is `rw_main.c`)
- `mem-least` : compile using the `least` types of stdint
- `noprint` : same as `all` but printing is disabled (using `-DVALGRING_DISABLE_PRINT`)
- `noprintrw` : same as noprint but in `rw` mode
- `json-parser` : Compiles the project in `ro` mode
- `clean` : removes the executable

Valgrind rules :
- `valgrind-compile` : compiles the parser with the `-DVALGRING_DISABLE_PRINT` flag (disables the printing functions to only have the time of the parsing functions when using a profiler)
- `calgrind` : calls `valgrind-compile` and generates a callgrind file usable by the `KCachegrind` profiling software
- `leaks` : checks for leaks but using valgrind (using the file `t.json`)

## Compilation options

You can change some defines directly at compilation time, depending on your use of this parser

#### MAX_READ_BUFF_SIZE

Defines the maximum size of the allocated buffer that is used to store the file (defaults to `2 << 30`, which is roughly equals to 1GB)

