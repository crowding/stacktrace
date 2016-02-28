#include <R.h>
#include <Rinternals.h>
#include <setjmp.h>

void assert_type(SEXP, SEXPTYPE);
void assert_type3(SEXP, SEXPTYPE, const char *);
SEXP allocate_dots(int length);

# define JMP_BUF jmp_buf
typedef struct {
  int tag;
  union {
    int ival;
    double dval;
    SEXP sxpval;
  } u;
} R_bcstack_t;

typedef struct RCNTXT {
  struct RCNTXT *nextcontext;	/* The next context up the chain */
    int callflag;		/* The context "type" */
      int cjmpbuf[_JBLEN + 1]; // oops an extra int was needed to fix alignment on my machine. How can I tell during build???
      //JMP_BUF cjmpbuf;		/* C stack and register information */
    int cstacktop;		    /* Top of the pointer protection stack */
    int evaldepth;	      /* evaluation depth at inception */
    SEXP promargs;	    	/* Promises supplied to closure */
    SEXP callfun;		      /* The closure called */
    SEXP sysparent;	    	/* environment the closure was called from */
    SEXP call;			      /* The call that effected this context*/
    SEXP cloenv;	      	/* The environment */
    SEXP conexit;		      /* Interpreted "on.exit" code */
    void (*cend)(void *);	/* C "on.exit" thunk */
    void *cenddata;		    /* data for C "on.exit" thunk */
    void *vmax;		        /* top of R_alloc stack */
    int intsusp;          /* interrupts are suspended */
    SEXP handlerstack;    /* condition handler stack */
    SEXP restartstack;    /* stack of available restarts */
    struct RPRSTACK *prstack;   /* stack of pending promises */
    R_bcstack_t *nodestack;
  #ifdef BC_INT_STACK
  IStackval *intstack;
  #endif
  SEXP srcref;	         /* The source line in effect */
    int browserfinish;     /* should browser finish this context without stopping */
    SEXP returnValue;			 /* only set during on.exit calls */
} RCNTXT;

enum {
  CTXT_TOPLEVEL = 0,
  CTXT_NEXT	  = 1,
  CTXT_BREAK	  = 2,
  CTXT_LOOP	  = 3,	/* break OR next target */
    CTXT_FUNCTION = 4,
    CTXT_CCODE	  = 8,
    CTXT_RETURN	  = 12,
    CTXT_BROWSER  = 16,
    CTXT_GENERIC  = 20,
    CTXT_RESTART  = 32,
    CTXT_BUILTIN  = 64  /* used in profiling */
};
