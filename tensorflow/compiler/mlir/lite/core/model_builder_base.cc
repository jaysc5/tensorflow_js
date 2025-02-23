/* Copyright 2017 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "tensorflow/compiler/mlir/lite/core/model_builder_base.h"

#include <stddef.h>

#include <memory>
#include <string>

#include "absl/strings/str_cat.h"
#include "tensorflow/compiler/mlir/lite/allocation.h"
#include "tensorflow/compiler/mlir/lite/core/api/error_reporter.h"

namespace tflite {

namespace impl {
void PrintModelLayers(const tflite::Model* model) {
  if (!model || !model->subgraphs()) {
    std::cerr << "Invalid model or no subgraphs found.\n";
    return;
  }

  for (size_t subgraph_idx = 0; subgraph_idx < model->subgraphs()->size(); ++subgraph_idx) {
    const tflite::SubGraph* subgraph = model->subgraphs()->Get(subgraph_idx);
    std::cout << "SubGraph " << subgraph_idx << " has " 
              << subgraph->operators()->size() << " layers.\n";

    for (size_t op_idx = 0; op_idx < subgraph->operators()->size(); ++op_idx) {
      const tflite::Operator* op = subgraph->operators()->Get(op_idx);
      const tflite::OperatorCode* op_code = model->operator_codes()->Get(op->opcode_index());
      const char* op_name = tflite::EnumNameBuiltinOperator(
          static_cast<tflite::BuiltinOperator>(op_code->builtin_code()));

      std::cout << "  Layer " << op_idx << ": " << op_name << "\n";
      std::cout << "    Inputs: ";
      for (int32_t input_idx : *op->inputs()) {
        std::cout << input_idx << " ";
      }
      std::cout << "\n    Outputs: ";
      for (int32_t output_idx : *op->outputs()) {
        std::cout << output_idx << " ";
      }
      std::cout << "\n";
    }
  }
}
}

#ifndef TFLITE_MCU
// Loads a model from `filename`. If `mmap_file` is true then use mmap,
// otherwise make a copy of the model in a buffer.
std::unique_ptr<Allocation> GetAllocationFromFile(
    const char* filename, ErrorReporter* error_reporter) {
  std::unique_ptr<Allocation> allocation;
  if (MMAPAllocation::IsSupported()) {
    allocation = std::make_unique<MMAPAllocation>(filename, error_reporter);
  } else {
    allocation = std::make_unique<FileCopyAllocation>(filename, error_reporter);
  }
  return allocation;
}

// Loads a model from `fd`. If `mmap_file` is true then use mmap,
// otherwise make a copy of the model in a buffer.
std::unique_ptr<Allocation> GetAllocationFromFile(
    int fd, ErrorReporter* error_reporter) {
  std::unique_ptr<Allocation> allocation;
  if (MMAPAllocation::IsSupported()) {
    allocation = std::make_unique<MMAPAllocation>(fd, error_reporter);
  } else {
    allocation = std::make_unique<FileCopyAllocation>(
        absl::StrCat("/proc/self/fd/", fd).c_str(), error_reporter);
  }
  return allocation;
}

#endif

}  // namespace tflite
