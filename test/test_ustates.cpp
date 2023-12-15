#include "unity.h"
#include "StateHandler.h"
#include "MockState.h"
#include "StringFifo.h"

void test_string_fifo_one_element()
{
  StringFifo fifo;
  fifo.Push("Hello");
  TEST_ASSERT_EQUAL_STRING("Hello", fifo.Pop().c_str());
}

void test_string_fifo_fill_empty()
{
  StringFifo fifo(3);
  fifo.Push("Hello");
  fifo.Push("World");
  fifo.Push("Today");
  TEST_ASSERT_EQUAL_STRING("Hello", fifo.Pop().c_str());
  TEST_ASSERT_EQUAL_STRING("World", fifo.Pop().c_str());
  TEST_ASSERT_EQUAL_STRING("Today", fifo.Pop().c_str());
  fifo.Push("Goodbye");
  TEST_ASSERT_EQUAL_STRING("Goodbye", fifo.Pop().c_str());
}

void test_create()
{
  StateHandler handler;
}

void test_one_state()
{
  StateHandler handler;
  StringFifo fifo;
  MockState state(&handler, "state1", fifo);

  TEST_ASSERT_EQUAL(&state, handler.addState(1, &state, "state1"));
  handler.requestState(1);
  handler.loop();
  
  TEST_ASSERT_EQUAL_STRING("state1::stateEnter", state.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state1::stateLoop", state.pop().c_str());
}

void test_two_states()
{
  StateHandler handler;
  StringFifo fifo;
  MockState state1(&handler, "state1", fifo);
  MockState state2(&handler, "state2", fifo);

  TEST_ASSERT_EQUAL(&state1, handler.addState(1, &state1, "state1"));
  TEST_ASSERT_EQUAL(&state2, handler.addState(2, &state2, "state2"));
  
  handler.requestState(1);
  handler.loop();
  TEST_ASSERT_EQUAL_STRING("state1::stateEnter", state1.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state1::stateLoop", state1.pop().c_str());

  handler.loop();
  TEST_ASSERT_EQUAL_STRING("state1::stateLoop", state1.pop().c_str());

  handler.requestState(2);
  handler.loop();
  TEST_ASSERT_EQUAL_STRING("state1::stateExit", state1.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state2::stateEnter", state1.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state2::stateLoop", state1.pop().c_str());

}

int runUnityTests(void) 
{
  UNITY_BEGIN();
  RUN_TEST(test_string_fifo_one_element);
  RUN_TEST(test_string_fifo_fill_empty);
  RUN_TEST(test_create);
  RUN_TEST(test_one_state);
  RUN_TEST(test_two_states);
  return UNITY_END();
}

int main(void) 
{
  return runUnityTests();
}
