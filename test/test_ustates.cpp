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
  
  TEST_ASSERT_EQUAL_INT8(1, handler.getStateId());
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
  TEST_ASSERT_EQUAL_INT8(1, handler.getStateId());
  TEST_ASSERT_EQUAL_STRING("state1::stateEnter", state1.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state1::stateLoop", state1.pop().c_str());

  handler.loop();
  TEST_ASSERT_EQUAL_INT8(1, handler.getStateId());
  TEST_ASSERT_EQUAL_STRING("state1::stateLoop", state1.pop().c_str());

  handler.requestState(2);
  TEST_ASSERT_EQUAL_INT8(1, handler.getStateId());
  handler.loop();
  TEST_ASSERT_EQUAL_INT8(2, handler.getStateId());
  TEST_ASSERT_EQUAL_STRING("state1::stateExit", state1.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state2::stateEnter", state1.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state2::stateLoop", state1.pop().c_str());

}

void test_cant_add_same_id_twice()
{
  StateHandler handler;
  StringFifo fifo;
  MockState state(&handler, "state1", fifo);
  MockState state2(&handler, "state2", fifo);

  TEST_ASSERT_EQUAL(&state, handler.addState(1, &state, "state1"));
  TEST_ASSERT_EQUAL(nullptr, handler.addState(1, &state, "duplicate"));
  TEST_ASSERT_EQUAL(nullptr, handler.addState(1, &state2, "another duplicate"));
}

void test_request_last_state()
{
  StateHandler handler;
  StringFifo fifo;
  MockState state1(&handler, "state1", fifo);
  MockState state2(&handler, "state2", fifo);

  handler.addState(1, &state1, "state1");
  handler.addState(2, &state2, "state2");

  // First check that it is nit possible to request the last state
  // if there was no last state.
  TEST_ASSERT_FALSE(handler.requestLastState());
  TEST_ASSERT_FALSE(state1.hasEvent());
  TEST_ASSERT_FALSE(state2.hasEvent());
  
  handler.requestState(1);
  handler.loop();
  TEST_ASSERT_EQUAL_INT8(1, handler.getStateId());
  TEST_ASSERT_EQUAL_STRING("state1::stateEnter", state1.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state1::stateLoop", state1.pop().c_str());
  TEST_ASSERT_FALSE(state1.hasEvent());

  // It still should not be possible to switbh back to last state since there is none.
  TEST_ASSERT_FALSE(handler.requestLastState());
  TEST_ASSERT_FALSE(state1.hasEvent());
  TEST_ASSERT_FALSE(state2.hasEvent());

  // Now enter state 2
  handler.requestState(2);
  handler.loop();
  TEST_ASSERT_EQUAL_INT8(2, handler.getStateId());
  TEST_ASSERT_EQUAL_STRING("state1::stateExit", state1.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state2::stateEnter", state2.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state2::stateLoop", state2.pop().c_str());
  TEST_ASSERT_FALSE(state1.hasEvent());
  TEST_ASSERT_FALSE(state2.hasEvent());

  // After being instate 2 it should be possible to return to state 1
  handler.requestLastState();
  handler.loop();
  TEST_ASSERT_EQUAL_STRING("state2::stateExit", state2.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state1::stateEnter", state1.pop().c_str());
  TEST_ASSERT_EQUAL_STRING("state1::stateLoop", state1.pop().c_str());
  TEST_ASSERT_FALSE(state1.hasEvent());
  TEST_ASSERT_FALSE(state2.hasEvent());
}

int runUnityTests(void) 
{
  UNITY_BEGIN();
  RUN_TEST(test_string_fifo_one_element);
  RUN_TEST(test_string_fifo_fill_empty);
  RUN_TEST(test_create);
  RUN_TEST(test_one_state);
  RUN_TEST(test_two_states);
  RUN_TEST(test_cant_add_same_id_twice);
  RUN_TEST(test_request_last_state);
  return UNITY_END();
}

int main(void) 
{
  return runUnityTests();
}
