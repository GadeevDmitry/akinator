#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

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
void    skip_spaces            (                             const char *buff, const int buff_size, size_t *const pos);

/*___________________________________________USULAL_FUNCTIONS____________________________________________*/

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

/*__________________________________________ALGORITHM_FUNCTIONS__________________________________________*/

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

/*_________________________________________FUNCTIONS_WITH_STREAM_________________________________________*/

int is_empty_input_stream(FILE *const stream)
{
    assert(stream);

    int     is_empty_temp = ' ';
    while ((is_empty_temp = getc(stream)) == ' ' || is_empty_temp == '\t')
        ;
    ungetc(is_empty_temp, stream);
    
    if (is_empty_temp == '\n' || is_empty_temp == EOF) return 1;

    return 0;
}

void clear_input_stream(FILE *const stream)
{
    assert(stream != NULL);

    int     clear_temp ='#';
    while ((clear_temp = getc(stream)) != '\n' && clear_temp != EOF)
        ;
    ungetc(clear_temp, stream);
}

void get_line(char *s, const int max_size, FILE *const stream)
{
    assert(s     );
    assert(stream);

    skip_spaces(stream);

    int  cnt = 0;
    for (cnt = 0; cnt < max_size - 1; ++cnt)
    {
        int cur_char =  getc(stream);
        if (cur_char == EOF || cur_char == '\n')
        {
            ungetc(cur_char, stream);
            break;
        }
        
        s[cnt] = cur_char;
    }
    while (isspace(s[cnt - 1]) && cnt > 0)
        --cnt;
    s[cnt] = '\0';
}

void get_word(char *s, const int max_size, FILE *const stream)
{
    assert(s     );
    assert(stream);

    skip_spaces(stream);

    int  cnt = 0;
    for (cnt = 0; cnt < max_size - 1; ++cnt)
    {
        //<<<<<<<<<<<<<<<<<<<<<<<
        //fprintf(stderr, "DO\n");
        //>>>>>>>>>>>>>>>>>>>>>>>

        int cur_char =  getc(stream);
        if (cur_char == EOF || isspace(cur_char))
        {
            ungetc(cur_char, stream);
            break;
        }

        s[cnt] = cur_char;
    }
    s[cnt] = '\0';
}

void skip_spaces(FILE *const stream)
{
    assert(stream);

    int cur_char = ' ';
    while (isspace(cur_char = getc(stream)))
        ;
    ungetc(cur_char, stream);
}

/*_________________________________________FUNCTIONS_WITH_BUFFER_________________________________________*/

int is_empty_input_buff(const char *buff, const int buff_size, size_t *const pos)
{
    assert(buff);
    assert(pos );

    while (*pos < buff_size && (buff[*pos] == ' ' || buff[*pos] == '\t'))
        ++ *pos;

    if (*pos == buff_size || buff[*pos] == '\n' || buff[*pos] == '\0') return 1;

    return 0;
}

void clear_input_buff(const char *buff, const int buff_size, size_t *const pos)
{
    assert(buff);
    assert(pos );

    while (*pos < buff_size && buff[*pos] != '\n' && buff[*pos] != '\0')
        ++ *pos;
}

void get_line(char *s, const int max_size, const char *buff, const int buff_size, size_t *const pos)
{
    assert(s   );
    assert(buff);
    assert(pos );

    int  limit = (buff_size < max_size) ? buff_size : max_size - 1;

    int  cnt = 0;
    for (cnt = 0; cnt < limit; ++cnt)
    {
        if (buff[*pos] == '\0' || buff[*pos] == '\n') break;
        
        s[cnt] = buff[*pos];
        ++*pos;
    }
    while (isspace(s[cnt - 1]) && cnt > 0)
        --cnt;
    s[cnt] = '\0';
}

void get_word(char *s, const int max_size, const char *buff, const int buff_size, size_t *const pos)
{
    assert(s     );
    assert(buff  );
    assert(pos   );

    int limit = (buff_size < max_size) ? buff_size : max_size - 1;

    int  cnt = 0;
    for (cnt = 0; cnt < limit; ++cnt)
    {
        if (buff[*pos] == '\0' || isspace(buff[*pos])) break;

        s[cnt] = buff[*pos];
        ++*pos;
    }
    s[cnt] = '\0';
}

void skip_spaces(const char *buff, const int buff_size, size_t *const pos)
{
    assert(buff);
    assert(pos );

    while (*pos < buff_size)
    {
        if (!isspace(buff[*pos])) break;
        ++*pos;
    }
}
/*_______________________________________________________________________________________________________*/