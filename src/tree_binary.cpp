#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "../lib/logs/log.h"
#include "../lib/graph_dump/graph_dump.h"
#include "../lib/algorithm/algorithm.h"
#include "../lib/read_write/read_write.h"

#include "tree_binary.h"

#define get_line_stream(s, max_size, stream)                                                                    \
        get_line       (s, max_size, stream);                                                                   \
                                                                                                                \
        if (!is_empty_input_stream(stream))                                                                     \
        {                                                                                                       \
            clear_input_stream(stream);                                                                         \
                                                                                                                \
            fprintf(stderr, "You message is so long. Please print not more than %d characters.\n", SIZE_DATA);  \
            continue;                                                                                           \
        }

const int SIZE_ANS = 20;

void Tree_dump_graph_make(const Tree_node *tree,    const char *call_file,
                                                    const char *call_func,
                                                    const int   call_line)
{
    assert(call_file);
    assert(call_func);
}

/*_______________________________________________________________________________________________*/

void ctor_tree_node(Tree_node *node, Tree_node *prev, const char *node_data)
{
    assert(node);

   *node              = node_default;
    node->prev        = prev;
    node->visit_left  = false;
    node->visit_right = false;

    if    (node_data == nullptr) return;
    memcpy(node->data, node_data, SIZE_DATA);
}

void mode_guess(Tree_node *node)
{
    assert(node);

    if (node->left == nullptr && node->right == nullptr)
    {
        char *creature   = (char *) calloc(sizeof(char), SIZE_DATA);
        char *difference = (char *) calloc(sizeof(char), SIZE_DATA);

        if (!print_guess(node, difference, creature))
        {
            node->left  = (Tree_node *) calloc(sizeof(Tree_node), 1);
            node->right = (Tree_node *) calloc(sizeof(Tree_node), 1);

            ctor_tree_node(node->right, node, node->data);
            ctor_tree_node(node->left , node, creature  );

            memcpy(node->data, difference, SIZE_DATA);
        }

        free(creature);
        free(difference);

        fprintf(stderr, "Game over. Chose the mode.\n");
        return;
    }
    if (print_quation(node)) node = node->left;
    else                     node = node->right;

    mode_guess(node);
}

/*_______________________________________________________________________________________________*/

bool print_guess(const Tree_node *const node, char *const difference, char *const creature)
{
    assert(node      );
    assert(difference);
    assert(creature  );

    fprintf(stderr, "Is \"%s\" your guess?\n", node->data);

    bool ans = yes_no();

    if  (ans)
    {
        fprintf(stderr, "It was very easy)\n");
        return true;
    }

    while (true)
    {
        fprintf(stderr, "Who have you guessed?\n");    
    
        get_line_stream(creature, SIZE_DATA, stdin);
        if (!strcasecmp(creature, "exit")) exit(0);
    
        fprintf (stderr, "What the difference between \"%s\" and \"%s\"?\n", creature, node->data);

        get_line_stream(difference, SIZE_DATA, stdin);
        if (!strcasecmp(difference, "exit")) exit(0);

        return false;
    }

    return false;
}

bool print_quation(const Tree_node *const node)
{
    assert(node);

    fprintf(stderr, "Is your character %s?\n", node->data);
    return yes_no();
}

/*_______________________________________________________________________________________________*/

bool yes_no()
{
    char answer[SIZE_ANS] = "";

    while (true)
    {
        get_word(answer, SIZE_ANS, stdin);

        if (!is_empty_input_stream(stdin))
        {
            clear_input_stream(stdin);

            fprintf(stderr, "Undefined answer. Print \"yes\" or \"no\"\n");
            continue;
        }
        if (!strcasecmp("yes" , answer)) return true ;
        if (!strcasecmp("no"  , answer)) return false;
        if (!strcasecmp("exit", answer)) exit(0);

        fprintf(stderr, "Undefined answer. Print \"yes\" or \"no\"\n");
    }

    return true;
}

/*_______________________________________________________________________________________________*/

void save_data(Tree_node *node)
{
    fprintf(stderr, "Tell me the name of file to save the data base in (or print \"-\" if you don't want to save it), before I stop the program\n");

    char filename[SIZE_DATA] =      "";
    FILE  *stream            = nullptr;

    while (true)
    {
        get_word(filename, SIZE_DATA, stdin);

        if (!is_empty_input_stream(stdin))
        {
            clear_input_stream(stdin);

            fprintf(stderr, "Undefined name of file. Tell me another name\n");
            continue;
        }
        if (!strcmp("-", filename)) return;

        stream = fopen(filename, "w");

        if (stream == nullptr)
        {
            fprintf(stderr, "I can't open this file(. Tell me another name\n");
            continue;
        }

        fill_output_file(node, stream, 0);
        fclose          (stream);
        return;
    }  
}

/*_______________________________________________________________________________________________*/

#define wrong_file_fmt()                                                                        \
        {                                                                                       \
        fprintf(stderr, "Wrong format of \"%s\"\n", filename);                                  \
        return false;                                                                           \
        }

bool read_input_base(Tree_node *root, const char *filename)
{
    assert(root    );
    assert(filename);

    Tree_node *node = nullptr;

    size_t data_size = 0;
    size_t data_pos  = 0;
    char  *data_base =  (char *) read_file(filename, &data_pos);
    if    (data_base == nullptr)
    {
        fprintf(stderr, "I can't open this file(. Tell me another name\n");
        return false;
    }

    while (!is_empty_input_buff(data_base, data_size, &data_pos))
    {
        char c = data_base[data_pos++];
        if  (c == '}')
        {
            if (node == nullptr) wrong_file_fmt();

            node->visit_left  = false;
            node->visit_right = false;
            node              = node->prev;
        }
        else if  (c == '{')
        {
            if (node == nullptr) node = root;
            else
            {
                if (node->left == nullptr)
                {
                    node->left  = (Tree_node *) calloc(1, sizeof(Tree_node));
                    node->right = (Tree_node *) calloc(1, sizeof(Tree_node));

                    ctor_tree_node(node->left , node, nullptr);
                    ctor_tree_node(node->right, node, nullptr);
                }
                if (node->visit_left)
                {
                    if (node->visit_right)  wrong_file_fmt();

                    node->visit_right = true;
                    node              = node->right;
                }
                else
                {
                    node->visit_left = true;
                    node             = node->left;
                }
            }
        }
        else wrong_file_fmt();

        if (!update_node(node, data_base, data_size, &data_pos)) wrong_file_fmt();
    }

    if (node == nullptr) return true;
    wrong_file_fmt();
}

#undef wrong_file_fmt

bool update_node(Tree_node *node, const char *buff, const int buff_size, size_t *const pos)
{
    assert(node);
    assert(buff);
    assert(pos );

    char node_data[SIZE_DATA] = "";

    if (!get_node_data(node_data, SIZE_DATA, buff, buff_size, pos)) return false;

    memcpy(node->data, node_data, SIZE_DATA);
    return true;
}

bool get_node_data(char *s, const int max_size, const char *buff, const int buff_size, size_t *const pos)
{
    assert(s   );
    assert(buff);
    assert(pos );

    bool  ans = false;
    int limit = (buff_size < max_size) ? buff_size : max_size - 1;

    int  cnt = 0;
    for (cnt = 0; cnt < limit; ++cnt)
    {
        if (*pos == buff_size || buff[*pos] == '}' || buff[*pos] == '\n')
        {
            ans = true;
            break;
        }

        s[cnt] = buff[*pos];
        ++*pos;
    }

    s[cnt] = '\0';
    return ans;
}

void fill_output_file(Tree_node *node, FILE *const stream, int tab_shift)
{
    assert(node  );
    assert(stream);

    tab    (stream, tab_shift);
    fprintf(stream, "{%s", node->data);

    if (node->left == nullptr) fprintf(stream, "}\n");
    else
    {
        putc('\n', stream);
        fill_output_file(node->left , stream, tab_shift + 1);
        fill_output_file(node->right, stream, tab_shift + 1);
    }

    tab    (stream, tab_shift);
    fprintf(stream, "}\n");
}

/*_______________________________________________________________________________________________*/

void tab(FILE *const stream, int n)
{
    while (n--) putc('\t', stream);
}

/*_______________________________________________________________________________________________*/