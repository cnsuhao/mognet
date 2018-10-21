load("@org_meme_rules_protobuf//cpp:rules.bzl", "cc_proto_library")

cc_proto_library(
  name = "proto",
  protos = ["vision/result.proto", "vision/service.proto"],
  with_grpc = True,
  visibility = ["//visibility:public"],
)
