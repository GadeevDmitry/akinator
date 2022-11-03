#ifndef GRAPH_DUMP_H
#define GRAPH_DUMP_H

static const char *grphviz_color_names[]   =
{
    "lightblue"     ,
    "lightred"      ,
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
    LIGHT_RED       ,
    LIGHT_GREEN     ,
    LIGHT_GREY      ,

    DARK_BLUE       ,
    DARK_RED        ,
    DARK_GREEN      ,

    WHITE           ,
    BLACK
};

/*____________________________FUNCTION_DECLARATION____________________________*/

void dump_graph_main(const  void  *var,

                            void (*dump_simple)     (const void *,  const char *,
                                                                    const char *,
                                                                    const int   ),
                            void (*dump_graph_make) (const void *, FILE *const  ),

                     const  char *call_file,
                     const  char *call_func,
                     const  char *call_line);
                     
/*____________________________________________________________________________*/

#endif //GRAPH_DUMP_H