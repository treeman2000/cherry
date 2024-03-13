cc_library(
    name = "util",
    hdrs = ["util.h"],
    srcs = ["util.cpp"],
    # 默认只有一个build文件中的目标能互相看见
    visibility = ["//visibility:public"],
)

cc_library(
    name = "time",
    hdrs = ["time.h"],
    srcs = ["time.cpp"],
    # 默认只有一个build文件中的目标能互相看见
    visibility = ["//visibility:public"],
)

cc_test(
    name = "time_test",
    srcs = ["time_test.cpp"],
    deps = [":time"],
)
