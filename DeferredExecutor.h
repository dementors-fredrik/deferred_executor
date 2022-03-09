//
// Created by Fredrik Andersson on 2022-03-09.
//

#ifndef DEFERREDEXECUTOR_H
#define DEFERREDEXECUTOR_H
#include <memory>
#include <cassert>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

namespace FCA {
using callback_key_t = std::string;
using callback_t = std::function<void()>;
using callback_list_t = std::vector<callback_t>;
using callback_map_t = std::unordered_map<std::string, std::unique_ptr<callback_list_t>>;
using callback_evaluator_t = std::function<bool(callback_key_t)>;

class DeferredExecutor {
public:
  /**
   * Setup a deferred executor
   * Which will execute callbacks if evaluator returns true for the key
   *
   * @param evaluator
   */
  explicit DeferredExecutor(callback_evaluator_t evaluator)
      : m_evaluator(std::move(evaluator)) {}

  /**
   * Submits callback for execution
   *
   * This method enqueues and executes pending callbacks
   *
   * @param id
   * @param fn
   */
  void submit(const callback_key_t &id, const callback_t &fn);

  /**
   * Invalidates all pending callbacks for the supplied id
   *
   * @param id
   * @return bool - if any callbacks was evict
   */
  bool invalidate(const callback_key_t &id);
  /**
   * Enforce execution of pending callbacks, should be called if external
   * factors alters the evaluation result
   *
   * @return number callbacks run
   */
  size_t evaluate_pending();

private:
  callback_map_t m_pending_callbacks;
  callback_evaluator_t m_evaluator;
};

} // namespace FCA

#endif // DEFERREDEXECUTOR_H
