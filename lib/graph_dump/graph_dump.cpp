#include <stdio.h>
#include <stdlib.h>

#include "../logs/log.h"

static const int   graph_size_file =  40;
static const int   graph_size_cmd  = 100;

void dump_graph_main(const  void  *var,

                            void (*dump_simple)     (const void *,  const char *,
                                                                    const char *,
                                                                    const int   ),
                            void (*dump_graph_make) (const void *, FILE *const  ),

                     const  char *call_file,
                     const  char *call_func,
                     const  int   call_line)
{
    log_assert(call_file);
    log_assert(call_func);
    log_assert(dump_graph_make);
    
    if (dump_simple) (*dump_simple)(var, call_file, call_func, call_line);

    static int cur_dump = 1;

    char    output_file[graph_size_file] = "";
    sprintf(output_file, "dump_txt/graph_dump%d.txt", cur_dump);

    FILE *stream =  fopen(output_file, "w");
    if   (stream == nullptr)
    {
        log_error("Can't open grah_dump output file\n");
        return;
    }

    setvbuf(stream, nullptr, _IONBF, 0);
    fprintf(stream, "digraph {\n"
                    "rankdir=LR\n"
                    "node[shape=record, style=\"rounded, filled\", fontsize=8]\n");

    (*dump_graph_make)(var, stream);

    fprintf(stream, "}\n");

    char cmd[graph_size_cmd] = "";
    sprintf     (cmd, "dot %s -T png -o dump_png/List_graph_dump%d.png", output_file, cur_dump);
    system      (cmd);
    log_message ("<img src=dump_png/List_graph_dump%d.png>\n", cur_dump);

    fclose(stream);
    ++cur_dump;
}