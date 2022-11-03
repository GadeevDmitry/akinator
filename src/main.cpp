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
        if (!strcasecmp(#name, mode))       \
        {                                   \
            return MODE_##name;             \
        }                                   \
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

MODE get_mode()
{
    char mode[SIZE_MODE] = "";

    scanf("%9s", mode);

    if (!is_empty_input_buff(stdin))
    {
        clear_input_buff(stdin);

        fprintf(stderr, "Undefined mode. Use \"help\" to get manual\n");
        return MODE_UNDEFINED;
    }

    #include "mode.h"

    fprintf(stderr, "Undefined mode. Use \"help\" to get manual\n");
    return MODE_UNDEFINED;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#undef MODE
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
