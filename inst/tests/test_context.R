context("stacktrace")

test_that("Stacktrace runs", {
  expect_true(!is.null(x<-stacktrace()))
})

test_that("Stacktrace prints", {
  expect_true(!is.null(capture.output(print(stacktrace()))))
})
