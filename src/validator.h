// clang-format Language: C
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <stdbool.h>
#include <stdio.h>

bool is_json_valid(const char *buff, size_t buff_len, bool is_dict);

#endif // !VALIDATOR_H
