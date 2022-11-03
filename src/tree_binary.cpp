#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../lib/logs/log.h"
#include "../lib/graph_dump/graph_dump.h"
#include "../lib/algorithm/algorithm.h"
#include "../lib/read_write/read_write.h"

#include "tree_binary.h"

const int SIZE_ANS = 20;

void Tree_dump_graph_make(const Tree_node *tree,    const char *call_file,
                                                    const char *call_func,
                                                    const int   call_line)
{
    assert(call_file);
    assert(call_func);
}

/*_______________________________________________________________________________________________*/

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

            memcpy(node->right->data, node->data, SIZE_DATA);
            memcpy(node->left-> data, creature  , SIZE_DATA);
            memcpy(node->data       , difference, SIZE_DATA);
        }

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
    fprintf(stderr, "Who have you guessed?\n");    
    
    scanf("%99s", creature);
    if   (!strcasecmp("exit", creature)) exit(0);
    
    fprintf (stderr, "What the difference between \"%s\" and \"%s\"?\n", creature, node->data);

    scanf("%99s", difference);
    if   (!strcasecmp("exit", difference)) exit(0);

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
        scanf("%19s", answer);

        if (!is_empty_input_buff(stdin))
        {
            clear_input_buff(stdin);

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

    char filename[SIZE_DATA] = "";
    FILE  *stream =  nullptr;

    while (true)
    {
        scanf("%99s", filename);

        if (!strcmp("-", filename)) return;
        
        stream = fopen(filename, "w");

        if (stream == nullptr) fprintf(stderr, "I can't open this file(. Tell me another name\n");
        else
        {
            fill_output_file(node, stream, 0);
            fclose(stream);
            return;
        }
    }  
}

/*_______________________________________________________________________________________________*/

void parsing(const char *filename)
{
    assert(filename);

    size_t data_size = 0;
    char  *data_base = (char *) read_file(filename, &data_size);
    if    (data_base == nullptr)
    {
        fprintf(stderr, "I can't open this file(. Tell me another name\n");
        return;
    }
}

void fill_output_file(Tree_node *node, FILE *const stream, int tab_shift)
{
    assert(node  );
    assert(stream);

    tab    (stream, tab_shift);
    fprintf(stream, "{ %s ", node->data);

    if (node->left == nullptr) fprintf(stream, "}\n");
    else
    {
        putc('\n', stream);
        fill_output_file(node->left , stream, tab_shift + 1);
        fill_output_file(node->right, stream, tab_shift + 1);
    }
}

void tab(FILE *const stream, int n)
{
    while (n--) putc('\t', stream);
}

/*_______________________________________________________________________________________________*/
