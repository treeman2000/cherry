cc_library(
    name = "net",
    srcs = [
        "channel.cpp",
        "eventloop.cpp",
        "eventloop_thread.cpp",
        "poller.cpp",
    ],
    hdrs = [
        "channel.h",
        "eventloop.h",
        "eventloop_thread.h",
        "poller.h",
    ],
    visibility = ["//visibility:public"],
    deps = ["//cherry/utils:logging"],
)

cc_test(
    name = "elt_test",
    srcs = ["eventloop_thread_test.cpp"],
    deps = [":net"],
)

cc_test(
    name = "el_test",
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
