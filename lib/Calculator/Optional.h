#include <stdexcept>

template <typename T>
class Optional {
   private:
    T _value;
    bool _is_present;

   public:
    Optional() : _value(T()), _is_present(false) {}

    Optional(T value) : _value(value), _is_present(true) {}

    bool has_value() const { return _is_present; }

    T value() const {
        if (!_is_present) {
            return T();
        }
        return _value;
    }
};
