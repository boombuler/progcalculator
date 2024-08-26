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

void test_reset_after_calculation(void) {
  Calculator c;
  c.input('4');
  c.input('\r');
  TEST_ASSERT_EQUAL_STRING("4", c.display.c_str());
  c.input('\r');
  TEST_ASSERT_EQUAL_STRING("0", c.display.c_str());
}

void test_binary_output(void){
  TEST_ASSERT_EQUAL_STRING("1", calc("1", 2).c_str());
  TEST_ASSERT_EQUAL_STRING("10", calc("2", 2).c_str());
  TEST_ASSERT_EQUAL_STRING("11", calc("3", 2).c_str());
  TEST_ASSERT_EQUAL_STRING("1100", calc("12", 2).c_str());
  TEST_ASSERT_EQUAL_STRING("1111", calc("15", 2).c_str());
  TEST_ASSERT_EQUAL_STRING("10000", calc("16", 2).c_str());
  TEST_ASSERT_EQUAL_STRING("10001", calc("17", 2).c_str());
}

void test_hexadecimal_output(void){
  TEST_ASSERT_EQUAL_STRING("5", calc("5", 16).c_str());
  TEST_ASSERT_EQUAL_STRING("a", calc("10", 16).c_str());
  TEST_ASSERT_EQUAL_STRING("b", calc("11", 16).c_str());
  TEST_ASSERT_EQUAL_STRING("c", calc("12", 16).c_str());
  TEST_ASSERT_EQUAL_STRING("d", calc("13", 16).c_str());
  TEST_ASSERT_EQUAL_STRING("e", calc("14", 16).c_str());
  TEST_ASSERT_EQUAL_STRING("f", calc("15", 16).c_str());
  TEST_ASSERT_EQUAL_STRING("10", calc("16", 16).c_str());
  TEST_ASSERT_EQUAL_STRING("1f", calc("31", 16).c_str());
  TEST_ASSERT_EQUAL_STRING("1a", calc("26", 16).c_str());
}

void test_octal_output(void) {
  TEST_ASSERT_EQUAL_STRING("1", calc("1", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("2", calc("2", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("3", calc("3", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("4", calc("4", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("5", calc("5", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("6", calc("6", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("7", calc("7", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("10", calc("8", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("11", calc("9", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("12", calc("10", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("13", calc("11", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("14", calc("12", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("15", calc("13", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("16", calc("14", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("17", calc("15", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("20", calc("16", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("21", calc("17", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("22", calc("18", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("23", calc("19", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("24", calc("20", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("25", calc("21", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("26", calc("22", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("27", calc("23", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("30", calc("24", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("31", calc("25", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("32", calc("26", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("33", calc("27", 8).c_str());
  TEST_ASSERT_EQUAL_STRING("34", calc("28", 8).c_str());
}

void test_addition_op(void) {
  TEST_ASSERT_EQUAL_STRING("2", calc("1+1").c_str());
  TEST_ASSERT_EQUAL_STRING("44", calc("12+32").c_str());
}

void test_subtraction_op(void) {
  TEST_ASSERT_EQUAL_STRING("0", calc("1-1").c_str());
  TEST_ASSERT_EQUAL_STRING("10", calc("32-22").c_str());
}

void test_multiplication_op(void) {
  TEST_ASSERT_EQUAL_STRING("1", calc("1*1").c_str());
  TEST_ASSERT_EQUAL_STRING("384", calc("12*32").c_str());
}

void test_division_op(void) {
  TEST_ASSERT_EQUAL_STRING("1", calc("1/1").c_str());
  TEST_ASSERT_EQUAL_STRING("2", calc("32/16").c_str());
}

void test_shl_op(void) {
  TEST_ASSERT_EQUAL_STRING("2", calc("1<1").c_str());
  TEST_ASSERT_EQUAL_STRING("64", calc("32<1").c_str());
  TEST_ASSERT_EQUAL_STRING("256", calc("32<3").c_str());
  TEST_ASSERT_EQUAL_STRING("1024", calc("32<5").c_str());
}

void test_shr_op(void) {
  TEST_ASSERT_EQUAL_STRING("0", calc("1>1").c_str());
  TEST_ASSERT_EQUAL_STRING("16", calc("32>1").c_str());
  TEST_ASSERT_EQUAL_STRING("4", calc("32>3").c_str());
  TEST_ASSERT_EQUAL_STRING("1", calc("32>5").c_str());
}

void test_and_op(void) {
  TEST_ASSERT_EQUAL_STRING("0", calc("0&0").c_str());
  TEST_ASSERT_EQUAL_STRING("0", calc("0&1").c_str());
  TEST_ASSERT_EQUAL_STRING("0", calc("1&0").c_str());
  TEST_ASSERT_EQUAL_STRING("1", calc("1&1").c_str());
  TEST_ASSERT_EQUAL_STRING("4", calc("6&12").c_str());
  TEST_ASSERT_EQUAL_STRING("32", calc("44&51").c_str());
}

void test_or_op(void) {
  TEST_ASSERT_EQUAL_STRING("1", calc("1|1").c_str());
  TEST_ASSERT_EQUAL_STRING("48", calc("32|16").c_str());
  TEST_ASSERT_EQUAL_STRING("49", calc("33|16").c_str());
  TEST_ASSERT_EQUAL_STRING("50", calc("34|16").c_str());
  TEST_ASSERT_EQUAL_STRING("51", calc("35|16").c_str());
  TEST_ASSERT_EQUAL_STRING("52", calc("36|16").c_str());
  TEST_ASSERT_EQUAL_STRING("53", calc("37|16").c_str());
}

void test_xor_op(void) {
  TEST_ASSERT_EQUAL_STRING("1", calc("1^0").c_str());
  TEST_ASSERT_EQUAL_STRING("48", calc("32^16").c_str());
  TEST_ASSERT_EQUAL_STRING("49", calc("33^16").c_str());
  TEST_ASSERT_EQUAL_STRING("50", calc("34^16").c_str());
  TEST_ASSERT_EQUAL_STRING("51", calc("35^16").c_str());
  TEST_ASSERT_EQUAL_STRING("52", calc("36^16").c_str());
  TEST_ASSERT_EQUAL_STRING("53", calc("37^16").c_str());
}

void test_modulo_op(void) {
  TEST_ASSERT_EQUAL_STRING("0", calc("1%1").c_str());
  TEST_ASSERT_EQUAL_STRING("0", calc("32%16").c_str());
  TEST_ASSERT_EQUAL_STRING("1", calc("33%16").c_str());
  TEST_ASSERT_EQUAL_STRING("2", calc("34%16").c_str());
  TEST_ASSERT_EQUAL_STRING("3", calc("35%16").c_str());
  TEST_ASSERT_EQUAL_STRING("4", calc("36%16").c_str());
  TEST_ASSERT_EQUAL_STRING("5", calc("37%16").c_str());
}

void test_order_of_operation(void) {
  TEST_ASSERT_EQUAL_STRING("6", calc("1+2*4-3").c_str());
  TEST_ASSERT_EQUAL_STRING("3", calc("1+2*(4-3)").c_str());
  TEST_ASSERT_EQUAL_STRING("9", calc("(1+2)*4-3").c_str());
  TEST_ASSERT_EQUAL_STRING("1", calc("27/9/3").c_str());
}

void test_invalid_expressions(void) {
  TEST_ASSERT_EQUAL_STRING("Error", calc("(1+1").c_str());
  TEST_ASSERT_EQUAL_STRING("Error", calc("1/0").c_str());
  TEST_ASSERT_EQUAL_STRING("Error", calc("5%0").c_str());
}

void test_reuse_last_number(void) {
  TEST_ASSERT_EQUAL_STRING("4", calc("1+++").c_str());
  TEST_ASSERT_EQUAL_STRING("9", calc("3+*2").c_str());
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_initial_state);
  RUN_TEST(test_binary_output);
  RUN_TEST(test_hexadecimal_output);
  RUN_TEST(test_octal_output);
  RUN_TEST(test_addition_op);
  RUN_TEST(test_subtraction_op);
  RUN_TEST(test_multiplication_op);
  RUN_TEST(test_division_op);
  RUN_TEST(test_modulo_op);
  RUN_TEST(test_shl_op);
  RUN_TEST(test_shr_op);
  RUN_TEST(test_and_op);
  RUN_TEST(test_or_op);
  RUN_TEST(test_xor_op);
  RUN_TEST(test_order_of_operation);
  RUN_TEST(test_invalid_expressions);
  RUN_TEST(test_reuse_last_number);
  return UNITY_END();
}

/**
  * For native dev-platform or for some embedded frameworks
  */
int main(void) {
  return runUnityTests();
}
