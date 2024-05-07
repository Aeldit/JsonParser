#ifndef JSON_PARSER_H
#define JSON_PARSER_H

struct fc_control *read_file(char *path);

void print_file_content(struct fc_control *fcc);

#endif // !JSON_PARSER_H
