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
            fprintf_with_voice(stderr, "Сообщение слишком длинное. ");                                                      \
            fprintf           (stderr, "Введи не более %d символов.\n", SIZE_DATA);                                         \
            continue;                                                                                                       \
        }

const int SIZE_ANS = 20;

struct trip
{
    char *node_data;
    bool        yes;
};

/*_______________________________________________________________________________________________*/

void ctor_tree_node(Tree_node *node, Tree_node *prev, const char *node_data, bool from_disk)
{
    assert(node);

   *node              = node_default;
    node->prev        = prev;
    node->from_disk   = from_disk;

    if    (node_data == nullptr) return;

    int  cnt = 0;
    for (cnt = 0; cnt < SIZE_DATA - 1 && node_data[cnt]; ++cnt) node->data[cnt] = node_data[cnt];

    node->data[cnt] = '\0';
}

Tree_node *new_tree_node(Tree_node *prev, const char *node_data, bool from_disk)
{
    Tree_node *new_node = (Tree_node *) calloc(sizeof(Tree_node), 1);
    if        (new_node == nullptr) return nullptr;

    ctor_tree_node(new_node, prev, node_data, from_disk);
    return          new_node;
}

/*_______________________________________________________________________________________________*/

void mode_guess(Tree_node *node, stack * postponed_stk)
{
    assert(node);

    if (postponed_stk == nullptr) postponed_stk = new_stack(sizeof(Tree_node *));

    assert(postponed_stk);

    if (node->left == nullptr && node->right == nullptr)
    {
        if (print_guess_quation(node) == NO)
        {
            if (!get_postponed_node_from_stack(postponed_stk, &node))
            {
                get_new_terminal_node(node);

                free              (postponed_stk);
                fprintf_with_voice(stderr, "Конец игры. Выбери режим.\n");
                return;
            }
        }
        else
        {
            print_guess_answer(nullptr, nullptr, nullptr, YES);
            
            free              (postponed_stk);
            fprintf_with_voice(stderr, "Конец игры. Выбери режим.\n");
            return;
        }
    }

    get_next_node(&node, postponed_stk);
    mode_guess   ( node, postponed_stk);
}

void get_next_node(Tree_node **node, stack *const postponed_stk)
{
    assert( node        );
    assert(*node        );
    assert(postponed_stk);

    switch(print_quation(*node))
    {
        case YES:
            *node = (*node)->left;
            break;
        
        case NO:
            *node = (*node)->right;
            break;
        
        case MAYBE_YES:
            stack_push(postponed_stk, &(*node)->right);
            *node = (*node)->left;
            break;

        case MAYBE_NO:
            stack_push(postponed_stk, &(*node)->left);
            *node = (*node)->right;
            break;
        
        default:
            assert(0 && "default in this switch must not be");
            break;
    }
}

void get_new_terminal_node(Tree_node *node)
{
    assert(node);

    char *creature   = (char *) calloc(sizeof(char), SIZE_DATA);
    char *difference = (char *) calloc(sizeof(char), SIZE_DATA);

    print_guess_answer(node, difference, creature, NO);

    node->left  = (Tree_node *) new_tree_node(node, creature  , false          );
    node->right = (Tree_node *) new_tree_node(node, node->data, node->from_disk);

    node->from_disk = false;
    memcpy(node->data, difference, SIZE_DATA);

    free(creature  );
    free(difference);
}

ANSWERS print_guess_quation(const Tree_node *const node)
{
    assert(node);

    fprintf_with_voice(stderr, "Ты загадал \"%s\"?\n", node->data);
    return yes_no     (true);
}

bool print_guess_answer(const Tree_node *const node, char *const difference, char *const creature, ANSWERS ans)
{

    if (ans == YES)
    {
        fprintf_with_voice(stderr, "Это было проще простого)\n");
        return true;
    }

    assert(node      );
    assert(difference);
    assert(creature  );

    while (true)
    {
        fprintf_with_voice(stderr, "Кого ты загадал?\n");    
        get_line_stream(creature, SIZE_DATA, stdin)

        fprintf_with_voice(stderr, "В чём разница между \"%s\" и \"%s\"?\n", creature, node->data);
        get_line_stream(difference, SIZE_DATA, stdin)

        return false;
    }

    return false;
}

ANSWERS print_quation(const Tree_node *const node)
{
    assert(node);

    fprintf_with_voice(stderr, "Твой персонаж %s?\n", node->data);
    return yes_no();
}

/*_______________________________________________________________________________________________*/

bool get_postponed_node_from_stack(stack *const postponed_stack, Tree_node **const push_in)
{
    assert(postponed_stack);
    assert(push_in        );

    if (stack_empty(postponed_stack)) return false;

    memcpy   (push_in, stack_front(postponed_stack), sizeof(Tree_node *));
    stack_pop(postponed_stack);
    
    return true;
}

/*_______________________________________________________________________________________________*/

void mode_download(Tree_node *const ROOT)
{
    assert(ROOT);

    fprintf_with_voice(stderr, "Введи путь до файла, откуда взять базу.\n");

    char filename[SIZE_DATA] = "";

    while (true)
    {
        get_word(filename, SIZE_DATA, stdin);
        if (!is_empty_input_stream(stdin))
        {
            clear_input_stream(stdin);

            fprintf_with_voice(stderr, "Некорректный путь. ");
            fprintf           (stderr, "Введи одно слово из не более,чем %d симаолов.\n", SIZE_DATA);
            continue;
        }

        if (read_input_base(ROOT, filename))
        {
            fprintf_with_voice(stderr, "Парсинг произошёл успешно. Выбери режим.\n");
            return;
        }
    }
}

#define wrong_file_fmt()                                                                        \
        {                                                                                       \
        fprintf_with_voice(stderr, "Неправильный формат ");                                     \
        fprintf           (stderr, "\"%s\". Введи другое имя.\n", filename);                    \
        free              (data_base);                                                          \
        return false;                                                                           \
        }

bool read_input_base(Tree_node *const ROOT, const char *filename)
{
    assert(ROOT    );
    assert(filename);

    ROOT->from_disk = true;
    Tree_node *node = nullptr;

    size_t data_size = 0;
    size_t data_pos  = 0;
    char  *data_base =  (char *) read_file(filename, &data_size);
    if    (data_base == nullptr)
    {
        fprintf_with_voice(stderr, "Не могу открыть файл. ");
        fprintf           (stderr, "Введи другое имя.\n");
        return false;
    }

    skip_spaces(data_base, data_size, &data_pos);

    while (data_base[data_pos] != '\0')
    {
        char c = data_base[data_pos++];

        if  (c == '}')
        {
            if (node == nullptr) wrong_file_fmt()
            if (node->left != nullptr && node->right == nullptr ||
                node->left == nullptr && node->right != nullptr)    wrong_file_fmt()

            node = node->prev;

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
                    ctor_tree_node(node->left , node, nullptr, true);
                    node = node->left;
                }
                else if (node->right == nullptr)
                {
                    node->right = (Tree_node *) calloc(1, sizeof(Tree_node));
                    ctor_tree_node(node->right, node, nullptr, true);
                    node = node->right;
                }
                else wrong_file_fmt()
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

bool get_node_data(char *push_in, const int max_size, const char *buff, const int buff_size, size_t *const pos)
{
    assert(push_in);
    assert(buff   );
    assert(pos    );

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

        push_in[cnt] = buff[*pos];
       *pos          = *pos + 1;
    }

    push_in[cnt] = '\0';
    return ans;
}

/*_______________________________________________________________________________________________*/


void mode_definition(Tree_node *const ROOT)
{
    assert(ROOT);

    fprintf_with_voice(stderr, "Назови имя того, чьё определение тебя интересует.\n");

    char term[SIZE_DATA] = "";
    
    while (true)
    {
        get_line_stream(term, SIZE_DATA, stdin)

        stack       tree_way;
        stack_ctor(&tree_way, sizeof(trip));

        if (Tree_definition_dfs(ROOT, term, &tree_way))
        {
            print_definition  (&tree_way, term);
            fprintf_with_voice(stderr, "Выбери режим.\n");

            stack_dtor(&tree_way);
            return;
        }
        else
        {
            fprintf_with_voice(stderr, "Этого термина нет в базе. ");
            fprintf           (stderr, "Назови другое имя.\n");
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

        if (cur.yes) fprintf(stderr, "%s"    , cur.node_data);
        else         fprintf(stderr, "не %s", cur.node_data);

        if (cnt != size - 1) fprintf(stderr, ", ");
        else                 fprintf(stderr, ".\n");
    }
}

/*_______________________________________________________________________________________________*/

void mode_compare(Tree_node *const ROOT)
{
    assert(ROOT);

    fprintf_with_voice(stderr, "Назови два имени, а я сравню их определения.\n");

    char term1[SIZE_DATA] = "";
    char term2[SIZE_DATA] = "";

    while (true)
    {
        get_line_stream(term1, SIZE_DATA, stdin)
        get_line_stream(term2, SIZE_DATA, stdin)

        stack       tree_way1 =           {};
        stack       tree_way2 =           {};
        stack_ctor(&tree_way1, sizeof(trip));
        stack_ctor(&tree_way2, sizeof(trip));

        if (!Tree_definition_dfs(ROOT, term1, &tree_way1))
        {
            fprintf_with_voice(stderr, "Первого термина нет в базе. ");
            fprintf           (stderr, "Введи другое имя.\n");
            continue;
        }
        if (!Tree_definition_dfs(ROOT, term2, &tree_way2))
        {
            fprintf_with_voice(stderr, "Второго термина нет в базе. ");
            fprintf           (stderr, "Введи другое имя.\n");
            continue;
        }

        print_compare     (&tree_way1, term1, &tree_way2, term2);
        fprintf_with_voice(stderr, "Выбери режим.\n");

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

    trip *beg1 = (trip *) tree_way1->data;
    trip *beg2 = (trip *) tree_way2->data;

    for (; cnt1 < size1 && cnt2 < size2; ++cnt1, ++cnt2)
    {
        trip cur1 = beg1[cnt1];
        trip cur2 = beg2[cnt2];

        if (!strcasecmp(cur1.node_data, cur2.node_data) && cur1.yes == cur2.yes)
        {
            if (!same)
            {
                same = true;
                fprintf(stderr, "\"%s\" и \"%s\" оба ", term1, term2);
            }
            else fprintf(stderr, ", ");

            if (cur1.yes) fprintf(stderr, "%s"    , cur1.node_data);
            else          fprintf(stderr, "не %s", cur2.node_data);
        }
        else break;
    }

    if (cnt1 < size1)
    {
        fprintf(stderr, ", но \"%s\" также ", term1);
        print_difference  (tree_way1, cnt1, size1);

        if (tree_way2->size)
        {
            fprintf(stderr, ", a \"%s\" ещё и ", term2);
            print_difference  (tree_way2, cnt2, size2);
        }
    }
    else if (cnt2 < size2)
    {
        fprintf(stderr, ", но \"%s\" также ", term2);
        print_difference  (tree_way2, cnt2, size2);
    }
    fprintf(stderr, ".\n");
}

void print_difference(stack *const tree_way, int cnt, const int size)
{
    assert(tree_way);

    trip *beg = (trip *) tree_way->data;

    for (; cnt < size; ++cnt)
    {
        trip cur = beg[cnt];

        if (cur.yes) fprintf(stderr, "%s"    , cur.node_data);
        else         fprintf(stderr, "не %s", cur.node_data);

        if (cnt != size - 1) fprintf(stderr, ", ");
    }
}

/*_______________________________________________________________________________________________*/

void save_data(Tree_node *node)
{
    fprintf_with_voice(stderr, "Введи путь до файла, в который сохранить базу");
    fprintf           (stderr, "или введи \"-\", чтобы не сохранять её).\n");
    
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

            fprintf_with_voice(stderr, "Некорректное имя файла. ");
            fprintf           (stderr, "Введи другое имя.\n");
            continue;
        }
        if (!strcmp("-", filename)) return;

        stream = fopen(filename, "w");

        if (stream == nullptr)
        {
            fprintf_with_voice(stderr, "Не могу открыть этот файл(. Введи другое имя.\n");
            continue;
        }

        fill_output_file  (node, stream, 0);
        fclose            (      stream   );
        fprintf_with_voice(stderr, "Запись произошла успешно. Пока!\n");
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

ANSWERS yes_no(bool strict)
{
    char answer[SIZE_ANS] = "";

    while (true)
    {
        get_word(answer, SIZE_ANS, stdin);

        if (!is_empty_input_stream(stdin))
        {
            clear_input_stream(stdin);

            fprintf_with_voice(stderr, "Неопределённый ответ. ");

            if (!strict) fprintf(stderr, "Скажи одно из: \"да\", \"нет\", \"скорее_да\", \"скорее_нет\".\n");
            else         fprintf(stderr, "Скажи \"да\" или \"нет\".\n");

            continue;
        }
        if (!strcasecmp("да", answer)) return YES;
        if (!strcasecmp("нет", answer)) return NO;

        if (!strict && !strcasecmp("скорее_да", answer)) return MAYBE_YES;
        if (!strict && !strcasecmp("скорее_нет", answer)) return MAYBE_NO;

        fprintf_with_voice(stderr, "Неопределённый ответ. ");

        if (!strict) fprintf(stderr, "Скажи одно из: \"да\", \"нет\", \"скорее_да\", \"скорее_нет\".\n");
        else         fprintf(stderr, "Скажи \"да\" или \"нет\".\n");
    }

    return YES;
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

    for (int cnt = 0; cnt < SIZE_DATA && cmd_text[cnt]; ++cnt)
    {
        if (cmd_text[cnt] == '\n' ||
            cmd_text[cnt] == '('  ||
            cmd_text[cnt] == ')'  ||
            cmd_text[cnt] == '-'  ||
            cmd_text[cnt] == ','  ||
            cmd_text[cnt] == '.')   cmd_text[cnt] = ' ';
    }

    sprintf(cmd,    "echo \"%s\" | festival --tts", cmd_text);
    //fprintf(stderr, "cmd - %s\n", cmd);

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
        fprintf_with_voice(stderr, "Не могу открыть текстовый дамп-файл.\n");
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

    fprintf_with_voice(stderr, "Дампинг произошёл успешно. Выбери режим.\n");
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

    fprintf(stream, "node%d->node%d[xlabel=\"Да \", color=\"black\"]\n", number_cur, number_left);
    fprintf(stream, "node%d->node%d[xlabel=\"Нет\", color=\"black\"]\n", number_cur, number_right);
}

void Tree_node_describe(Tree_node *node, int *const node_number, FILE *const stream)
{
    assert(node);
    assert(stream);

    GRAPHVIZ_COLOR fillcolor = WHITE;
    GRAPHVIZ_COLOR     color = WHITE;

    if (node->left == nullptr && node->from_disk)
    {
        fillcolor = LIGHT_BLUE;
            color =  DARK_BLUE;
    }
    else if (node->left != nullptr && node->from_disk)
    {
        fillcolor = LIGHT_GREEN;
            color =  DARK_GREEN;
    }
    else if (node->left == nullptr && !node->from_disk)
    {
        fillcolor = LIGHT_ORANGE;
            color =  DARK_ORANGE;
    }
    else
    {
        fillcolor = LIGHT_PINK;
            color =  DARK_PINK;
    }

    fprintf(stream, "node%d[color=\"%s\", fillcolor=\"%s\", label=\"{cur=%p \\n | from_disk=%d \\n | prev = %p\\n | :%s:\\n | {left=%p | right=%p}}\"]\n",
                    *node_number,
                                    graphviz_color_names[color],
                                                      graphviz_color_names[fillcolor],
                                                                         node,
                                                                                            node->from_disk,
                                                                                                            node->prev,
                                                                                                                     node->data,
                                                                                                                                    node->left,
                                                                                                                                               node->right);
    ++*node_number;
}