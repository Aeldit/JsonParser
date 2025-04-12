# Json Parser

![logo](https://raw.githubusercontent.com/Aeldit/Aeldit/2c162a9bf611658c32247bd5bba500a30d1b6ad9/github_profile/json-parser.svg)
![c](https://raw.githubusercontent.com/Aeldit/Aeldit/7abcf209fcfe3cbc0f29ffdc22668087fc9cea67/images/made-with-c11.svg)

This parser has 2 modes: **read-only** and **read-write**, which will be refered to respectively as `ro` and `rw`

## Performance

Here is a little benchmark with 3 files (`t.json`, `big.json` and `flights-m1.json`), run
with the command `time ./json-parser <json file>`:

> `S` => sys time and `U` => usr time
> | `ms` => milliseconds and `s` => seconds

| `t.json` | `big.json` | `flights-m1.json` |
|:------:|:--------:|:---------------:|
| 3.06ms U=2.06ms S=1.06ms | 51.85ms U=39.63ms S=11.93ms | 2.26s U=1.87s S=0.33s |

> `t.json` is 680 bytes | `big.json` is 4.9MB | `flights-m1.json` is 119MB
>
> `flights-m1.json` being above GitHub's 100MB limit so I cannot include it in the repo, but
> it has a million lines like this one: `{"FL_DATE":"2006-01-01","DEP_DELAY":5,"ARR_DELAY":19,"AIR_TIME":350,"DISTANCE":2475,"DEP_TIME":9.083333,"ARR_TIME":12.483334}`


## Usage

If you want to use this parser in your C code, put the `src` directory of the parser in your project's directory (probably rename in to `json_parser` before to not overwrite your project's `src` dir)

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
  enum elt_type type;
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
  enum elt_type type;
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
- `S` : Runs the parser with the `-fsanitize=address` gcc flag (checks for memory leaks)
- `D` : Runs the parser with the flags `-g -ggdb -DDEBUG` (prints some debug info)
- `SD` or `DS` : Use both options


## Makefile rules

Base rules:
- `all` : compiles and runs the program in `ro` mode with the file `t.json`
- `rw` : same as `all` but  in `rw` mode (the main file is `rw_main.c`)
- `mem-least` : compile using the `least` types of stdint
- `noprint` : same as `all` but printing is disabled (using `-DVALGRING_DISABLE_PRINT`)
- `noprintrw` : same as noprint but in `rw` mode
- `json-parser` : Compiles the project in `ro` mode
- `clean` : removes the executable

Testing rules:
- `valgrind-compile` : compiles the parser with the `-DVALGRING_DISABLE_PRINT` flag (disables the printing functions to only have the time of the parsing functions when using a profiler)
- `calgrind` : calls `valgrind-compile` and generates a callgrind file usable by the `KCachegrind` profiling software
- `leaks` : checks for leaks but using valgrind (using the file `t.json`)
- `check` : runs the criterion test suite

Publishing rules:
- `release` : compiles the parser using the `-O2` flag
- `releaserw` : same as `release` but in `rw` mode
- `lib` : generates a shared library (`.so` file)

## Compilation options

You can change some defines directly at compilation time, depending on your use of this parser

#### MAX_READ_BUFF_SIZE

Defines the maximum size of the allocated buffer that is used to store the file (defaults to `2 << 30`, which is roughly equals to 1GB)

#### LEAST

By default, the program uses the `(u)int_fastX_t` types. If the program is run using `-DLEAST`, the types used will be `(u)int_leastX_t`.

