/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
#ifndef TVM_SCRIPT_IR_BUILDER_TIR_FRAME_H_
#define TVM_SCRIPT_IR_BUILDER_TIR_FRAME_H_

#include <tvm/script/ir_builder/base.h>
#include <tvm/script/ir_builder/ir/frame.h>
#include <tvm/tir/stmt.h>

namespace tvm {
namespace script {
namespace ir_builder {
namespace tir {

/*!
 * \brief A base frame that represents the TIR fame with body of statements.
 *
 * \sa TIRFrame
 */
class TIRFrameNode : public IRBuilderFrameNode {
 public:
  /*! \brief The Stmt within in this frame. */
  Array<tvm::tir::Stmt> stmts;

  void VisitAttrs(tvm::AttrVisitor* v) {
    IRBuilderFrameNode::VisitAttrs(v);
    v->Visit("stmts", &stmts);
  }

  static constexpr const char* _type_key = "script.ir_builder.tir.TIRFrame";
  TVM_DECLARE_BASE_OBJECT_INFO(TIRFrameNode, IRBuilderFrameNode);
};

/*!
 * \brief Managed reference to TIRFrameNode.
 *
 * \sa TIRFrameNode
 */
class TIRFrame : public IRBuilderFrame {
 public:
  TVM_DEFINE_MUTABLE_NOTNULLABLE_OBJECT_REF_METHODS(TIRFrame, IRBuilderFrame, TIRFrameNode);

 protected:
  TIRFrame() = default;
};

/*!
 * \brief A frame that represents the PrimFunc containing TIR statements.
 *
 * \sa PrimFuncFrame
 */
class PrimFuncFrameNode : public TIRFrameNode {
 public:
  /*! \brief The name of the block. */
  Optional<String> name;
  /*! \brief Function parameters. */
  Array<tvm::tir::Var> args;
  /*! \brief The return type of the function. */
  Optional<Type> ret_type;
  /*! \brief Maps some parameters to specific Buffer data structures. */
  Map<tvm::tir::Var, tvm::tir::Buffer> buffer_map;
  /*! \brief The buffer map prior to flattening. */
  Map<tvm::tir::Var, tvm::tir::Buffer> preflattened_buffer_map;
  /*! \brief Additional attributes storing the meta-data */
  Optional<Map<String, ObjectRef>> attrs;
  /*! \brief The variable map bound to thread env. */
  Map<tvm::tir::Var, tvm::tir::IterVar> env_threads;
  /*! \brief The buffer allocated in root block. */
  Array<tvm::tir::Buffer> root_alloc_buffers;

  void VisitAttrs(tvm::AttrVisitor* v) {
    TIRFrameNode::VisitAttrs(v);
    v->Visit("name", &name);
    v->Visit("args", &args);
    v->Visit("ret_type", &ret_type);
    v->Visit("buffer_map", &buffer_map);
    v->Visit("preflattened_buffer_map", &preflattened_buffer_map);
    v->Visit("attrs", &attrs);
    v->Visit("env_threads", &env_threads);
    v->Visit("root_alloc_buffers", &root_alloc_buffers);
  }

  static constexpr const char* _type_key = "script.ir_builder.tir.PrimFuncFrame";
  TVM_DECLARE_FINAL_OBJECT_INFO(PrimFuncFrameNode, TIRFrameNode);

 public:
  /*!
   * \brief The method called when exiting RAII scope.
   * \sa tvm::support::With
   */
  void ExitWithScope() final;
};

/*!
 * \brief Managed reference to PrimFuncFrameNode.
 *
 * \sa PrimFuncFrameNode
 */
class PrimFuncFrame : public TIRFrame {
 public:
  TVM_DEFINE_MUTABLE_NOTNULLABLE_OBJECT_REF_METHODS(PrimFuncFrame, TIRFrame, PrimFuncFrameNode);
};

/*!
 * \brief A frame that represents the block.
 *
 * \sa BlockFrame
 */
class BlockFrameNode : public TIRFrameNode {
 public:
  /*! \brief The name of the block. */
  String name;
  /*! \brief The variables of the block. */
  Array<tvm::tir::IterVar> iter_vars;
  /*! \brief The read buffer regions of the block. */
  Optional<Array<tvm::tir::BufferRegion>> reads;
  /*! \brief The write buffer regions of the block. */
  Optional<Array<tvm::tir::BufferRegion>> writes;
  /*! \brief The init statement of the bolck. */
  Optional<tvm::tir::Stmt> init;
  /*! \brief The buffer allocated in the block. */
  Array<tvm::tir::Buffer> alloc_buffers;
  /*! \brief The match buffer regions. */
  Array<tvm::tir::MatchBufferRegion> match_buffers;
  /*! \brief The annotation of the block. */
  Optional<Map<String, ObjectRef>> annotations;
  /*! \brief The corresponding values of the iter vars. */
  Array<PrimExpr> iter_values;
  /*!
   * \brief The predicate of the block realization, the block will only be executed when the
   * predicate is true.
   */
  Optional<PrimExpr> predicate;
  /*! \brief The flag whether to construct BlockRealize or Block. */
  bool no_realize;

  void VisitAttrs(tvm::AttrVisitor* v) {
    TIRFrameNode::VisitAttrs(v);
    v->Visit("name", &name);
    v->Visit("iter_vars", &iter_vars);
    v->Visit("reads", &reads);
    v->Visit("writes", &writes);
    v->Visit("init", &init);
    v->Visit("alloc_buffers", &alloc_buffers);
    v->Visit("match_buffers", &match_buffers);
    v->Visit("annotations", &annotations);
    v->Visit("iter_values", &iter_values);
    v->Visit("predicate", &predicate);
    v->Visit("no_realize", &no_realize);
  }

  static constexpr const char* _type_key = "script.ir_builder.tir.BlockFrame";
  TVM_DECLARE_FINAL_OBJECT_INFO(BlockFrameNode, TIRFrameNode);

 public:
  /*!
   * \brief The method called when exiting RAII scope.
   * \sa tvm::support::With
   */
  void ExitWithScope() final;
};

/*!
 * \brief Managed reference to BlockFrameNode.
 *
 * \sa BlockFrameNode
 */

class BlockFrame : public TIRFrame {
 public:
  TVM_DEFINE_MUTABLE_NOTNULLABLE_OBJECT_REF_METHODS(BlockFrame, TIRFrame, BlockFrameNode);
};

/*!
 * \brief A frame that represents the assert statement. Proceeds if the condition is true,
 * otherwise aborts with the message.
 *
 * \sa AssertFrame
 */
class AssertFrameNode : public TIRFrameNode {
 public:
  /*! \brief The PrimExpr to test. */
  PrimExpr condition;
  /*! \brief The output error message when the assertion failed. */
  PrimExpr message;

  void VisitAttrs(tvm::AttrVisitor* v) {
    TIRFrameNode::VisitAttrs(v);
    v->Visit("condition", &condition);
    v->Visit("message", &message);
  }

  static constexpr const char* _type_key = "script.ir_builder.tir.AssertFrame";
  TVM_DECLARE_FINAL_OBJECT_INFO(AssertFrameNode, TIRFrameNode);

 public:
  /*!
   * \brief The method called when exiting RAII scope.
   * \sa tvm::support::With
   */
  void ExitWithScope() final;
};

}  // namespace tir
}  // namespace ir_builder
}  // namespace script
}  // namespace tvm

#endif  // TVM_SCRIPT_IR_BUILDER_TIR_FRAME_H_
