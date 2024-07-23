# Json Parser

![nvim](https://raw.githubusercontent.com/Aeldit/Aeldit/267a0998a55b0462b042d3e471c02f137dbad551/images/nvim.svg)
![c](https://raw.githubusercontent.com/Aeldit/Aeldit/267a0998a55b0462b042d3e471c02f137dbad551/images/made_with_c.svg)

The goal of this project is to allow its user to give the script a `json` file and be able to store it and access its elements in C.

> [!WARNING]\
> This parser does not sanitize the content it reads (yet, at least). Use at your own risk.

## Usage

To use the script, clone the repository on your computer :

```shell
git clone git@github.com:Aeldit/JsonParser.git
cd JsonParser
```

Then run the following commands :

```shell
make
./json-parser <your_file.json>
```

