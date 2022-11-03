#ifndef TREE_BINARY_H
#define TREE_BINARY_H

const int SIZE_DATA = 100;
const int SIZE_MODE =  10;

struct Tree_node
{
    char data[SIZE_DATA];

    Tree_node *prev;
    Tree_node *left;
    Tree_node *right;
};

const Tree_node node_default =
{
    "некто", // data

    nullptr, // prev
    nullptr, // left
    nullptr  // right
};

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define MODE(name, num, ...)        \
        MODE_##name = num,
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

enum MODE
{
    #include "mode.h"
    MODE_FICTIONAL
};

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#undef MODE
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/*_____________________________________FUNCTION_DECLARATIONS_____________________________________*/

void Tree_dump_graph_make(const Tree_node *tree,    const char *call_file,
                                                    const char *call_func,
                                                    const int   call_line);

bool print_guess         (const Tree_node *const node, char *const difference, char *const creature);
bool print_quation       (const Tree_node *const node);
bool yes_no              ();

void save_data           (Tree_node *node);
void fill_output_file    (Tree_node *node, FILE *const stream, int tab_shift);
void parsing             (const char *filename);
void tab                 (FILE *const stream, int n);

void mode_guess          (Tree_node *const node);
/*_______________________________________________________________________________________________*/

#endif //TREE_BINARY_H