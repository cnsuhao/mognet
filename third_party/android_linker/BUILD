licenses(["notice"])

cc_library(
    name = "android_linker",
    linkstatic = True,
    srcs = glob(["src/**/*.c"]),
    hdrs = glob(["include/**/*.h", "src/**/*.h"]),
    includes = ["include/"],
    copts = ["-m32"],
    defines = [
        "ANDROID_X86_LINKER",
        "_GNU_SOURCE",
        "LINKER_TEXT_BASE=0xB0000100",
        "LINKER_AREA_SIZE=0x01000000",
        "LINKER_DEBUG=1"
    ],
    visibility = ["//visibility:public"],
)
