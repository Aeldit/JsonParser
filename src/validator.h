#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <stdbool.h>
#include <stdio.h>

bool is_json_valid_buff(char *buff, size_t buff_len, bool is_dict);
bool is_json_valid(FILE *buff);

#endif // !VALIDATOR_H
