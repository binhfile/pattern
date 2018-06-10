//
// Created by ngothanhbinh on 10/06/2018.
//

#ifndef OBSERVER_OBSERVER_H
#define OBSERVER_OBSERVER_H

#include <functional>
#include <list>
#include <shared_mutex>

namespace lib {
namespace pattern {

template <typename Tag, typename... Args>
class observable {
  public:
    observable() = default;
    virtual ~observable() = default;

    void publish(Args &&... args) {
        std::shared_lock<std::shared_mutex> lck(lock_);
        for (const auto &callable : lst_function_) {
            callable(std::forward<Args>(args)...);
        }
    }
    template <typename Callable>
    void subscribe(const Callable &callable) {
        std::unique_lock<std::shared_mutex> lck(lock_);
        lst_function_.emplace_back(std::forward<decltype(callable)>(callable));
    }

  private:
    using Function = std::function<void(Args...)>;
    std::list<Function> lst_function_;
    std::shared_mutex lock_;
};

} // namespace pattern
} // namespace lib

#endif // OBSERVER_OBSERVER_H
