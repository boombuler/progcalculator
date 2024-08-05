#include "Calculator.h"
#include <algorithm>
#include <sstream>
#include "Operator.h"

// Initialize static const member
const std::vector<Operator*> Calculator::_operators = {
    new MulOperator(), new DivOperator(),
    new ModOperator(), new AddOperator(),
    new SubOperator(), new ShlOperator(),
    new ShrOperator(), new AndOperator(),
    new XorOperator(), new OrOperator(),
    new Solve()};

void Calculator::addDigit(int n) {
    if (n >= static_cast<int>(_mode))
        return;  // Ensure the base is contain the value...

    if (_newNumber) {
        _operant = n;
        _newNumber = false;
    } else {
        switch (_mode) {
            case Mode::Bin:
                _operant <<= 1;
                break;
            case Mode::Oct:
                _operant <<= 3;
                break;
            case Mode::Hex:
                _operant <<= 4;
                break;
            default:
                _operant *= 10;
                break;
        }
        _operant += n;
    }
    displayNumber();
}

void Calculator::removeDigit() {
    if (_newNumber)
        _operant = 0;
    else {
        switch (_mode) {
            case Mode::Bin:
                _operant >>= 1;
                break;
            case Mode::Oct:
                _operant >>= 3;
                break;
            case Mode::Hex:
                _operant >>= 4;
                break;
            default:
                _operant /= 10;
                break;
        }
    }
    displayNumber();
}

std::string toBinary(uint32_t n) {
    if (n == 0) return "0";

    std::string s;
    while (n != 0) {
        s += (n % 2 == 0) ? '0' : '1';
        n = n / 2;
    }
    std::reverse(s.begin(), s.end());
    return s;
}

void Calculator::displayNumber() {
    std::stringstream ss;
    switch (_mode) {
        case Mode::Bin:
            ss << toBinary(_operant);
            break;
        case Mode::Oct:
            ss << std::oct << _operant;
            break;
        case Mode::Hex:
            ss << std::hex << _operant;
            break;
        default:
            ss << std::dec << _operant;
            break;
    }

    display = ss.str();
}

void Calculator::error() {
    _pending.clear();
    _operant = 0;
    _newNumber = true;
    display = "Error";
}

void Calculator::input(char c) {
    if (c >= '0' && c <= '9')
        addDigit(c - '0');
    else if (c >= 'a' && c <= 'f')
        addDigit(c - 'a' + 10);
    else if (c == '\b')
        removeDigit();
    else {
        for (const auto &op : _operators) {
            if (op->symbol() == c) {
                push(*op);
                break;
            }
        }
    }
}

void Calculator::push(Operator &op) {
    _pending.push_back({_operant, op});
    _newNumber = true;
    if (op.startEvaluating()) {
        solve();
        displayNumber();
    }
}

void Calculator::solve() {
    while (_pending.size() > 1) {
        auto argA =
            std::min_element(_pending.begin(), _pending.end(),
                             [](const Operation a, const Operation b) {
                                 return a.op.precedence() < b.op.precedence();
                             });
        auto argB = argA;
        ++argB;
        auto result = argA->op.calculate(argA->value, argB->value);
        if (result.has_value()) {
            argB->value = result.value();
            _pending.erase(argA);
        } else {
            error();
            return;
        }
    }
    _operant = _pending.begin()->value;
    _pending.clear();
}
