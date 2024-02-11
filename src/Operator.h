#pragma once

#include <climits>

#include "Optional.h"

class Operator {
   public:
    virtual char symbol() const = 0;
    virtual int precedence() const = 0;
    virtual Optional<int32_t> calculate(int32_t a, int32_t b) const = 0;
    virtual bool startEvaluating() const { return false; }
    virtual ~Operator() = default;  // Important: Virtual Destructor!
};

#define DEFINE_OPERATOR(NAME, SYMBOL, PRECEDENCE, OPERATION)               \
    class NAME : public Operator {                                         \
       public:                                                             \
        char symbol() const override { return SYMBOL; }                    \
        int precedence() const override { return PRECEDENCE; }             \
        Optional<int32_t> calculate(int32_t a, int32_t b) const override { \
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
    int precedence() const override { return 5; }
    Optional<int32_t> calculate(int32_t a, int32_t b) const override {
        if (b == 0) return Optional<int32_t>();
        return Optional<int32_t>(a / b);
    }
};

class ModOperator : public Operator {
   public:
    char symbol() const override { return '%'; }
    int precedence() const override { return 5; }
    Optional<int32_t> calculate(int32_t a, int32_t b) const override {
        if (b == 0) return Optional<int32_t>();
        return Optional<int32_t>(a % b);
    }
};

class Solve : public Operator {
   public:
    char symbol() const override { return '\r'; }
    int precedence() const override { return INT_MAX; }
    Optional<int32_t> calculate(int32_t a, int32_t b) const override {
        return Optional<int32_t>();
    }
    bool startEvaluating() const { return true; }
};