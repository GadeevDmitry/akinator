#ifndef ALGORITHM_H
#define ALGORITHM_H

/*________________________FUNCTION_DECLARATIONS________________________*/

void    my_swap             (void *a, void *b,  const int elem_size);
void   *get_ptr             (void *begin_array, const int index,     const int elem_size);

void    my_quick_sort       (void        *data, const int elem_size, const int      left, 
                                                                     const int     right,
                                                                     int (*cmp) (void *, void *));

int     is_empty_input_buff (FILE *const stream);
void       clear_input_buff (FILE *const stream);

/*_____________________________________________________________________*/

#endif //ALGORITHM_H