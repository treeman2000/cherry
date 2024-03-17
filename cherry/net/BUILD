cc_library(
    name = "net",
    srcs = [
        "channel.cpp",
        "eventloop.cpp",
    ],
    hdrs = [
        "channel.h",
        "eventloop.h",
    ],
    visibility = ["//visibility:public"],
    deps = ["//cherry/utils:logging"],
)

# initial version
cc_library(
    name = "eventloop",
    srcs = ["eventloop.cpp"],
    hdrs = ["eventloop.h"],
    visibility = ["//visibility:public"],
    deps = ["//cherry/utils:logging"],
)

cc_test(
    name = "eventloop_test",
    srcs = ["eventloop_test.cpp"],
    deps = [":eventloop"],
)
