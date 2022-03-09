#include "DeferredExecutor.h"
#include <iostream>


using namespace std;
using namespace FCA;

int main() {
  unordered_map<callback_key_t, bool> run_map;
  size_t callbacks_fired = 0;

  auto trigger_condition = [&](const std::string &id) {
    run_map.emplace(make_pair(id, true));
  };

  DeferredExecutor executor([&](const callback_key_t &s) -> bool {
    return run_map.find(s) != run_map.end();
  });

  auto fixture = [&callbacks_fired] {
      callbacks_fired++;
  };

  for (const auto &s : vector<string>{"s1", "s2", "s3", "s4"}) {
    executor.submit(s, fixture);
  }

  // 4 callbacks in queue, none should run at this point
  assert(executor.evaluate_pending() == 0);

  // s1 fired, 1 callback should run
  trigger_condition("s1");
  assert(executor.evaluate_pending() == 1);


  // as s1 been fired this callback should trigger immediately on submit
  // evaluate pending should have no eligible callbacks to run
  executor.submit("s1", fixture);
  assert(executor.evaluate_pending() == 0);

  // submit a new callback for id s2- which has yet to be triggered
  executor.submit("s2", fixture);
  trigger_condition("s2");
  trigger_condition("s3");
  trigger_condition("s4");
  // The rest of the callbacks should trigger now
  assert(executor.evaluate_pending() == 4);

  // A total of 6 callbacks should be run
  assert(callbacks_fired == 6);



  executor.submit("s6", fixture);
  executor.invalidate("s6");
  trigger_condition("s6");
  // submit callbacks are retracted before s6 triggers
  assert(executor.evaluate_pending() == 0);

  cout << "All tests passed" << std::endl;

  return 0;
}
