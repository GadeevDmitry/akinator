#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "../lib/logs/log.h"
#include "../lib/graph_dump/graph_dump.h"
#include "../lib/algorithm/algorithm.h"
#include "../lib/read_write/read_write.h"
#include "../lib/stack/stack.h"

#include "akinator.h"

#define get_line_stream(s, max_size, stream)                                                                                \
        get_line       (s, max_size, stream);                                                                               \
                                                                                                                            \
        if (!is_empty_input_stream(stream))                                                                                 \
        {                                                                                                                   \
            clear_input_stream(stream);                                                                                     \
                                                                                                                            \
            fprintf_with_voice(stderr, "You message is so long. Please print not more than %d characters.\n", SIZE_DATA);   \
            continue;                                                                                                       \
        }

const int SIZE_ANS = 20;
extern Tree_node  *ROOT;

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

            ctor_tree_node(node->right, node, node->data);
            ctor_tree_node(node->left , node, creature  );

            memcpy(node->data, difference, SIZE_DATA);
        }

        free(creature);
        free(difference);

        fprintf_with_voice(stderr, "Game over. Choose the mode.\n");

        return;
    }
    if (print_quation(node)) node = node->left;
    else                     node = node->right;

    mode_guess(node);
}

bool print_guess(const Tree_node *const node, char *const difference, char *const creature)
{
    assert(node      );
    assert(difference);
    assert(creature  );

    fprintf_with_voice(stderr, "Is \"%s\" your guess?\n", node->data);

    bool ans = yes_no();

    if  (ans)
    {
        fprintf_with_voice(stderr, "It was very easy)\n");
        return true;
    }

    while (true)
    {
        fprintf_with_voice(stderr, "Who have you guessed?\n");    

        get_line_stream(creature, SIZE_DATA, stdin)
        if (!strcasecmp(creature, "exit")) exit(0)

        fprintf_with_voice(stderr, "What the difference between \"%s\" and \"%s\"?\n", creature, node->data);

        get_line_stream(difference, SIZE_DATA, stdin)
        if (!strcasecmp(difference, "exit")) exit(0)

        return false;
    }

    return false;
}

bool print_quation(const Tree_node *const node)
{
    assert(node);

    fprintf_with_voice(stderr, "Is your character %s?\n", node->data);
    return yes_no();
}

/*_______________________________________________________________________________________________*/

void mode_download()
{
    fprintf_with_voice(stderr, "Tell me the name of file to take the data base from.\n");

    char filename[SIZE_DATA] = "";

    while (true)
    {
        get_word(filename, SIZE_DATA, stdin);
        if (!is_empty_input_stream(stdin))
        {
            clear_input_stream(stdin);

            fprintf_with_voice(stderr, "You message is incorrect. Please print one word with not more than %d characters.\n", SIZE_DATA);
            continue;
        }
        if (!strcasecmp(filename, "exit")) exit(0)

        if (read_input_base(filename))
        {
            fprintf_with_voice(stderr, "Parsing was successful. Choose the mode.\n");
            return;
        }
    }
}

#define wrong_file_fmt()                                                                        \
        {                                                                                       \
        fprintf_with_voice(stderr, "Wrong format of \"%s\". Tell me another name.\n", filename);\
        free   (data_base);                                                                     \
        return false;                                                                           \
        }

bool read_input_base(const char *filename)
{
    assert(filename);

    Tree_node *node = nullptr;

    size_t data_size = 0;
    size_t data_pos  = 0;
    char  *data_base =  (char *) read_file(filename, &data_size);
    if    (data_base == nullptr)
    {
        fprintf_with_voice(stderr, "I can't open this file(. Tell me another name\n");
        return false;
    }

    skip_spaces(data_base, data_size, &data_pos);

    while (data_base[data_pos] != '\0')
    {
        char c = data_base[data_pos++];

        if  (c == '}')
        {
            if (node == nullptr) wrong_file_fmt()

            node->visit_left  = false;
            node->visit_right = false;
            node              = node->prev;

            skip_spaces(data_base, data_size, &data_pos);
            continue;
        }
        else if  (c == '{')
        {
            if (node == nullptr) node = ROOT;
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
                    if (node->visit_right)  wrong_file_fmt()

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
        else wrong_file_fmt()

        if (!update_node(node, data_base, data_size, &data_pos)) wrong_file_fmt()

        skip_spaces(data_base, data_size, &data_pos);
    }

    if (node == nullptr)
    {
        free(data_base);
        return true;
    }
    wrong_file_fmt()
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

/*_______________________________________________________________________________________________*/

struct trip
{
    char *node_data;
    bool        yes;
};

void mode_definition()
{
    fprintf_with_voice(stderr, "Tell me the name of something you are interested in defining.\n");

    char term[SIZE_DATA] = "";
    
    while (true)
    {
        get_line_stream(term, SIZE_DATA, stdin)
        if (!strcasecmp(term, "exit" )) exit(0)

        stack       tree_way;
        stack_ctor(&tree_way, sizeof(trip));

        if (Tree_definition_dfs(ROOT, term, &tree_way))
        {
            print_definition  (&tree_way, term);
            fprintf_with_voice(stderr, "Choose the mode.\n");

            stack_dtor(&tree_way);
            return;
        }
        else
        {
            fprintf_with_voice(stderr, "Can't find this term in my base. Tell me another name.\n");
            continue;
        }
    }
}

bool Tree_definition_dfs(Tree_node *node, const char *term_to_find, stack *const tree_way)
{
    assert(node        );
    assert(term_to_find);

    if (node->left == nullptr)
    {
        if (!strcasecmp(node->data, term_to_find)) return true;
        return false;
    }

    trip cur_trip = {node->data, true};
    stack_push      (tree_way, &cur_trip);

    if (Tree_definition_dfs(node->left , term_to_find, tree_way) == true) return true;

    cur_trip = {node->data, false};
    stack_pop (tree_way);
    stack_push(tree_way, &cur_trip);

    if (Tree_definition_dfs(node->right, term_to_find, tree_way) == true) return true;
    
    stack_pop(tree_way);
    return false;
}

void print_definition(stack *const tree_way, const char *term)
{
    assert(tree_way);
    assert(term    );

    fprintf(stderr, "%s - ", term);

    int size = tree_way->size;

    for (int cnt = 0; cnt < size; ++cnt)
    {
        trip cur = *(trip *) ((char *) tree_way->data + sizeof(trip) * cnt);

        if (cur.yes) fprintf_with_voice(stderr, "%s"    , cur.node_data);
        else         fprintf_with_voice(stderr, "not %s", cur.node_data);

        if (cnt != size - 1) fprintf(stderr, ", ");
        else                 fprintf(stderr, ".\n");
    }
}

/*_______________________________________________________________________________________________*/

void mode_compare()
{
    fprintf_with_voice(stderr, "Tell me two things you want to compare.\n");

    char term1[SIZE_DATA] = "";
    char term2[SIZE_DATA] = "";

    while (true)
    {
        get_line_stream(term1, SIZE_DATA, stdin)
        if (!strcasecmp(term1, "exit"))  exit(0)
        get_line_stream(term2, SIZE_DATA, stdin)
        if (!strcasecmp(term2, "exit"))  exit(0)

        stack       tree_way1;
        stack       tree_way2;
        stack_ctor(&tree_way1, sizeof(trip));
        stack_ctor(&tree_way2, sizeof(trip));

        if (!Tree_definition_dfs(ROOT, term1, &tree_way1))
        {
            fprintf_with_voice(stderr, "Can't find first term in my base. Try another name.\n");
            continue;
        }
        if (!Tree_definition_dfs(ROOT, term2, &tree_way2))
        {
            fprintf_with_voice(stderr, "Can't find second term in my base. Try another name.\n");
            continue;
        }

        print_compare     (&tree_way1, term1, &tree_way2, term2);
        fprintf_with_voice(stderr, "Choose the mode.\n");

        stack_dtor(&tree_way1);
        stack_dtor(&tree_way2);
        return;
    }
}

void print_compare(stack *const tree_way1, const char *term1, stack *const tree_way2, const char *term2)
{
    assert(tree_way1);
    assert(term1    );
    assert(tree_way2);
    assert(term2    );

    bool same = false;

    int  cnt1 = 0;
    int  cnt2 = 0;
    int size1 = tree_way1->size;
    int size2 = tree_way2->size;

    for (; cnt1 < size1 && cnt2 < size2; ++cnt1, ++cnt2)
    {
        trip cur1 = *(trip *) ((char *) tree_way1->data + cnt2 * sizeof(trip));
        trip cur2 = *(trip *) ((char *) tree_way2->data + cnt1 * sizeof(trip));

        if (!strcasecmp(cur1.node_data, cur2.node_data) && cur1.yes == cur2.yes)
        {
            if (!same)
            {
                same = true;
                fprintf_with_voice(stderr, "The %s and %s are both ", term1, term2);
            }
            else fprintf(stderr, ", ");

            if (cur1.yes) fprintf_with_voice(stderr, "%s"    , cur1.node_data);
            else          fprintf_with_voice(stderr, "not %s", cur2.node_data);
        }
        else break;
    }

    if (cnt1 < size1)
    {
        fprintf_with_voice(stderr, ", but %s also ", term1);
        print_difference  (tree_way1, cnt1, size1);

        if (tree_way2->size)
        {
            fprintf_with_voice(stderr, ", and %s also ", term2);
            print_difference  (tree_way2, cnt2, size2);
        }
    }
    else if (cnt2 < size2)
    {
        fprintf_with_voice(stderr, ", but %s also ", term2);
        print_difference  (tree_way2, cnt2, size2);
    }
    fprintf(stderr, ".\n");
}

void print_difference(stack *const tree_way, int cnt, const int size)
{
    assert(tree_way);

    for (; cnt < size; ++cnt)
    {
        trip cur = *(trip *) ((char *) tree_way->data + cnt * sizeof(trip));

        if (cur.yes) fprintf_with_voice(stderr, "%s"    , cur.node_data);
        else         fprintf_with_voice(stderr, "not %s", cur.node_data);

        if (cnt != size - 1) fprintf(stderr, ", ");
    }
}

/*_______________________________________________________________________________________________*/

void save_data(Tree_node *node)
{
    fprintf_with_voice(stderr, "Tell me the name of file to save the data base in (or print \"-\" if you don't want to save it), before I stop the program.\n");
    
    //>>>>>>>>>>>>>>>>
    //fprintf(stderr, "return to save_data()\n");
    //<<<<<<<<<<<<<<<<

    char filename[SIZE_DATA] =      "";
    FILE  *stream            = nullptr;

    while (true)
    {
        get_word(filename, SIZE_DATA, stdin);
        

        if (!is_empty_input_stream(stdin))
        {
            clear_input_stream(stdin);

            fprintf_with_voice(stderr, "Undefined name of file. Tell me another name\n");
            continue;
        }
        if (!strcmp("-", filename)) return;

        stream = fopen(filename, "w");

        if (stream == nullptr)
        {
            fprintf_with_voice(stderr, "I can't open this file(. Tell me another name\n");
            continue;
        }

        fill_output_file  (node, stream, 0);
        fclose            (      stream   );
        fprintf_with_voice(stderr, "Writing was successful. Bye!\n");
        return;
    }  
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

        tab    (stream, tab_shift);
        fprintf(stream, "}\n");
    }
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

            fprintf_with_voice(stderr, "Undefined answer. Print \"yes\" or \"no\"\n");
            continue;
        }
        if (!strcasecmp("yes" , answer)) return true ;
        if (!strcasecmp("no"  , answer)) return false;
        if (!strcasecmp("exit", answer)) exit(0)

        fprintf_with_voice(stderr, "Undefined answer. Print \"yes\" or \"no\"\n");
    }

    return true;
}

void tab(FILE *const stream, int n)
{
    while (n--) putc('\t', stream);
}

void fprintf_with_voice(FILE *const stream, const char *fmt, ...)
{
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //fprintf(stderr, "I am in fprintf_with_voice()\n");
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    va_list  ap;
    va_start(ap, fmt);

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //fprintf(stderr, "Alreay after va_init\n");
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    char     output [SIZE_DATA] = "";
    vsprintf(output, fmt, ap);

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //fprintf(stderr, "Already after char_init\n");
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


    fprintf(stderr, output); 
    voice  (        output);
    
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    //fprintf(stderr, "return to fprintf_with_voice()\n");
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    
    va_end(ap);
}

void voice(const char *s)
{
    //>>>>>>>>>>>>>>>>>>>>>>
    //fprintf(stderr, "start voice\n");
    //<<<<<<<<<<<<<<<<<<<<<<

    assert(s);

    char    cmd     [SIZE_CMD ] = "";
    char    cmd_text[SIZE_DATA] = "";
    sprintf(cmd_text, "%s", s);

    for (int cnt = 0; cnt < SIZE_DATA; ++cnt)
    {
        if (!isalnum(cmd_text[cnt]) ||
            cmd_text[cnt] == '\n' ) cmd_text[cnt] = ' ';
    }

    //fprintf(stderr, "%s\n", cmd);
    sprintf(cmd, "echo \"%s\" | festival --tts", cmd_text);

    system(cmd);

    //>>>>>>>>>>>>>
    //fprintf(stderr, "make voice\n");
    //<<<<<<<<<<<<<
}

/*_______________________________________________________________________________________________*/

void Tree_dump(Tree_node *root)
{
    assert(root);

    static int cur = 0;

    char    dump_txt[graph_size_file] = "";
    char    dump_png[graph_size_file] = "";

    sprintf(dump_txt, "../dump_txt/Tree%d.txt", cur);
    sprintf(dump_png, "../dump_png/Tree%d.png", cur);

    FILE *stream_txt =  fopen(dump_txt, "w");
    if   (stream_txt == nullptr)
    {
        fprintf(stderr, "Can't open the dump_txt file\n");
        return;
    }

    setvbuf(stream_txt, nullptr, _IONBF, 0);
    fprintf(stream_txt, "digraph {\n"
                        "splines=ortho\n"
                        "node[shape=record, style=\"rounded, filled\", fontsize=8]\n");
    
    int node_number = 0;
    Tree_dump_dfs(root, &node_number, stream_txt);

    fprintf(stream_txt, "}\n");

    char cmd[graph_size_cmd] = "";
    sprintf    (cmd, "dot %s -T png -o %s", dump_txt, dump_png);
    system     (cmd);
    log_message("<img src=%s>\n", dump_png);

    fclose(stream_txt);

    fprintf(stderr, "Tree dumping was successful. Choose the mode.\n");
}

void Tree_dump_dfs(Tree_node *node, int *const node_number, FILE *const stream)
{
    assert(node);
    assert(stream);

    int number_cur =  *node_number;
    Tree_node_describe(node, node_number, stream);

    if (node->left == nullptr) return;

    int number_left = *node_number;
    Tree_dump_dfs(node->left, node_number, stream);

    int number_right = *node_number;
    Tree_dump_dfs(node->right, node_number, stream);

    fprintf(stream, "node%d->node%d[xlabel=\"Yes\", color=\"black\"]\n", number_cur, number_left);
    fprintf(stream, "node%d->node%d[xlabel=\"No \", color=\"black\"]\n", number_cur, number_right);
}

void Tree_node_describe(Tree_node *node, int *const node_number, FILE *const stream)
{
    assert(node);
    assert(stream);

    GRAPHVIZ_COLOR fillcolor = WHITE;
    GRAPHVIZ_COLOR     color = WHITE;

    if (node->left == nullptr)
    {
        fillcolor = LIGHT_BLUE;
            color =  DARK_BLUE;
    }
    else
    {
        fillcolor = LIGHT_GREEN;
            color =  DARK_GREEN;
    }

    fprintf(stream, "node%d[color=\"%s\", fillcolor=\"%s\", label=\"{cur=%p\\n | prev = %p\\n | :%s:\\n | {left=%p | right=%p}}\"]\n",
                    *node_number,
                                    graphviz_color_names[color],
                                                      graphviz_color_names[fillcolor],
                                                                       node,
                                                                                      node->prev,
                                                                                                 node->data,
                                                                                                                node->left,
                                                                                                                           node->right);
    ++*node_number;
}