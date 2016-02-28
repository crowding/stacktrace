## Stacktrace

The function stacktrace() dumps R's current context stack into a data frame.
This stack may be somewhat different from the various fictions constructed by sys.*
functions. Since this package inspects data structures that are not exported parts of R's API,
it may be unstable or need adjustment to work on your system.

```
   callflag evaldepth                       promargs                        callfun                  sysparent                           call                     cloenv
1         0         0                           NULL                           NULL        <environment: base>                           NULL        <environment: base>
2        20         1  args(<environment: base> ? x)          function (x, ...) ... <environment: 0x11283cdc8>          function (x, ...) ... <environment: 0x1130f12d8>
3        12         2  args(<environment: base> ? x) function (x, ..., digits =.... <environment: 0x11283cdc8>            print.data.frame(x) <environment: 0x11283cd58>
4        12         3 args(<environment: base> ?....          function (x, ...) ... <environment: 0x11283cd58> as.matrix(format.data.fram.... <environment: 0x11438e000>
5        12         6 args(<environment: base> ?.... function (x, ..., justify .... <environment: 0x11283cd58> format.data.frame(x, digit.... <environment: 0x110ff2ad8>
6        20         7 args(<environment: base> ?....          function (x, ...) ... <environment: 0x110ff2ad8> format(x[[i]], ..., justif.... <environment: 0x11438eb10>
7        12         8 args(<environment: base> ?.... function (x, width = 20, ..... <environment: 0x110ff2ad8> format.oneline(x[[i]], ....... <environment: 0x110ff2c28>
8        12         9 args(<environment: base> ?....       function (x, f, ...) ... <environment: 0x110ff2c28> one_line(list(x), format_r.... <environment: 0x1130e5838>
9        12        10 args(<environment: base> ?....     function (X, FUN, ...) ... <environment: 0x1130e5838>                   lapply(x, f) <environment: 0x1130e87c8>
10       12        11 args(<environment: base> ?....          function (x, ...) ... <environment: 0x1130e87c8>               FUN(X[[i]], ...) <environment: 0x112806740>
11       12        12 args(<environment: base> ?.... function (expr, ..., final.... <environment: 0x112806740> tryCatch(format.AsIs(x, ...... <environment: 0x112809ed0>
12       12        13 args(<environment: base> ?.... function (expr, names, par.... <environment: 0x112809ed0> tryCatchList(expr, classes.... <environment: 0x1130ec2d8>
13       12        14 args(<environment: base> ?.... function (expr, name, pare.... <environment: 0x1130ec2d8> tryCatchOne(expr, names, p.... <environment: 0x112809f08>
14       12        15 args(<environment: 0x11280.... function (expr, name, pare.... <environment: 0x112809f08> doTryCatch(return(expr), n.... <environment: 0x111613828>
15       12        20  args(<environment: base> ? x) function (x, width = 12, ..... <environment: 0x112806740>            format.AsIs(x, ...) <environment: 0x1130ebee8>
16       20        21 args(<environment: base> ?....          function (x, ...) ... <environment: 0x1130ebee8> toString(y, width = width,.... <environment: 0x111613438>
17       12        22 args(<environment: base> ?.... function (x, width = NULL,.... <environment: 0x1130ebee8> toString.default(y, width .... <environment: 0x1130ef0a8>
18       12        23 args(<environment: base> ?.... function (..., sep = " ", .... <environment: 0x1130ef0a8>      paste(x, collapse = ", ") <environment: 0x1113b0b48>
19       12        30                           NULL                function () ... <environment: 0x1061bc1c0>                   stacktrace() <environment: 0x1113b06b0>
```
