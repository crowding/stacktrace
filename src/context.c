#include "context.h"
#include <Rinterface.h>
#include <R_ext/Boolean.h>

void assert_type3(SEXP x, SEXPTYPE type, const char *what) {
  if (TYPEOF(x) != type) {
    error("Expected %s in %s, got %s",
          type2char(type), what, type2char(TYPEOF(x)));
  }
}

/* How deep is the given stack frame. (The top stack frame has depth 0) */
int contextDepth(RCNTXT *cptr)
{
  int n = 0;
  while (cptr->nextcontext != NULL) {
    cptr = cptr->nextcontext;
    n++;
  }
  return n;
}

SEXP copy_dotlist(SEXP promlist, Rboolean deep) {
  //Copy a promlist (or dotlist) into a new dotlist.
  //If deep, copies promise objects as well rather than referencing them.
  int length = 0;
  {
    SEXP list = promlist;
    while (list != R_NilValue) {list = CDR(list); length++;}
  }
  if (length == 0) return R_NilValue;

  SEXP newlist = PROTECT(allocList(length));

  for (SEXP old = promlist, nu = newlist;
       old != R_NilValue;
       old = CDR(old), nu = CDR(nu)) {
    SET_TYPEOF(nu, DOTSXP);
    if (TYPEOF(old) != LISTSXP && TYPEOF(old) != DOTSXP) {
      error("Expected dotlist or pairlist in copy_dotlist, got %s.", type2char(TYPEOF(old)));
    }

    SEXP head = CAR(old);

    if (TYPEOF(head) == PROMSXP) {
      //Rprintf("Copying promise at %p\n", CAR(old));
      if (deep) {
        SETCAR(nu, duplicate(head));
      } else {
        SETCAR(nu, head);
      }
    } else {
      SEXP nuProm;
      SETCAR(nu, nuProm = allocSExp(PROMSXP));
      //Rprintf("injecting %d into completed promise at %p\n", type2char(TYPEOF(CAR(old))), nuProm);
      SET_PRVALUE(nuProm, head);
      SET_PRCODE(nuProm, head);
      SET_PRENV(nuProm, R_NilValue);
      SET_PRSEEN(nuProm, 0);
    }
    SET_TAG(nu, TAG(old));
  }
  setAttrib(newlist, R_ClassSymbol, ScalarString(mkChar("...")));

  UNPROTECT(1);
  return newlist;
}

SEXP _context_stack() {
  RCNTXT *c = (RCNTXT *) R_GlobalContext;
  int depth = contextDepth(c);
  int protection = 0;

  SEXP dataFrame, colNames, names;

  PROTECT(dataFrame = allocVector(VECSXP, 7));
  PROTECT(colNames = allocVector(STRSXP, 7));
  PROTECT(names = allocVector(REALSXP, 2));
  REAL(names)[0] = NA_REAL;
  REAL(names)[1] = -(double)(depth+1);
  protection += 3;

  int i = 0;
  SEXP callflag;
  PROTECT(callflag = allocVector(INTSXP, depth+1));
  SET_VECTOR_ELT(dataFrame, i, callflag);
  SET_STRING_ELT(colNames, i++, mkChar("callflag"));

  SEXP evaldepth;
  PROTECT(evaldepth = allocVector(INTSXP, depth+1));
  SET_VECTOR_ELT(dataFrame, i, evaldepth);
  SET_STRING_ELT(colNames, i++, mkChar("evaldepth"));

  SEXP promargs;
  PROTECT(promargs = allocVector(VECSXP, depth+1));
  SET_VECTOR_ELT(dataFrame, i, promargs);
  SET_STRING_ELT(colNames, i++, mkChar("promargs"));

  SEXP callfun;
  PROTECT(callfun = allocVector(VECSXP, depth+1));
  SET_VECTOR_ELT(dataFrame, i, callfun);
  SET_STRING_ELT(colNames, i++, mkChar("callfun"));

  SEXP sysparent;
  PROTECT(sysparent = allocVector(VECSXP, depth+1));
  SET_VECTOR_ELT(dataFrame, i, sysparent);
  SET_STRING_ELT(colNames, i++, mkChar("sysparent"));

  SEXP call;
  PROTECT(call = allocVector(VECSXP, depth+1));
  SET_VECTOR_ELT(dataFrame, i, call);
  SET_STRING_ELT(colNames, i++, mkChar("call"));

  SEXP cloenv;
  PROTECT(cloenv = allocVector(VECSXP, depth+1));
  SET_VECTOR_ELT(dataFrame, i, cloenv);
  SET_STRING_ELT(colNames, i++, mkChar("cloenv"));
  setAttrib(dataFrame, R_NamesSymbol, colNames);
  setAttrib(dataFrame, R_RowNamesSymbol, names);
  setAttrib(dataFrame, R_ClassSymbol, ScalarString(mkChar("data.frame")));
  protection += i;
  //Rprintf("Allocated data frame with %d rows...\n", depth+1);

  for (int i = depth;
       i >= 0 && c != NULL;
       i--, c = c->nextcontext)
  {
    //Rprintf("[%d/%d]: callflag\n", i, depth);
    INTEGER(callflag)[i] = c->callflag;
    //Rprintf("[%d/%d]: evaldepth\n", i, depth);
    INTEGER(evaldepth)[i] = c->evaldepth;
    //Rprintf("[%d/%d]: promargs\n", i, depth);
    SET_VECTOR_ELT(promargs, i, copy_dotlist(c->promargs, TRUE));
    //Rprintf("[%d/%d]: callfun\n", i, depth);
    SET_VECTOR_ELT(callfun, i, c->callfun);
    //Rprintf("[%d/%d]: sysperant\n", i, depth);
    SET_VECTOR_ELT(sysparent, i, c->sysparent);
    //Rprintf("[%d/%d]: call\n", i, depth);
    SET_VECTOR_ELT(call, i, c->call);
    //Rprintf("[%d/%d]: cloenv\n", i, depth);
    SET_VECTOR_ELT(cloenv, i, c->cloenv);
    //Rprintf("[%d/%d]: rownames\n", i, depth);
  }
  UNPROTECT(protection);
  //Rprintf("Done\n");
  return(dataFrame);
}
