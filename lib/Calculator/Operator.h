#pragma once

#include "Optional.h"
#include <vector>

class Operator {
   public:
      enum ReduceMode {
          Stop,
          Eval,
          DropAndStop,
      };
    virtual char symbol() const = 0;
    virtual uint8_t precedence() const = 0;
    virtual uint8_t operands() const { return 2; }
    virtual bool followsNumber() const { return true; }
    virtual Optional<int32_t> calculate(std::vector<int32_t> args) const = 0;
    virtual ReduceMode reduce(Operator* head) const {
      if (head->precedence() <= precedence())
        return ReduceMode::Eval;
      else
        return ReduceMode::Stop;
    }
    virtual ~Operator() = default;  // Important: Virtual Destructor!
};

#define DEFINE_OPERATOR(NAME, SYMBOL, PRECEDENCE, OPERATION)               \
    class NAME : public Operator {                                         \
       public:                                                             \
        char symbol() const override { return SYMBOL; }                    \
        uint8_t precedence() const override { return PRECEDENCE; }         \
        Optional<int32_t> calculate(std::vector<int32_t> args) const override { \
            int32_t a = args[0], b = args[1];                              \
            return Optional<int32_t>(OPERATION);                           \
        }                                                                  \
    };

DEFINE_OPERATOR(MulOperator, '*', 5, a* b)
DEFINE_OPERATOR(AddOperator, '+', 6, a + b)
DEFINE_OPERATOR(SubOperator, '-', 6, a - b)
DEFINE_OPERATOR(ShlOperator, '<', 7, a << b)
DEFINE_OPERATOR(ShrOperator, '>', 7, a >> b)
DEFINE_OPERATOR(AndOperator, '&', 11, a& b)
DEFINE_OPERATOR(XorOperator, '^', 12, a ^ b)
DEFINE_OPERATOR(OrOperator, '|', 13, a | b)

class DivOperator : public Operator {
   public:
    char symbol() const override { return '/'; }
    uint8_t precedence() const override { return 5; }
    Optional<int32_t> calculate(std::vector<int32_t> args) const override {
        if (args[1] == 0) return Optional<int32_t>();
        return Optional<int32_t>(args[0] / args[1]);
    }
};

class ModOperator : public Operator {
   public:
    char symbol() const override { return '%'; }
    uint8_t precedence() const override { return 5; }
    Optional<int32_t> calculate(std::vector<int32_t> args) const override {
        if (args[1] == 0) return Optional<int32_t>();
        return Optional<int32_t>(args[0] % args[1]);
    }
};

class Solve : public Operator {
   public:
    char symbol() const override { return '\r'; }
    uint8_t precedence() const override { return UINT8_MAX; }
    uint8_t operands() const override { return 0; }
    bool followsNumber() const override { return false; }
    Optional<int32_t> calculate(std::vector<int32_t> args) const override {
        return Optional<int32_t>();
    }
    ReduceMode reduce(Operator* head) const override { return ReduceMode::Eval; }
    bool startEvaluating() const { return true; }
};

class StartParentheses : public Operator {
    char symbol() const override { return '('; }
    uint8_t precedence() const override { return 200; }
    uint8_t operands() const override { return 0; }
    bool followsNumber() const override { return false; }
    Optional<int32_t> calculate(std::vector<int32_t> args) const override {
        return Optional<int32_t>();
    }
    ReduceMode reduce(Operator* head) const override { return ReduceMode::Stop; }
    bool startEvaluating() const { return true; }
};

class EndParentheses : public Operator {
    char symbol() const override { return ')'; }
    uint8_t precedence() const override { return UINT8_MAX; }
    uint8_t operands() const override { return 0; }
    Optional<int32_t> calculate(std::vector<int32_t> args) const override {
        return Optional<int32_t>();
    }
    ReduceMode reduce(Operator* head) const override {
      if (head->symbol() != '(')
        return ReduceMode::Eval;
      else
        return ReduceMode::DropAndStop;
    }
    bool startEvaluating() const { return true; }
};