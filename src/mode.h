MODE(UNDEFINED  , 0, {break;                    }, "undefined mode"                                 )
MODE(GUESS      , 1, {mode_guess     (ROOT);    }, "guessing game"                                  )
MODE(DOWNLOAD   , 2, {mode_download  (ROOT);    }, "download base from file"                        )
MODE(EXIT       , 3, {save_data      (ROOT);
                      return              0;    }, "stop akinator and exit"                         )
MODE(DUMP       , 4, {Tree_dump      (ROOT);    }, "dump current tree in log-file"                  )
MODE(DEFINITION , 5, {mode_definition(ROOT);    }, "show definition of concept from the base"       )
MODE(COMPARE    , 6, {mode_compare   (ROOT);    }, "show comparing between two concepts from base"  )
MODE(HELP       , 7, {mode_help      ();        }, "show this manual and exit"                      )