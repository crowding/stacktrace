#' Extract the current context stack to a data frame.
#' @return A data frame with one row per stack frame, with deeper frames
#'   first. The stack frame that executes "context" is included, as well
#'   as context frames corresponding to builtins, looping and such.
#' @useDynLib stacktrace _context_stack
#' @import nseval
#' @export
stacktrace <- function() {
  out <- .Call(`_context_stack`) 
  d2flist <- function(x) .Call("_dotsxp_to_flist", x, PACKAGE="nseval")
  out$promargs <- lapply(out$promargs, d2flist)
  class(out$callfun) <- "oneline"
  class(out$call) <- "oneline"
  class(out$promargs) <- "oneline"
  out
}
