# Copyright 2024 Google LLC.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

package(
    # copybara:uncomment default_applicable_licenses = ["//tensorflow:license"],
    default_visibility = ["//tensorflow/lite/experimental/litert:__subpackages__"],
)

# TODO: b/365295276 - Make custom rule and move to `.sh`.
OUT_DIR = "$(RULEDIR)"

CONVERTER = "//tensorflow/compiler/mlir/lite:tf_tfl_translate"

CMD = """
for mlir_file in $(SRCS); do
    $(location {converter}) --input-mlir $$mlir_file --o={out_dir}/testdata/$$(basename $$mlir_file .mlir).tflite
done
""".format(
    converter = CONVERTER,
    out_dir = OUT_DIR,
)

genrule(
    name = "tflite_test_data",
    srcs = glob(["testdata/*.mlir"]),
    outs = [s.removesuffix(".mlir") + ".tflite" for s in glob(["testdata/*.mlir"])],
    cmd = CMD,
    tools = [CONVERTER],
)

cc_library(
    name = "common",
    testonly = 1,
    srcs = [
        "common.cc",
    ],
    hdrs = [
        "common.h",
    ],
    deps = [
        "//tensorflow/compiler/mlir/lite:allocation",
        "//tensorflow/lite:framework",
        "//tensorflow/lite:model_builder",
        "//tensorflow/lite/c:c_api_opaque",
        "//tensorflow/lite/c:common",
        "//tensorflow/lite/core:cc_api_stable",
        "//tensorflow/lite/experimental/litert/c:litert_common",
        "//tensorflow/lite/experimental/litert/cc:litert_buffer_ref",
        "//tensorflow/lite/experimental/litert/cc:litert_expected",
        "//tensorflow/lite/experimental/litert/cc:litert_model",
        "//tensorflow/lite/experimental/litert/cc:litert_model_predicates",
        "//tensorflow/lite/experimental/litert/core/model:model_buffer",
        "//tensorflow/lite/experimental/litert/core/model:model_load",
        "//tensorflow/lite/kernels:builtin_ops",
        "//tensorflow/lite/schema:schema_fbs",
        "@com_google_absl//absl/log:absl_check",
        "@com_google_absl//absl/strings:string_view",
        "@local_tsl//tsl/platform",
    ],
)

cc_library(
    name = "simple_model",
    testonly = 1,
    hdrs = [
        "testdata/simple_model_test_vectors.h",
    ],
    data = [
        "testdata/simple_model.tflite",
        "testdata/simple_model_google_tensor.bin",
        "testdata/simple_model_qualcomm.bin",
    ],
    deps = [
        "//tensorflow/lite/experimental/litert/c:litert_model",
    ],
)

cc_library(
    name = "simple_model_npu",
    testonly = 1,
    srcs = [],
    hdrs = [
        "testdata/simple_model_test_vectors.h",
    ],
    data = [
        "testdata/simple_model_google_tensor.bin",
        "testdata/simple_model_npu.tflite",
        "testdata/simple_model_qualcomm.bin",
    ],
    deps = [
        "//tensorflow/lite/experimental/litert/c:litert_model",
    ],
)
