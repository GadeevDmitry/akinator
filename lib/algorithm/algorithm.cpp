#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*________________________FUNCTION_DECLARATIONS________________________*/

void    my_swap             (void *a, void *b,  const int elem_size);
void   *get_ptr             (void *begin_array, const int index,     const int elem_size);

void    my_quick_sort       (void        *data, const int elem_size, const int      left, 
                                                                     const int     right,
                                                                     int (*cmp) (void *, void *));

int     is_empty_input_buff (FILE *const stream);
void       clear_input_buff (FILE *const stream);

/*_____________________________________________________________________*/

void my_swap(void *a, void *b, const int elem_size)
{
    assert(a);
    assert(b);

    unsigned char temp = '\0';

    for (size_t i = 0; i < elem_size; ++i)
    {
        temp = *((unsigned char *)a + i);
               *((unsigned char *)a + i) = *((unsigned char *)b + i);
               *((unsigned char *)b + i) = temp;
    }
}

void *get_ptr(void *begin_array, const int index, const int elem_size)
{
    assert(begin_array);

    void  *ptr = (unsigned char *)begin_array + index * elem_size;
    return ptr;
}

void my_quick_sort(void *data, const int elem_size, const int left, const int right, int (*cmp) (void * elem1, void * elem2))
{
    assert(data);
    assert(cmp );

    if        (left >= right) return;
    int mid = (left  + right) / 2;

    my_swap(get_ptr(data, left, elem_size), get_ptr(data, mid, elem_size), elem_size);

    int cut = left;
    for (int i = left + 1; i <= right; ++i) {

        if ((*cmp)(get_ptr(data, i, elem_size), get_ptr(data, left, elem_size)) <= 0) {

            ++cut;

            my_swap(get_ptr(data, cut, elem_size), get_ptr(data, i, elem_size), elem_size);
        }
    }

    my_swap(get_ptr(data, cut, elem_size), get_ptr(data, left, elem_size), elem_size);

    my_quick_sort(data, elem_size,    left, cut - 1, cmp);
    my_quick_sort(data, elem_size, cut + 1,   right, cmp);
}

int is_empty_input_buff(FILE *const stream)
{
    assert(stream);

    int     is_empty_temp = ' ';
    while ((is_empty_temp = getc(stream)) == ' ' || is_empty_temp == '\t')
        ;
    ungetc(is_empty_temp, stream);

    if (is_empty_temp == '\n' || is_empty_temp == EOF) return 1;

    return 0;
}

void clear_input_buff(FILE *const stream)
{
    assert(stream != NULL);

    int     clear_temp ='#';
    while ((clear_temp = getc(stream)) != '\n' && clear_temp != EOF)
        ;
    if (clear_temp == '\n') ungetc(clear_temp, stream);
}