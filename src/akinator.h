#ifndef TREE_BINARY_H
#define TREE_BINARY_H

#include "../lib/stack/stack.h"

const int SIZE_DATA =  500;
const int SIZE_MODE =   50;
const int SIZE_CMD  = 1000;

extern sf::RenderWindow TheWnd;
extern const int     HEIGHT;
extern const int      WIDTH;

struct Tree_node
{
    char data[SIZE_DATA];

    bool from_disk;

    Tree_node *prev;
    Tree_node *left;
    Tree_node *right;
};

const Tree_node node_default =
{
    "кто-то неизвестный", // data

    true                , // from_disk
    
    nullptr             , // prev
    nullptr             , // left
    nullptr               // right
};

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define MODE(name, num, ...)        \
        MODE_##name = num,
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

enum MODE
{
    #include "mode.h"
};

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#undef MODE
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

enum ANSWERS
{
    YES         ,
    NO          ,
    MAYBE_YES   ,
    MAYBE_NO    ,
};

/*_____________________________________FUNCTION_DECLARATIONS_____________________________________*/

void        ctor_tree_node              (Tree_node *node, Tree_node *prev, const char *node_data, bool from_disk);
Tree_node  *new_tree_node               (                 Tree_node *prev, const char *node_data, bool from_disk);

void        mode_guess                  (Tree_node *node,  stack *const postponed_stk);
void        get_next_node               (Tree_node **node, stack *const postponed_stk);
void        get_new_terminal_node       (Tree_node *node);
ANSWERS     print_guess_quation         (const Tree_node *const node);
bool        print_guess_answer          (const Tree_node *const node, char *const difference, char *const creature, ANSWERS ans);
ANSWERS     print_quation               (const Tree_node *const node);

bool        get_postponed_node_from_stack  (stack *const postponed_stack, Tree_node **const push_in);

void        save_data                   (Tree_node *node);
void        fill_output_file            (Tree_node *node, FILE *const stream, int tab_shift);

void        mode_download               (Tree_node *const ROOT);
bool        read_input_base             (Tree_node *const ROOT, const char *filename);
bool        update_node                 (Tree_node *node,                   const char *buff, const int buff_size, size_t *const pos);
bool        get_node_data               (char *push_in, const int max_size, const char *buff, const int buff_size, size_t *const pos);

void        mode_definition             (Tree_node *const ROOT);
bool        Tree_definition_dfs         (Tree_node *node, const char *term_to_find, stack *const tree_way);
void        print_definition            (stack *const tree_way, const char *term);

void        mode_compare                (Tree_node *const ROOT);
void        print_compare               (stack *const tree_way1, const char *term1, stack *const tree_way2, const char *term2);
void        print_difference            (stack *const tree_way, int cnt, const int size);

ANSWERS     yes_no                      (bool strict = false);
void        tab                         (FILE *const stream, int n);
void        fprintf_with_voice          (FILE *const stream, const char *fmt, ...);
void        voice                       (const char *text);

void        Tree_dump                   (Tree_node *root);
void        Tree_dump_dfs               (Tree_node *node, int *const node_number, FILE *const stream);
void        Tree_node_describe          (Tree_node *node, int *const node_number, FILE *const stream);

void        draw_akinator               (const char *emotion);
void        check_event                 ();

/*_______________________________________________________________________________________________*/

#endif //TREE_BINARY_H