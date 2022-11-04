#ifndef TREE_BINARY_H
#define TREE_BINARY_H

const int SIZE_DATA = 100;
const int SIZE_MODE =  10;

struct Tree_node
{
    char data[SIZE_DATA];
    
    bool visit_left;
    bool visit_right;

    Tree_node *prev;
    Tree_node *left;
    Tree_node *right;
};

const Tree_node node_default =
{
    "некто", // data

    false  , // visit_left
    false  , // visit_right

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

void ctor_tree_node     (Tree_node *node, Tree_node *prev, const char *node_data);

void mode_guess         (Tree_node *node);
bool print_guess        (const Tree_node *const node, char *const difference, char *const creature);
bool print_quation      (const Tree_node *const node);

void save_data          (Tree_node *node);
void fill_output_file   (Tree_node *node, FILE *const stream, int tab_shift);

void mode_download      ();
bool read_input_base    (const char *filename);
bool update_node        (Tree_node *node,             const char *buff, const int buff_size, size_t *const pos);
bool get_node_data      (char *s, const int max_size, const char *buff, const int buff_size, size_t *const pos);

bool yes_no             ();
void tab                (FILE *const stream, int n);

void Tree_dump          (Tree_node *root);
void Tree_dump_dfs      (Tree_node *node, int *const node_number, FILE *const stream);
void Tree_node_describe (Tree_node *node, int *const node_number, FILE *const stream);

/*_______________________________________________________________________________________________*/

#define exit(code)                                                                                              \
        {                                                                                                       \
            save_data(ROOT);                                                                                    \
            exit     (code);                                                                                    \
        }

#endif //TREE_BINARY_H