cc_library(
    name = "eventloop",
    hdrs = ["eventloop.h"],
    srcs = ["eventloop.cpp"],
    deps = ["//cherry/utils:logging"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "eventloop_test",
    srcs = ["eventloop_test.cpp"],
    deps = [":eventloop"],
)