# Json Parser

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

> Make sure the `configure` file has execution permission (`chmod +x configure`)
>
> If you want to compile using `gdb` or `fsanitize`, use `./configure DEBUG` instead of `./configure`

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


