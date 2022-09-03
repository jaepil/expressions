//
// Expressions
//
// Copyright (c) 2022 Jaepil Jeong <jaepil@appspand.com>
//

#include <expressions/interpreter/ast_interpreter.hpp>

#include <expressions/common/enumerate.hpp>
#include <expressions/common/visitor.hpp>
#include <expressions/exception/throw_exception.hpp>

#include <cmath>
#include <optional>


namespace expressions::interpreter {

void __builtin_print(const std::vector<BoxedValue>& args) {
    auto printer = SelfVisitableVisitor {
        [](auto&&, interpreter::Null) {
            return std::string {"null"};
        },
        [](auto&&, bool value) {
            return fmt::format("{}", value);
        },
        [](auto&&, int64_t value) {
            return fmt::format("{}", value);
        },
        [](auto&&, uint64_t value) {
            return fmt::format("{}", value);
        },
        [](auto&&, double value) {
            return fmt::format("{}", value);
        },
        [](auto&&, const interpreter::Name& value) {
            return value.value;
        },
        [](auto&&, const interpreter::String& value) {
            return fmt::format("{}", value.value);
        },
        [](auto&&, const interpreter::Date& value) {
            return fmt::format("{:04}-{:02}-{:02}", value.year, value.month,
                               value.day);
        },
        [](auto&&, const interpreter::DateRange& value) {
            return fmt::format("{:04}-{:02}-{:02}-{:04}-{:02}-{:02}",
                               value.begin.year, value.begin.month,
                               value.begin.day, value.end.year, value.end.month,
                               value.end.day);
        },
        [](auto&& self, const interpreter::Tuple<interpreter::BoxedValue>& c) {
            auto buffer = fmt::memory_buffer {};
            auto output = std::back_inserter(buffer);
            fmt::format_to(output, "(");
            for (const auto& [index, item] : enumerate(c)) {
                if (index > 0) {
                    fmt::format_to(output, ", ");
                }
                fmt::format_to(output, "{}", self.visit(item));
            }
            fmt::format_to(output, ")");

            return fmt::to_string(buffer);
        },
        [](auto&& self, const interpreter::Vector<interpreter::BoxedValue>& c) {
            auto buffer = fmt::memory_buffer {};
            auto output = std::back_inserter(buffer);
            fmt::format_to(output, "[");
            for (const auto& [index, item] : enumerate(c)) {
                if (index > 0) {
                    fmt::format_to(output, ", ");
                }
                fmt::format_to(output, "{}", self.visit(item));
            }
            fmt::format_to(output, "]");

            return fmt::to_string(buffer);
        },
        [](auto&& self, const interpreter::Set<interpreter::BoxedValue>& c) {
            auto buffer = fmt::memory_buffer {};
            auto output = std::back_inserter(buffer);
            fmt::format_to(output, "<<?");
            for (const auto& [index, item] : enumerate(c)) {
                if (index > 0) {
                    fmt::format_to(output, ", ");
                }
                fmt::format_to(output, "{}", self.visit(item));
            }
            fmt::format_to(output, "?>>");

            return fmt::to_string(buffer);
        },
        [](auto&&, const interpreter::Map<interpreter::BoxedValue,
                                          interpreter::BoxedValue>& c) {
            (void)c;
            return std::string {};
        },
        [](auto&&, const auto& value) {
            auto name
                = boost::typeindex::type_id<decltype(value)>().pretty_name();
            return fmt::format("{}", name);
        },
    };

    auto output = std::vector<std::string> {};
    output.reserve(args.size());
    for (const auto& arg : args) {
        output.emplace_back(printer.visit(arg));
    }
    fmt::print("{}\n", fmt::join(output, " "));
}

auto ASTInterpreter::operator()(const ast::MonoState& node) const
    -> ReturnType {
    (void)node;

    THROW_EXCEPTION(ProgramTerminated {"Program got into unexpected state."});
}

auto ASTInterpreter::operator()(const ast::Null& node) const -> ReturnType {
    (void)node;

    return Null {};
}

auto ASTInterpreter::operator()(bool value) const -> ReturnType {
    return value;
}

auto ASTInterpreter::operator()(int64_t value) const -> ReturnType {
    return value;
}

auto ASTInterpreter::operator()(uint64_t value) const -> ReturnType {
    return value;
}

auto ASTInterpreter::operator()(double value) const -> ReturnType {
    return value;
}

auto ASTInterpreter::operator()(const ast::Name& node) const -> ReturnType {
    auto it = std::optional<Context::iterator> {};
    if (stack_.empty()) {
        it = context_.find(node.value);
        if (it.value() == context_.end()) {
            fmt::print("Symbol {} not found.\n", node.value);
            return Name {node.value};
        }
    } else {
        auto& frame = stack_.back();
        it = frame.find(node.value);
        if (it.value() == frame.end()) {
            it = context_.find(node.value);
            if (it.value() == context_.end()) {
                fmt::print("Symbol {} not found.\n", node.value);
                return Name {node.value};
            }
        }
    }

    if (ast::holds_alternative<Code>(it.value()->second)) {
        auto code = ast::get<Code>(it.value()->second);
        return visit_(code.code);
    } else {
        return it.value()->second;
    }
}

auto ASTInterpreter::operator()(const ast::String& node) const -> ReturnType {
    auto it = std::optional<Context::iterator> {};
    if (stack_.empty()) {
        it = context_.find(node.value);
        if (it.value() == context_.end()) {
            return Name {node.value};
        }
    } else {
        auto& frame = stack_.back();
        it = frame.find(node.value);
        if (it.value() == frame.end()) {
            it = context_.find(node.value);
            if (it.value() == context_.end()) {
                return Name {node.value};
            }
        }
    }

    if (ast::holds_alternative<Code>(it.value()->second)) {
        auto code = ast::get<Code>(it.value()->second);
        return visit_(code.code);
    } else {
        return it.value()->second;
    }
}

auto ASTInterpreter::operator()(const ast::QuotedString& node) const
    -> ReturnType {
    return String {node.value};
}

auto ASTInterpreter::operator()(const ast::Date& node) const -> ReturnType {
    // return String {node.to_string()};
    return Date {node.year, node.month, node.day};
}

auto ASTInterpreter::operator()(const ast::DateRange& node) const
    -> ReturnType {
    // return fmt::format("{}-{}", node.begin.to_string(),
    // node.end.to_string());
    return DateRange {
        {node.begin.year, node.begin.month, node.begin.day},
        {node.end.year, node.end.month, node.end.day},
    };
}

auto ASTInterpreter::operator()(const ast::Tuple& node) const -> ReturnType {
    auto values = Tuple<BoxedValue> {};
    for (const auto& value : node.values) {
        values.emplace_back(visit_(value));
    }

    return BoxedValue {std::move(values)};
}

auto ASTInterpreter::operator()(const ast::List& node) const -> ReturnType {
    auto values = Vector<BoxedValue> {};
    for (const auto& value : node.values) {
        values.emplace_back(visit_(value));
    }

    return BoxedValue {std::move(values)};
}

auto ASTInterpreter::operator()(const ast::Dict& node) const -> ReturnType {
    auto items = Map<BoxedValue, BoxedValue> {};
    for (const auto& item : node.items) {
        items.emplace(visit_(item.key), visit_(item.value));
    }

    return BoxedValue {std::move(items)};
}

auto ASTInterpreter::operator()(const ast::Set& node) const -> ReturnType {
    auto values = Set<BoxedValue> {};
    for (const auto& value : node.values) {
        values.emplace(visit_(value));
    }

    return BoxedValue {std::move(values)};
}

auto ASTInterpreter::operator()(const ast::CompareOp& node) const
    -> ReturnType {
    auto left = visit_(node.first);
    auto rest = std::vector<ast::CompareOpOperand> {};
    rest.reserve(node.rest.size());

    auto generic_compare = [](const auto& a, const auto& b, auto&& op) -> bool {
        bool result = false;

        if (ast::holds_any_of<int64_t, uint64_t, double>(a)
            && ast::holds_any_of<int64_t, uint64_t, double>(b)) {
            if (boost::get<int64_t>(&a)) {
                auto* lhs = boost::get<int64_t>(&a);
                if (auto* rhs_i64 = boost::get<int64_t>(&b)) {
                    result = op(*lhs, *rhs_i64);
                } else if (auto* rhs_u64 = boost::get<uint64_t>(&b)) {
                    result = op(static_cast<uint64_t>(*lhs), *rhs_u64);
                } else if (auto* rhs_double = boost::get<double>(&b)) {
                    result = op(*lhs, *rhs_double);
                }
            } else if (boost::get<uint64_t>(&a)) {
                auto* lhs = boost::get<uint64_t>(&a);
                if (auto* rhs_i64 = boost::get<int64_t>(&b)) {
                    result = op(*lhs, static_cast<uint64_t>(*rhs_i64));
                } else if (auto* rhs_u64 = boost::get<uint64_t>(&b)) {
                    result = op(*lhs, *rhs_u64);
                } else if (auto* rhs_double = boost::get<double>(&b)) {
                    result = op(*lhs, *rhs_double);
                }
            } else if (boost::get<double>(&a)) {
                auto* lhs = boost::get<double>(&a);
                if (auto* rhs_i64 = boost::get<int64_t>(&b)) {
                    result = op(*lhs, *rhs_i64);
                } else if (auto* rhs_u64 = boost::get<uint64_t>(&b)) {
                    result = op(*lhs, *rhs_u64);
                } else if (auto* rhs_double = boost::get<double>(&b)) {
                    result = op(*lhs, *rhs_double);
                }
            }
        } else if (ast::holds_alternative<String>(a)
                   && ast::holds_alternative<String>(b)) {
            auto* lhs = boost::get<String>(&a);
            auto* rhs = boost::get<String>(&b);
            return op(lhs->value, rhs->value);
        } else {
            result = op(a, b);
        }

        return result;
    };

    for (const auto& operand : node.rest) {
        auto right = visit_(operand.operand);

        bool result = false;
        switch (operand.op) {
            case ast::CompareOpType::kNone: {
                return {};
            }
            case ast::CompareOpType::kEQ: {
                result = generic_compare(left, right,
                                         [](const auto& a, const auto& b) {
                                             return a == b;
                                         });
                break;
            }
            case ast::CompareOpType::kNEQ: {
                result = generic_compare(left, right,
                                         [](const auto& a, const auto& b) {
                                             return a != b;
                                         });
                break;
            }
            case ast::CompareOpType::kLT: {
                result = generic_compare(left, right,
                                         [](const auto& a, const auto& b) {
                                             return a < b;
                                         });
                break;
            }
            case ast::CompareOpType::kLTE: {
                result = generic_compare(left, right,
                                         [](const auto& a, const auto& b) {
                                             return a <= b;
                                         });
                break;
            }
            case ast::CompareOpType::kGT: {
                result = generic_compare(left, right,
                                         [](const auto& a, const auto& b) {
                                             return a > b;
                                         });
                break;
            }
            case ast::CompareOpType::kGTE: {
                result = generic_compare(left, right,
                                         [](const auto& a, const auto& b) {
                                             return a >= b;
                                         });
                break;
            }
            case ast::CompareOpType::kIn: {
                auto visitor = Visitor {
                    [&](const Tuple<BoxedValue>& t) -> bool {
                        return std::find(t.begin(), t.end(), left) != t.end();
                    },
                    [&](const Vector<BoxedValue>& v) -> bool {
                        return std::find(v.begin(), v.end(), left) != v.end();
                    },
                    [&](const Set<BoxedValue>& s) -> bool {
                        return s.contains(left);
                    },
                    [&](const Map<BoxedValue, BoxedValue>& m) -> bool {
                        return m.contains(left);
                    },
                    [&](const BoxedValue&) -> bool {
                        return false;
                    },
                };
                result = boost::apply_visitor(visitor, right);
                break;
            }
            case ast::CompareOpType::kNotIn: {
                auto visitor = Visitor {
                    [&](const Tuple<BoxedValue>& t) -> bool {
                        return std::find(t.begin(), t.end(), left) == t.end();
                    },
                    [&](const Vector<BoxedValue>& v) -> bool {
                        return std::find(v.begin(), v.end(), left) == v.end();
                    },
                    [&](const Set<BoxedValue>& s) -> bool {
                        return !s.contains(left);
                    },
                    [&](const Map<BoxedValue, BoxedValue>& m) -> bool {
                        return !m.contains(left);
                    },
                    [&](const BoxedValue&) -> bool {
                        return false;
                    },
                };
                result = boost::apply_visitor(visitor, right);
                break;
            }
        }

        if (!result) {
            return false;
        }

        left = std::move(right);
    }

    return true;
}

auto ASTInterpreter::operator()(const ast::BinOp& node) const -> ReturnType {
    auto left = visit_(node.left);
    auto right = visit_(node.right);

    return execute_bin_op_(node.op, std::move(left), std::move(right));
}

auto ASTInterpreter::operator()(const ast::Call& node) const -> ReturnType {
    if (node.name.value == "print") {
        auto args = std::vector<ReturnType> {};
        args.reserve(node.args.size());
        for (const auto& arg : node.args) {
            args.emplace_back(visit_(arg));
        }
        __builtin_print(args);

        return Null {};
    }

    auto it = std::optional<Context::iterator> {};
    if (stack_.empty()) {
        it = context_.find(node.name.value);
        if (it.value() == context_.end()) {
            THROW_EXCEPTION(std::runtime_error(
                fmt::format("Object not found: {}", node.name.value)));
        }
    } else {
        auto& frame = stack_.back();
        it = frame.find(node.name.value);
        if (it.value() == frame.end()) {
            it = context_.find(node.name.value);
            if (it.value() == context_.end()) {
                THROW_EXCEPTION(std::runtime_error(
                    fmt::format("Object not found: {}", node.name.value)));
            }
        }
    }

    std::vector<ast::Value>* params = nullptr;
    ast::Value* body = nullptr;
    auto& object = it.value()->second;
    auto type_name = std::string {};
    if (auto* lambda = ast::get_if<Lambda>(&object)) {
        params = &lambda->params;
        body = &lambda->body;
        type_name
            = boost::typeindex::type_id<decltype(*lambda)>().pretty_name();
    } else if (auto* func = ast::get_if<Function>(&object)) {
        params = &func->params;
        body = &func->body;
        type_name = boost::typeindex::type_id<decltype(*func)>().pretty_name();
    } else {
        type_name = boost::typeindex::type_id<decltype(object)>().pretty_name();
        THROW_EXCEPTION(std::runtime_error(
            fmt::format("Object '{}' references to '{}' is not callable.",
                        node.name.value, type_name)));
    }

    if (params->size() != node.args.size()) {
        THROW_EXCEPTION(std::runtime_error(fmt::format(
            "Failed to call object '{}' references to '{}'. It "
            "takes {} arguments "
            "but {} were given.",
            node.name.value, type_name, params->size(), node.args.size())));
    }

    auto locals = Context {};
    for (const auto& [index, argument] : enumerate(node.args)) {
        auto name = std::string {};
        if (auto* parg_def = ast::get_if<ast::Argument>(&params->at(index))) {
            name = ast::get<ast::Name>(parg_def->arg).value;
        } else if (auto* kwarg_def
                   = ast::get_if<ast::KeywordArgument>(&params->at(index))) {
            name = kwarg_def->name.value;
        } else {
            THROW_EXCEPTION(std::runtime_error(
                fmt::format("Failed to call object '{}'. Invalid argument "
                            "type at position {}.",
                            node.name.value, index)));
        }
        locals.insert_or_assign(name, visit_(argument));
    }

    stack_.emplace_back(std::move(locals));
    try {
        visit_(*body);
    } catch (const ReturnException&) {
    }
    stack_.pop_back();

    return return_value_and_reset_();
}

auto ASTInterpreter::operator()(const ast::Argument& node) const -> ReturnType {
    return visit_(node.arg);
}

auto ASTInterpreter::operator()(const ast::KeywordArgument& node) const
    -> ReturnType {
    return visit_(node.arg);
}

auto ASTInterpreter::operator()(const ast::UnaryOp& node) const -> ReturnType {
    auto value = visit_(node.operand);

    auto generic_unary_op
        = [](auto type, const auto& operand, auto&& op) -> BoxedValue {
        if (ast::holds_any_of<int64_t, uint64_t, double>(operand)) {
            if (boost::get<int64_t>(&operand)) {
                return op(boost::get<int64_t>(operand));
            } else if (boost::get<uint64_t>(&operand)) {
                return op(boost::get<uint64_t>(operand));
            } else if (boost::get<double>(&operand)) {
                return op(boost::get<double>(operand));
            }
        } else if (type == ast::BoolOpType::kNot) {
            if (boost::get<bool>(&operand)) {
                return !boost::get<bool>(operand);
            } else if (boost::get<Null>(&operand)) {
                return true;
            } else if (auto* str = boost::get<String>(&operand)) {
                return str->value.empty();
            }
        }

        // TODO: throw type error
        return {};
    };

    switch (node.op) {
        case ast::BoolOpType::kPlus: {
            return generic_unary_op(node.op, value,
                                    [](const auto& operand) -> BoxedValue {
                                        return operand;
                                    });
        }
        case ast::BoolOpType::kMinus: {
            return generic_unary_op(node.op, value,
                                    [](const auto& operand) -> BoxedValue {
                                        return -operand;
                                    });
        }
        case ast::BoolOpType::kNot: {
            return generic_unary_op(node.op, value,
                                    [](const auto& operand) -> BoxedValue {
                                        return !operand;
                                    });
        }
        case ast::BoolOpType::kDefault:
        case ast::BoolOpType::kAnd:
        case ast::BoolOpType::kOr: {
            break;
        }
    }

    return {};
}

auto ASTInterpreter::operator()(const ast::BoolOp& node) const -> ReturnType {
    bool flag = false;
    if (node.op == ast::BoolOpType::kAnd) {
        flag = true;
    }

    for (const auto& operand : node.operands) {
        auto result = visit_(operand);
        switch (node.op) {
            case ast::BoolOpType::kAnd: {
                if (!check_branch_condition_(result)) {
                    return false;
                }
                break;
            }
            case ast::BoolOpType::kOr: {
                if (check_branch_condition_(result)) {
                    return true;
                }
                break;
            }
            case ast::BoolOpType::kDefault:
            case ast::BoolOpType::kNot:
            case ast::BoolOpType::kPlus:
            case ast::BoolOpType::kMinus: {
                break;
            }
        }
    }

    return flag;
}

auto ASTInterpreter::operator()(const ast::Lambda& node) const -> ReturnType {
    return visit_(node.expr);
}

auto ASTInterpreter::operator()(const ast::Expression& node) const
    -> ReturnType {
    return visit_(node.expr);
}

auto ASTInterpreter::operator()(const ast::AssignStatement& node) const
    -> ReturnType {
    if (!ast::holds_alternative<ast::Name>(node.target)) {
        // TODO: throw error
        return {};
    }
    auto name = ast::get<ast::Name>(node.target);
    if (const auto* lambda = ast::get_if<ast::Lambda>(&node.expr)) {
        auto value = Lambda {lambda->params, lambda->expr};
        context_.insert_or_assign(std::move(name.value), std::move(value));
    } else if (const auto* func = ast::get_if<ast::FunctionDef>(&node.expr)) {
        auto value
            = Function {Name {func->name.value}, func->params, func->body};
        context_.insert_or_assign(std::move(name.value), std::move(value));
    } else {
        auto value = visit_(node.expr);
        context_.insert_or_assign(std::move(name.value), std::move(value));
    }

    return {};
}

auto ASTInterpreter::operator()(const ast::LazyAssignStatement& node) const
    -> ReturnType {
    if (!ast::holds_alternative<ast::Name>(node.target)) {
        // TODO: throw error
        return {};
    }
    auto name = ast::get<ast::Name>(node.target);
    auto value = Code {node.expr};
    context_.insert_or_assign(std::move(name.value), std::move(value));

    return {};
}

auto ASTInterpreter::operator()(const ast::AugAssignStatement& node) const
    -> ReturnType {
    if (!ast::holds_alternative<ast::Name>(node.target)) {
        // TODO: throw error
        return {};
    }

    auto left = visit_(node.target);
    if (ast::holds_alternative<Name>(left)) {
        // TODO: throw error
        return {};
    }
    auto right = visit_(node.expr);
    auto value = execute_bin_op_(node.op, std::move(left), std::move(right));

    auto name = ast::get<ast::Name>(node.target).value;
    context_.insert_or_assign(std::move(name), std::move(value));

    return {};
}

auto ASTInterpreter::operator()(const ast::ReturnStatement& node) const
    -> ReturnType {
    return_value_ = visit_(node.expr);

    throw ReturnException {};
    // return return_value_and_reset_();
}

auto ASTInterpreter::operator()(const ast::StatementList& node) const
    -> ReturnType {
    for (const auto& n : node.stmts) {
        visit_(n);
    }

    // return return_value_and_reset_();
    return Null {};
}

auto ASTInterpreter::operator()(const ast::FunctionDef& node) const
    -> ReturnType {
    auto func = Function {{node.name.value}, node.params, node.body};
    context_.insert_or_assign(node.name.value, std::move(func));

    return Null {};
}

auto ASTInterpreter::operator()(const ast::IfStatement& node) const
    -> ReturnType {
    auto condition = visit_(node.condition);
    auto flag = check_branch_condition_(condition);

    try {
        if (flag) {
            return visit_(node.body);
        } else if (!ast::holds_alternative<ast::MonoState>(node.or_else)) {
            return visit_(node.or_else);
        }
    } catch (const ReturnException&) {
        return return_value_and_reset_();
    }

    return return_value_and_reset_();
}

auto ASTInterpreter::operator()(const ast::ForStatement& node) const
    -> ReturnType {
    bool run_else_block = true;
    visit_(node.init);
    while (true) {
        auto condition = visit_(node.condition);
        if (!check_branch_condition_(condition)) {
            break;
        }
        try {
            visit_(node.body);
        } catch (const BreakException&) {
            run_else_block = false;
            break;
        } catch (const ContinueException&) {
        } catch (const ReturnException&) {
            run_else_block = false;
            break;
        }
        visit_(node.iter);
    }

    if (run_else_block
        && !ast::holds_alternative<ast::MonoState>(node.or_else)) {
        visit_(node.or_else);
    }

    return return_value_and_reset_();
}

auto ASTInterpreter::operator()(const ast::RangeBasedForStatement& node) const
    -> ReturnType {
    (void)node;

    return {};
}

auto ASTInterpreter::operator()(const ast::WhileStatement& node) const
    -> ReturnType {
    bool run_else_block = true;
    while (true) {
        auto condition = visit_(node.condition);
        if (!check_branch_condition_(condition)) {
            break;
        }
        try {
            visit_(node.body);
        } catch (const BreakException&) {
            run_else_block = false;
            break;
        } catch (const ContinueException&) {
            continue;
        } catch (const ReturnException&) {
            run_else_block = false;
            break;
        }
    }

    if (run_else_block
        && !ast::holds_alternative<ast::MonoState>(node.or_else)) {
        visit_(node.or_else);
    }

    return return_value_and_reset_();
}

auto ASTInterpreter::operator()(const ast::Pass& node) const -> ReturnType {
    (void)node;

    return Null {};
}

auto ASTInterpreter::operator()(const ast::Break& node) const -> ReturnType {
    (void)node;

    throw BreakException {};
}

auto ASTInterpreter::operator()(const ast::Continue& node) const -> ReturnType {
    (void)node;

    throw ContinueException {};
}

auto ASTInterpreter::operator()(const ast::PackageName& node) const
    -> ReturnType {
    (void)node;

    return Null {};
}

auto ASTInterpreter::operator()(const ast::Entry& node) const -> ReturnType {
    try {
        return visit_(node.node);
    } catch (const BreakException&) {
    } catch (const ContinueException&) {
    } catch (const ReturnException&) {
    }

    return return_value_and_reset_();
}

auto ASTInterpreter::execute_bin_op_(ast::BinOpType op, BoxedValue&& left,
                                     BoxedValue&& right) const -> ReturnType {
    if (!ast::holds_any_of<int64_t, uint64_t, double, String>(left)) {
        fmt::print("--------1 {}, {}\n", static_cast<int32_t>(op),
                   left.which());
        return {};
    }
    if (!ast::holds_any_of<int64_t, uint64_t, double, String>(right)) {
        fmt::print("--------2 {}, {}\n", static_cast<int32_t>(op),
                   right.which());
        return {};
    }

    auto generic_bin_op = [](auto type, const auto& a, const auto& b,
                             auto&& op_func) -> BoxedValue {
        if (ast::holds_any_of<int64_t, uint64_t, double>(a)
            && ast::holds_any_of<int64_t, uint64_t, double>(b)) {
            if (boost::get<int64_t>(&a)) {
                auto* lhs = boost::get<int64_t>(&a);
                if (auto* rhs_i64 = boost::get<int64_t>(&b)) {
                    return op_func(*lhs, *rhs_i64);
                } else if (auto* rhs_u64 = boost::get<uint64_t>(&b)) {
                    return op_func(*lhs, *rhs_u64);
                } else if (auto* rhs_double = boost::get<double>(&b)) {
                    return op_func(*lhs, *rhs_double);
                }
            } else if (boost::get<uint64_t>(&a)) {
                auto* lhs = boost::get<uint64_t>(&a);
                if (auto* rhs_i64 = boost::get<int64_t>(&b)) {
                    return op_func(*lhs, *rhs_i64);
                } else if (auto* rhs_u64 = boost::get<uint64_t>(&b)) {
                    return op_func(*lhs, *rhs_u64);
                } else if (auto* rhs_double = boost::get<double>(&b)) {
                    return op_func(*lhs, *rhs_double);
                }
            } else if (boost::get<double>(&a)) {
                auto* lhs = boost::get<double>(&a);
                if (auto* rhs_i64 = boost::get<int64_t>(&b)) {
                    return op_func(*lhs, *rhs_i64);
                } else if (auto* rhs_u64 = boost::get<uint64_t>(&b)) {
                    return op_func(*lhs, *rhs_u64);
                } else if (auto* rhs_double = boost::get<double>(&b)) {
                    return op_func(*lhs, *rhs_double);
                }
            }
        } else if (type == ast::BinOpType::kAdd
                   && ast::holds_alternative<String>(a)
                   && ast::holds_alternative<String>(b)) {
            auto* lhs = boost::get<String>(&a);
            auto* rhs = boost::get<String>(&b);
            return String {lhs->value + rhs->value};
        }

        return {};
    };

    switch (op) {
        case ast::BinOpType::kNone: {
            return {};
        }
        case ast::BinOpType::kAdd: {
            return generic_bin_op(op, left, right, [](auto a, auto b) {
                return a + b;
            });
        }
        case ast::BinOpType::kSub: {
            return generic_bin_op(op, left, right, [](auto a, auto b) {
                return a - b;
            });
        }
        case ast::BinOpType::kMult: {
            return generic_bin_op(op, left, right, [](auto a, auto b) {
                return a * b;
            });
        }
        case ast::BinOpType::kTrueDiv: {
            return generic_bin_op(op, left, right, [](auto a, auto b) {
                return a / b;
            });
        }
        case ast::BinOpType::kFloorDiv: {
            return generic_bin_op(op, left, right, [](auto a, auto b) {
                return static_cast<int64_t>(a / b);
            });
        }
        case ast::BinOpType::kMod: {
            return generic_bin_op(op, left, right, [](auto a, auto b) {
                if constexpr (
                    std::same_as<decltype(a),
                                 double> || std::same_as<decltype(b), double>) {
                    return std::fmod(a, b);
                } else {
                    return a % b;
                }
            });
        }
        case ast::BinOpType::kPow: {
            return generic_bin_op(op, left, right,
                                  [](const auto& a, const auto& b) {
                                      return std::pow(a, b);
                                  });
        }
    }

    return {};
}

bool ASTInterpreter::check_branch_condition_(const BoxedValue& value) const {
    bool flag = true;
    if (ast::get_if<Null>(&value)) {
        flag = false;
    } else if (const auto* value_bool = ast::get_if<bool>(&value)) {
        flag = *value_bool;
    } else if (const auto* value_i64 = ast::get_if<int64_t>(&value)) {
        flag = *value_i64 != 0;
    } else if (const auto* value_u64 = ast::get_if<uint64_t>(&value)) {
        flag = *value_u64 != 0;
    } else if (const auto* value_double = ast::get_if<double>(&value)) {
        flag = *value_double != 0.f;
    } else if (const auto* value_str = ast::get_if<String>(&value)) {
        flag = !value_str->value.empty();
    } else {
        // TODO:
    }

    return flag;
}

BoxedValue ASTInterpreter::return_value_and_reset_() const {
    auto result = std::move(return_value_);
    return_value_ = Null {};

    return result;
}

}    // namespace expressions::interpreter
