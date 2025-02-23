#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <stdio.h>

char is_json_valid_buff(char *buff, char is_dict);
char is_json_valid(FILE *buff);

#endif // !VALIDATOR_H
