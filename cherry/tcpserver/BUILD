cc_library(
    name = "net",
    srcs = [
        "acceptor.cpp",
        "tcpserver.cpp",
    ],
    hdrs = [
        "acceptor.h",
        "tcpserver.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//cherry/net",
        "//cherry/utils:logging",
    ],
)

cc_test(
    name = "acceptor_test",
    srcs = ["acceptor_test.cpp"],
    deps = [
        ":net",
        "//cherry/utils:logging",
    ],
)