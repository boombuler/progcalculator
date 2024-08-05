#include "unity.h"
#include "Calculator.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

string calc(string input, int base = 10) {
  Calculator calc;
  for(char c : input)
    calc.input(c);
  calc.input('\r');
  calc.setMode((Calculator::Mode)base);
  return calc.display;
}

void test_initial_state(void) {
  Calculator c;
  TEST_ASSERT_EQUAL(Calculator::Mode::Dec, c.mode());
  TEST_ASSERT_EQUAL_STRING("0", c.display.c_str());
}

void test_binary_output(void){
  TEST_ASSERT_EQUAL_STRING("1", calc("1", 2).c_str());
  TEST_ASSERT_EQUAL_STRING("10", calc("2", 2).c_str());
  TEST_ASSERT_EQUAL_STRING("11", calc("3", 2).c_str());
  TEST_ASSERT_EQUAL_STRING("1100", calc("12", 2).c_str());
}

void test_addition(void) {
  TEST_ASSERT_EQUAL_STRING("2", calc("1+1").c_str());
  TEST_ASSERT_EQUAL_STRING("44", calc("12+32").c_str());
}

void test_order_of_operation(void) {
  TEST_ASSERT_EQUAL_STRING("6", calc("1+2*4-3").c_str());
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_initial_state);
  RUN_TEST(test_binary_output);
  RUN_TEST(test_addition);
  RUN_TEST(test_order_of_operation);
  return UNITY_END();
}

// WARNING!!! PLEASE REMOVE UNNECESSARY MAIN IMPLEMENTATIONS //

/**
  * For native dev-platform or for some embedded frameworks
  */
int main(void) {
  return runUnityTests();
}
