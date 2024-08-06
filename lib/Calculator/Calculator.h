#pragma once

#include <inttypes.h>
#include <list>
#include <vector>
#include <deque>

#include "Operator.h"

using namespace std;

class Calculator {
   public:
    enum class Mode {
        Dec = 10,
        Hex = 16,
        Bin = 2,
        Oct = 8,
    };

   private:
    static const std::vector<Operator*> _knownOperators;
    deque<int32_t> _operands;
    deque<Operator*> _operators;
    int32_t _pending = 0;
    bool _newNumber = true;

    Mode _mode = Mode::Dec;
    void addDigit(int n);
    void removeDigit();
    void error();
    void displayNumber();
    void push(Operator* op);
    bool apply(Operator* op);
   public:
    string display = "0";
    inline bool setMode(Mode m) {
        if (_mode != m) {
            _mode = m;
            displayNumber();
            return true;
        }
        return false;
    }
    inline Mode mode() const { return _mode; }
    void input(char c);
};
