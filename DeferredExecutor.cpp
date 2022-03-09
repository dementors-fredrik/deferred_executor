//
// Created by Fredrik Andersson on 2022-03-09.
//

#include "DeferredExecutor.h"

namespace FCA {
void DeferredExecutor::submit(const callback_key_t &id, const callback_t &fn) {
  auto arr = m_pending_callbacks.emplace(
      make_pair(id, std::make_unique<callback_list_t>(callback_list_t())));
  arr.first->second->push_back(fn);
  evaluate_pending();
}

size_t DeferredExecutor::evaluate_pending() {
  size_t cbs = 0;
  for (auto session = m_pending_callbacks.begin();
       session != m_pending_callbacks.end();)
    if (m_evaluator(session->first)) {
      for (const auto &callback : *session->second) {
        callback();
        cbs++;
      }
      session = m_pending_callbacks.erase(session);
    } else
      session++;
  return cbs;
}

bool DeferredExecutor::invalidate(const callback_key_t &id) {
  auto list = m_pending_callbacks.find(id);
  if (list == m_pending_callbacks.end())
    return false;
  m_pending_callbacks.erase(list);
  return true;
}
}; // namespace FCA