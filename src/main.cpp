#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/algorithm/algorithm.h"

#include "akinator.h"


/*______________________FUNCTION_DECLARATION______________________*/

MODE get_mode ();
void mode_help();

/*________________________________________________________________*/

int main()
{
    Tree_node *ROOT = (Tree_node *) calloc(1, sizeof(Tree_node));
   *ROOT = node_default;

    fprintf_with_voice(stderr, "Привет, я Акинатор!\n"
                               "Выбери режим игры.\n");
    fprintf           (stderr, "Введи \"help\", чтобы получить инструкцию.\n");

    while (true)
    {
        MODE mode = get_mode();

        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        #define MODE(name, num, code, ...)  \
                case MODE_##name:           \
                    code                    \
                    break;
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        switch (mode)
        {
            #include "mode.h"
        }

        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        #undef MODE
        //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define MODE(name, ...)                     \
        if  (!strcasecmp(#name, mode))      \
        {                                   \
            clear_input_stream(stdin);      \
            return MODE_##name;             \
        }
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

MODE get_mode()
{
    char mode[SIZE_MODE] = "";

    get_word(mode ,SIZE_MODE, stdin);
    if (!is_empty_input_stream(stdin))
    {
        clear_input_stream(stdin);

        fprintf_with_voice(stderr, "Неопределённый режим. Введи \"help\", чтобы получить инструкцию.\n");
        return MODE_UNDEFINED;
    }

    #include "mode.h"

    fprintf_with_voice(stderr, "Неопределённый режим. Введи \"help\", чтобы получить инструкцию.\n");
    return MODE_UNDEFINED;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#undef  MODE
#define MODE(name, num, code, description)  \
        fprintf(stderr, "%-15s - %s\n", #name, description);
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void mode_help()
{
    #include "mode.h"
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#undef MODE
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
