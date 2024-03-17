cc_library(
    name = "net",
    srcs = [
        "channel.cpp",
        "eventloop.cpp",
        "poller.cpp",
    ],
    hdrs = [
        "channel.h",
        "eventloop.h",
        "poller.h",
    ],
    visibility = ["//visibility:public"],
    deps = ["//cherry/utils:logging"],
)

cc_test(
    name = "net_test",
    srcs = ["eventloop_test.cpp"],
    deps = [":net"],
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
