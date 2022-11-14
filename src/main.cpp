#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.hpp>

#include "../lib/algorithm/algorithm.h"

#include "akinator.h"


/*______________________FUNCTION_DECLARATION______________________*/

MODE get_mode ();
void mode_help();

/*________________________________________________________________*/

const int HEIGHT = 247;
const int WIDTH  = 215;

sf::RenderWindow TheWnd(sf::VideoMode(215, 247), "AKINATOR");

/*________________________________________________________________*/

int main()
{
    draw_akinator("banal");
    
    Tree_node *ROOT = (Tree_node *) calloc(1, sizeof(Tree_node));
   *ROOT = node_default;

    fprintf_with_voice(stderr, "Привет, я Акинатор!\n");
    fprintf           (stderr, "Выбери режим игры.\n"
                               "Введи \"help\", чтобы получить инструкцию.\n");

    while (true)
    {
        check_event();

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
    check_event();
    
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
