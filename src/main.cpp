#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/algorithm/algorithm.h"

#include "tree_binary.h"

Tree_node *ROOT = nullptr;

/*______________________FUNCTION_DECLARATION______________________*/

MODE get_mode();

/*________________________________________________________________*/

int main()
{
    ROOT = (Tree_node *) calloc(1, sizeof(Tree_node));
   *ROOT = node_default;

    fprintf(stderr, "Hello, I am akinator!\n"
                    "Chose the mode of the game\n"
                    "Print \"help\" to get manual\n");

    while (true)
    {
        MODE mode = get_mode();

        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        #define MODE(name, num, code)   \
                case MODE_##name:       \
                    code                \
                    break;              \
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        switch (mode)
        {
            #include "mode.h"
        }
    }
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define MODE(name, ...)                     \
        if  (!strcasecmp(#name, mode))      \
        {                                   \
            clear_input_stream(stdin);      \
            return MODE_##name;             \
        }                                   \
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

MODE get_mode()
{
    char mode[SIZE_MODE] = "";

    scanf("%s", mode);

    if (!is_empty_input_stream(stdin))
    {
        clear_input_stream(stdin);

        fprintf(stderr, "Undefined mode. Print \"help\" to get manual.\n");
        return MODE_UNDEFINED;
    }

    #include "mode.h"

    fprintf(stderr, "Undefined mode. Print \"help\" to get manual.\n");
    return MODE_UNDEFINED;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#undef MODE
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
