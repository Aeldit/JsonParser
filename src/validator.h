#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <stdbool.h>
#include <stdio.h>

bool is_json_valid_buff(char *buff, size_t buff_len, bool is_dict);
bool is_json_valid_file(FILE *f, bool is_dict);

#endif // !VALIDATOR_H
