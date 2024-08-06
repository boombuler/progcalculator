#include "Calculator.h"
#include <algorithm>
#include <sstream>
#include <vector>
#include "Operator.h"

// Initialize static const member
const std::vector<Operator *> Calculator::_knownOperators = {
    new MulOperator(), new DivOperator(),
    new ModOperator(), new AddOperator(),
    new SubOperator(), new ShlOperator(),
    new ShrOperator(), new AndOperator(),
    new XorOperator(),      new OrOperator(),
    new StartParentheses(), new EndParentheses(),
    new Solve()};

void Calculator::addDigit(int n) {
    if (n >= static_cast<int>(_mode))
        return;  // Ensure the base is contain the value...

    if (_newNumber) {
        _pending = n;
        _newNumber = false;
    } else {
        switch (_mode) {
            case Mode::Bin:
                _pending <<= 1;
                break;
            case Mode::Oct:
                _pending <<= 3;
                break;
            case Mode::Hex:
                _pending <<= 4;
                break;
            default:
                _pending *= 10;
                break;
        }
        _pending += n;
    }
    displayNumber();
}

void Calculator::removeDigit() {
    if (_newNumber)
        _pending = 0;
    else {
        switch (_mode) {
            case Mode::Bin:
                _pending >>= 1;
                break;
            case Mode::Oct:
                _pending >>= 3;
                break;
            case Mode::Hex:
                _pending >>= 4;
                break;
            default:
                _pending /= 10;
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
            ss << toBinary(_pending);
            break;
        case Mode::Oct:
            ss << std::oct << _pending;
            break;
        case Mode::Hex:
            ss << std::hex << _pending;
            break;
        default:
            ss << std::dec << _pending;
            break;
    }

    display = ss.str();
}

void Calculator::error() {
    _operands.clear();
    _operators.clear();
    _pending = 0;
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
        for (const auto &op : _knownOperators) {
            if (op->symbol() == c) {
                push(op);
                break;
            }
        }
    }
}

bool Calculator::apply(Operator* op) {
    int8_t size = op->operands();
    
    std::vector<int32_t> args(size, _pending);
    int8_t max = size < _operands.size() ? size : _operands.size();
    for (int i = max - 1; i >= 0; i--) {
        args[i] = _operands.back();
        _operands.pop_back();
    }
    auto result = op->calculate(args);
    if (result.has_value()) {
        _operands.push_back(result.value());
    } else {
        return false;
    }
    return true;
}

void Calculator::push(Operator* op) {
    if (op->followsNumber()) {
        _operands.push_back(_pending);
    }
    _newNumber = true;

    while (_operators.size() > 0) {
        Operator *head = _operators.back();
        switch (op->reduce(head))
        { 
        case Operator::ReduceMode::Eval:
            if (!apply(head)) {
                error();
                return;
            }
            _operators.pop_back();
            break;
        case Operator::ReduceMode::DropAndStop:
            _operators.pop_back();
        case Operator::ReduceMode::Stop:
            goto evalEnded;
        }
    }

evalEnded:
    if (op->precedence() < UINT8_MAX) {
        _operators.push_back(op);
    } else if (_operators.size() == 0) {
        if (_operands.size() == 0) {
            error();
        } else {
            _pending = _operands.back();
            _operands.clear();
            _operators.clear();

            displayNumber();
        }
    }   
}
