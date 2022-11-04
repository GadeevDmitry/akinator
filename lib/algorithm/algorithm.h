#ifndef ALGORITHM_H
#define ALGORITHM_H

/*_________________________________________FUNCTION_DECLARATIONS_________________________________________*/

void    my_swap                (void *a, void *b,  const int elem_size);
void   *get_ptr                (void *begin_array, const int index,     const int elem_size);

void    my_quick_sort          (void        *data, const int elem_size, const int      left, 
                                                                        const int     right,
                                                                        int (*cmp) (void *, void *));

int     is_empty_input_stream  (FILE *const stream);
void       clear_input_stream  (FILE *const stream);

void    get_line               (char *s, const int max_size, FILE *const stream);
void    get_word               (char *s, const int max_size, FILE *const stream);
void    skip_spaces            (FILE *const stream);

int     is_empty_input_buff    (const char *buff, const int buff_size, size_t *const pos);
void       clear_input_buff    (const char *buff, const int buff_size, size_t *const pos);

void    get_line               (char *s, const int max_size, const char *buff, const int buff_size, size_t *const pos);
void    get_word               (char *s, const int max_size, const char *buff, const int buff_size, size_t *const pos);

/*_______________________________________________________________________________________________________*/

#endif //ALGORITHM_H