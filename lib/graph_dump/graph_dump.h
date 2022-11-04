#ifndef GRAPH_DUMP_H
#define GRAPH_DUMP_H

static const int   graph_size_file = 100;
static const int   graph_size_cmd  = 300;

static const char *graphviz_color_names[]   =
{
    "lightblue"     ,
    "lightgreen"    ,
    "lightgrey"     ,

    "darkblue"      ,
    "darkred"       ,
    "darkgreen"     ,

    "white"         ,
    "black"
};

enum GRAPHVIZ_COLOR
{
    LIGHT_BLUE      ,
    LIGHT_GREEN     ,
    LIGHT_GREY      ,

    DARK_BLUE       ,
    DARK_RED        ,
    DARK_GREEN      ,

    WHITE           ,
    BLACK
};

#endif //GRAPH_DUMP_H