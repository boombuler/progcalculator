#pragma once

#include <inttypes.h>
#include <list>
#include <vector>

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
    class Operation {
       public:
        int32_t value;
        Operator& op;
    };

    static const std::vector<Operator*> _operators;
    std::list<Operation> _pending;
    int32_t _operant = 0;
    bool _newNumber = true;

    Mode _mode = Mode::Dec;
    void addDigit(int n);
    void removeDigit();
    void error();
    void displayNumber();
    void push(Operator& op);
    void solve();

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
    inline Mode mode() { return _mode; }
    void input(char c);
};
