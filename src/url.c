#include "avatars_gen.h"



/* Fetch initials from URI and return result status */
int get_initials_from_uri(ngx_str_t *uri, unsigned char *initials) {
    unsigned char *curr_character = (uri->data + uri->len - 1);
    unsigned int characters_copied = 0;
    /* Skip trailing / and ? symbols */
    while ((*curr_character == '/') || (*curr_character == '?')) {
        curr_character--;
    }
    while ((*curr_character != '/') && (characters_copied != INITIALS_MAX_SIZE) && (curr_character != uri->data)) {
        initials[characters_copied] = *curr_character;
        characters_copied++;
        curr_character--;
    }
    if (*curr_character != '/') {
        return 0;
    }
    /* Revert bytes */
    unsigned char tmp;
    unsigned int i;
    for (i = 0; i < (characters_copied / 2); i++) {
        tmp = initials[i];
        initials[i] = initials[characters_copied - i - 1];
        initials[characters_copied - i - 1] = tmp;
    }
    initials[characters_copied] = '\0';
    /* Validate length */
    size_t utf8_count = ngx_utf8_length(initials, characters_copied - 1);
    if ((utf8_count > 2) || (utf8_count < 1)) {
        return 0;
    }
    return 1;
}
