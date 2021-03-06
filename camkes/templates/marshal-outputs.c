/*# We expect the following variables to be defined when this fragment is
 *# included.
 #*/
/*? assert(isinstance(instance, basestring)) ?*/      /*# Name of this component instance #*/
/*? assert(isinstance(interface, basestring)) ?*/     /*# Name of this interface #*/
/*? assert(isinstance(name, basestring)) ?*/          /*# Name of this method #*/
/*? assert(isinstance(function, basestring)) ?*/      /*# Name of function to create #*/
/*? assert(isinstance(buffer, basestring)) ?*/        /*# Buffer symbol (or expression) to marshal into #*/
/*? assert(isinstance(size, basestring)) ?*/          /*# Length of the buffer; possibly not generation-time constant #*/
/*? assert(isinstance(output_parameters, list)) ?*/   /*# All output parameters to this method #*/
/*? assert(return_type == None or isinstance(return_type, camkes.ast.Type) or isinstance(return_type, camkes.ast.Reference)) ?*/
                                               /*# Return type of this interface #*/
/*? assert(isinstance(error_handler, basestring)) ?*/ /*# Handler to invoke on error #*/

/*- set ret_fn = c_symbol('ret_fn') -*/
/*- if return_type is not none -*/
  /*- set offset = c_symbol('offset') -*/
  /*- set ret = c_symbol('return') -*/
  static unsigned int /*? function ?*/_/*? ret_fn ?*/(unsigned int /*? offset ?*/,
    /*- if isinstance(return_type, camkes.ast.Type) and return_type.type == 'string' -*/
      char ** /*? ret ?*/
    /*- else -*/
      const /*? show(return_type) ?*/ * /*? ret ?*/
    /*- endif -*/
  ) {

    /*- set base = c_symbol('buffer_base') -*/
    void * /*? base ?*/ UNUSED = (void*)(/*? buffer ?*/);

    /* Marshal the return value. */
    /*- if isinstance(return_type, camkes.ast.Type) and return_type.type == 'string' -*/
      /*- set strlen = c_symbol('strlen') -*/
      size_t /*? strlen ?*/ = strnlen(* /*? ret ?*/, /*? size ?*/ - /*? offset ?*/);
      ERR_IF(/*? strlen ?*/ >= /*? size ?*/ - /*? offset ?*/, /*? error_handler ?*/, ((camkes_error_t){
          .type = CE_BUFFER_LENGTH_EXCEEDED,
          .instance = "/*? instance ?*/",
          .interface = "/*? interface ?*/",
          .description = "buffer exceeded while marshalling return value for /*? name ?*/",
          .current_length = /*? offset ?*/,
          .target_length = /*? offset ?*/ + /*? strlen ?*/ + 1,
        }), ({
          return UINT_MAX;
        }));
      /* If we didn't trigger an error, we now know this strcpy is safe. */
      (void)strcpy(/*? base ?*/ + /*? offset ?*/, (* /*? ret ?*/));
      /*? offset ?*/ += /*? strlen ?*/ + 1;
    /*- else -*/
      ERR_IF(/*? offset ?*/ + sizeof(* /*? ret ?*/) > /*? size ?*/, /*? error_handler ?*/, ((camkes_error_t){
          .type = CE_BUFFER_LENGTH_EXCEEDED,
          .instance = "/*? instance ?*/",
          .interface = "/*? interface ?*/",
          .description = "buffer exceeded while marshalling return value for /*? name ?*/",
          .current_length = /*? offset ?*/,
          .target_length = /*? offset ?*/ + sizeof(* /*? ret ?*/),
        }), ({
          return UINT_MAX;
        }));
      memcpy(/*? base ?*/ + /*? offset ?*/, /*? ret ?*/, sizeof(* /*? ret ?*/));
      /*? offset ?*/ += sizeof(* /*? ret ?*/);
    /*- endif -*/

    return /*? offset ?*/;
  }
/*- endif -*/
/*- for p in output_parameters -*/
  /*- set offset = c_symbol('offset') -*/
  static unsigned int /*? function ?*/_/*? p.name ?*/(unsigned int /*? offset ?*/,
    /*- if p.array -*/
      const size_t * /*? p.name ?*/_sz,
      /*- if isinstance(p.type, camkes.ast.Type) and p.type.type == 'string' -*/
        char *** /*? p.name ?*/
      /*- else -*/
        /*? show(p.type) ?*/ ** /*? p.name ?*/
      /*- endif -*/
    /*- elif isinstance(p.type, camkes.ast.Type) and p.type.type == 'string' -*/
      char ** /*? p.name ?*/
    /*- else -*/
      const /*? show(p.type) ?*/ * /*? p.name ?*/
    /*- endif -*/
  ) {

    /*- set base = c_symbol('buffer_base') -*/
    void * /*? base ?*/ UNUSED = (void*)(/*? buffer ?*/);

    /*- if p.array -*/
      ERR_IF(/*? offset ?*/ + sizeof(* /*? p.name ?*/_sz) > /*? size ?*/, /*? error_handler ?*/, ((camkes_error_t){
          .type = CE_BUFFER_LENGTH_EXCEEDED,
          .instance = "/*? instance ?*/",
          .interface = "/*? interface ?*/",
          .description = "buffer exceeded while marshalling /*? p.name ?*/ in /*? name ?*/",
          .current_length = /*? offset ?*/,
          .target_length = /*? offset ?*/ + sizeof(* /*? p.name ?*/_sz),
        }), ({
          return UINT_MAX;
        }));
      memcpy(/*? base ?*/ + /*? offset ?*/, /*? p.name ?*/_sz, sizeof(* /*? p.name ?*/_sz));
      /*? offset ?*/ += sizeof(* /*? p.name ?*/_sz);
      /*- if isinstance(p.type, camkes.ast.Type) and p.type.type == 'string' -*/
        /*- set lcount = c_symbol() -*/
        for (int /*? lcount ?*/ = 0; /*? lcount ?*/ < * /*? p.name ?*/_sz; /*? lcount ?*/ ++) {
          /*- set strlen = c_symbol('strlen') -*/
          size_t /*? strlen ?*/ = strnlen((* /*? p.name ?*/)[/*? lcount ?*/], /*? size ?*/ - /*? offset ?*/);
          ERR_IF(/*? strlen ?*/ >= /*? size ?*/ - /*? offset ?*/, /*? error_handler ?*/, ((camkes_error_t){
              .type = CE_BUFFER_LENGTH_EXCEEDED,
              .instance = "/*? instance ?*/",
              .interface = "/*? interface ?*/",
              .description = "buffer exceeded while marshalling /*? p.name ?*/ in /*? name ?*/",
              .current_length = /*? offset ?*/,
              .target_length = /*? offset ?*/ + /*? strlen ?*/ + 1,
            }), ({
              return UINT_MAX;
            }));
          /* If we didn't trigger an error, we now know this strcpy is safe. */
          (void)strcpy(/*? base ?*/ + /*? offset ?*/, (* /*? p.name ?*/)[/*? lcount ?*/]);
          /*? offset ?*/ += /*? strlen ?*/ + 1;
        }
      /*- else -*/
        ERR_IF(/*? offset ?*/ + sizeof((* /*? p.name ?*/)[0]) * (* /*? p.name ?*/_sz) > /*? size ?*/, /*? error_handler ?*/, ((camkes_error_t){
            .type = CE_BUFFER_LENGTH_EXCEEDED,
            .instance = "/*? instance ?*/",
            .interface = "/*? interface ?*/",
            .description = "buffer exceeded while marshalling /*? p.name ?*/ in /*? name ?*/",
            .current_length = /*? offset ?*/,
            .target_length = /*? offset ?*/ + sizeof((* /*? p.name ?*/)[0]) * (* /*? p.name ?*/_sz),
          }), ({
            return UINT_MAX;
          }));
        memcpy(/*? base ?*/ + /*? offset ?*/, * /*? p.name ?*/, sizeof((* /*? p.name ?*/)[0]) * (* /*? p.name ?*/_sz));
        /*? offset ?*/ += sizeof((* /*? p.name ?*/)[0]) * (* /*? p.name ?*/_sz);
      /*- endif -*/
    /*- elif isinstance(p.type, camkes.ast.Type) and p.type.type == 'string' -*/
      /*- set strlen = c_symbol('strlen') -*/
      size_t /*? strlen ?*/ = strnlen(* /*? p.name ?*/, /*? size ?*/ - /*? offset ?*/);
      ERR_IF(/*? strlen ?*/ >= /*? size ?*/ - /*? offset ?*/, /*? error_handler ?*/, ((camkes_error_t){
          .type = CE_BUFFER_LENGTH_EXCEEDED,
          .instance = "/*? instance ?*/",
          .interface = "/*? interface ?*/",
          .description = "buffer exceeded while marshalling /*? p.name ?*/ in /*? name ?*/",
          .current_length = /*? offset ?*/,
          .target_length = /*? offset ?*/ + /*? strlen ?*/ + 1,
        }), ({
          return UINT_MAX;
        }));
      /* If we didn't trigger an error, we now know this strcpy is safe. */
      (void)strcpy(/*? base ?*/ + /*? offset ?*/, * /*? p.name ?*/);
      /*? offset ?*/ += /*? strlen ?*/ + 1;
    /*- else -*/
      ERR_IF(/*? offset ?*/ + sizeof(* /*? p.name ?*/) > /*? size ?*/, /*? error_handler ?*/, ((camkes_error_t){
          .type = CE_BUFFER_LENGTH_EXCEEDED,
          .instance = "/*? instance ?*/",
          .interface = "/*? interface ?*/",
          .description = "buffer exceeded while marshalling /*? p.name ?*/ in /*? name ?*/",
          .current_length = /*? offset ?*/,
          .target_length = /*? offset ?*/ + sizeof(* /*? p.name ?*/),
        }), ({
          return UINT_MAX;
        }));
      memcpy(/*? base ?*/ + /*? offset ?*/, /*? p.name ?*/, sizeof(* /*? p.name ?*/));
      /*? offset ?*/ += sizeof(* /*? p.name ?*/);
    /*- endif -*/

    return /*? offset ?*/;
  }
/*- endfor -*/

static unsigned int /*? function ?*/(
/*- set ret = c_symbol('return') -*/
/*- if return_type is not none -*/
  /*- if isinstance(return_type, camkes.ast.Type) and return_type.type == 'string' -*/
    char ** /*? ret ?*/
  /*- else -*/
    const /*? show(return_type) ?*/ * /*? ret ?*/
  /*- endif -*/
  /*- if len(output_parameters) > 0 -*/
    ,
  /*- endif -*/
/*- endif -*/
/*- for p in output_parameters -*/
  /*- if p.array -*/
    const size_t * /*? p.name ?*/_sz,
    /*- if isinstance(p.type, camkes.ast.Type) and p.type.type == 'string' -*/
      char *** /*? p.name ?*/
    /*- else -*/
      /*? show(p.type) ?*/ ** /*? p.name ?*/
    /*- endif -*/
  /*- elif isinstance(p.type, camkes.ast.Type) and p.type.type == 'string' -*/
    char ** /*? p.name ?*/
  /*- else -*/
    const /*? show(p.type) ?*/ * /*? p.name ?*/
  /*- endif -*/
  /*- if not loop.last -*/
    ,
  /*- endif -*/
/*- endfor -*/
/*- if return_type is none and len(output_parameters) == 0 -*/
  void
/*- endif -*/
) {

  /*- set length = c_symbol('length') -*/
  unsigned int /*? length ?*/ = 0;

  /*- if return_type is not none -*/
    /*? length ?*/ = /*? function ?*/_/*? ret_fn ?*/(/*? length ?*/,
      /*? ret ?*/
    );
    if (/*? length ?*/ == UINT_MAX) {
      return UINT_MAX;
    }
  /*- endif -*/

  /* Marshal output parameters. */
  /*- for p in output_parameters -*/
    /*? assert(isinstance(p.type, camkes.ast.Type) or isinstance(p.type, camkes.ast.Reference)) ?*/
    /*? length ?*/ = /*? function ?*/_/*? p.name ?*/(/*? length ?*/,
      /*- if p.array -*/
        /*? p.name ?*/_sz,
      /*- endif -*/
      /*? p.name ?*/
    );
    if (/*? length ?*/ == UINT_MAX) {
      return UINT_MAX;
    }
  /*- endfor -*/

  assert(/*? length ?*/ <= /*? size ?*/ &&
    "uncaught buffer overflow while marshalling outputs for /*? name ?*/");

  return /*? length ?*/;
}
