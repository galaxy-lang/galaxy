#include "backend/generator/expressions/generate_expr.hpp"
#include "backend/generator/expressions/generate_numeric_literal.hpp"
#include "backend/generator/expressions/generate_identifier.hpp"
#include "backend/generator/expressions/generate_binary_expr.hpp"
#include "backend/generator/expressions/generate_unary_minus.hpp"
#include "backend/generator/expressions/generate_logical_not.hpp"
#include "backend/generator/expressions/generate_unary_bitwise_not.hpp"
#include "backend/generator/expressions/generate_pre_increment.hpp"
#include "backend/generator/expressions/generate_pre_decrement.hpp"
#include "backend/generator/expressions/generate_assignment_expr.hpp"

llvm::Value *generate_expr(AstNode *node, llvm::LLVMContext &Context, llvm::IRBuilder<> &Builder) {
    switch (node->kind) {
        case NODE_NUMERIC_LITERAL: {
            NumericLiteralNode *num_node = (NumericLiteralNode *)node->data;
            return generate_numeric_literal(num_node, Context);
        }
        case NODE_IDENTIFIER: {
            IdentifierNode *id_node = (IdentifierNode *)node->data;
            return generate_identifier(id_node);
        }
        case NODE_BINARY_EXPR: {
            BinaryExprNode *bin_expr = (BinaryExprNode *)node->data;
            return generate_binary_expr(bin_expr, Context, Builder);
        }
        case NODE_UNARY_MINUS: {
            UnaryMinusExpr *unary_node = (UnaryMinusExpr *)node->data;
            return generate_unary_minus(unary_node, Context, Builder);
        }
        case NODE_LOGICAL_NOT: {
            LogicalNotExpr *not_node = (LogicalNotExpr *)node->data;
            return generate_logical_not(not_node, Context, Builder);
        }
        case NODE_UNARY_BITWISE_NOT: {
            UnaryBitwiseNotExpr *bitwise_not_node = (UnaryBitwiseNotExpr *)node->data;
            return generate_unary_bitwise_not(bitwise_not_node, Context, Builder);
        }
        case NODE_PRE_INCREMENT: {
            PreIncrementExpr *inc_node = (PreIncrementExpr *)node->data;
            return generate_pre_increment(inc_node, Context, Builder);
        }
        case NODE_PRE_DECREMENT: {
            PreDecrementExpr *dec_node = (PreDecrementExpr *)node->data;
            return generate_pre_decrement(dec_node, Context, Builder);
        }
        case NODE_ASSIGNMENT: {
            AssignmentNode *assignNode = (AssignmentNode *)node->data;
            return generate_assignment_expr(assignNode, Context, Builder);
        }
        default: {
            throw std::runtime_error("Unsupported expression type");
        }
    }
}
