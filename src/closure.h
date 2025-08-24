// Copyright 2024 The Worm Authors. All rights reserved.
// Licensed under the MIT that can be found in the LINCENSE file
// in the root directory of this source tree.

#ifndef __CLOSURE_H__
#define __CLOSURE_H__
#include <memory>
#include <tuple>

namespace worm {
/**
 * @brief 闭包函数基类，用来表示任务。
 */
class Closure {
 public:
  Closure() {}
  virtual ~Closure() {}
  virtual void Run() {}
};

template <size_t...>
struct IndexSequence {};

template <size_t... Ns>
struct MakeIndexSequenceImpl {};

template <size_t... Ns>
struct MakeIndexSequenceImpl<0, Ns...> {
  using Type = IndexSequence<Ns...>;
};

template <size_t N, size_t... Ns>
struct MakeIndexSequenceImpl<N, Ns...>
    : MakeIndexSequenceImpl<N - 1, N - 1, Ns...> {};

template <size_t N>
using MakeIndexSequence = typename MakeIndexSequenceImpl<N>::Type;

template <typename Pointer>
struct IsWeakRef : public std::false_type {};

template <typename T>
struct IsWeakRef<std::weak_ptr<T>> : public std::true_type {};

template <bool IsWeakCall, typename P, typename F, typename T>
struct InvokeHelper;

template <typename P, typename F, typename T>
struct InvokeHelper<true, P, F, T> {
  template <typename std::size_t... I>
  static void Invoke(P p, F& f, T& t, IndexSequence<I...>) {
    if (!p.expired()) {
      auto ptr = p.lock();
      (ptr.get()->*f)(std::get<I>(std::forward<T>(t))...);
    }
  }
};

template <typename P, typename F, typename T>
struct InvokeHelper<false, P, F, T> {
  template <typename std::size_t... I>
  static void Invoke(P p, F& f, T& t, IndexSequence<I...>) {
    (p->*f)(std::get<I>(std::forward<T>(t))...);
  }
};

template <typename F, typename T, typename M>
struct InvokeHelper<false, std::shared_ptr<M>, F, T> {
  template <typename std::size_t... I>
  static void Invoke(const std::shared_ptr<M>& p, F& f, T& t,
                     IndexSequence<I...>) {
    (p.get()->*f)(std::get<I>(std::forward<T>(t))...);
  }
};

template <typename Pointer, typename Functor, typename... Args>
class Callback : public Closure {
 public:
  using BoundIndices = MakeIndexSequence<sizeof...(Args)>;
  using FunctionType = typename std::decay<Functor>::type;
  using PointerType = typename std::decay<Pointer>::type;
  using StorageType = typename std::tuple<typename std::decay<Args>::type...>;
  using IsWeak = IsWeakRef<Pointer>;

  explicit Callback(Pointer p, Functor&& f, Args&&... args)
      : f_(std::forward<Functor>(f)),
        p_(p),
        args_storage_(std::forward<Args>(args)...) {}

  void Run() override {
    InvokeHelper<IsWeak::value, Pointer, Functor, StorageType>::Invoke(
        p_, f_, args_storage_, BoundIndices());
  }

 private:
  FunctionType f_;
  Pointer p_;
  StorageType args_storage_;
};

template <typename Pointer, typename Functor, typename... Args>
Closure* BindMember(Pointer p, Functor&& f, Args&&... args) {
  using CallbackType = Callback<Pointer, Functor, Args...>;
  return new CallbackType(p, std::forward<Functor>(f),
                          std::forward<Args>(args)...);
}

template <typename F, typename... Args>
class FunctionCallback : public Closure {
 public:
  using BoundIndices = MakeIndexSequence<sizeof...(Args)>;
  using StorageType = typename std::tuple<typename std::decay<Args>::type...>;
  FunctionCallback(F&& func, Args&&... args)
      : func_(std::move(func)), args_storage_(std::forward<Args>(args)...) {}
  virtual ~FunctionCallback() {}
  virtual void Run() override { invoke(BoundIndices()); }

 private:
  template <typename std::size_t... I>
  void invoke(IndexSequence<I...>) {
    (func_)(std::get<I>(std::forward<StorageType>(
        args_storage_))...);  // 这里最后的 ... 是参数包模式展开语法，前面 I
                              // 是一个参数包，准确来说是一个整数序列，...
                              // 就是展开形成多次 std::get<>
  }
  F func_;
  StorageType args_storage_;
};

template <typename F, typename... Args>
static Closure* Bind(F&& func, Args&&... args) {
  return new FunctionCallback<F, Args...>(std::move(func),
                                          std::forward<Args>(args)...);
}

}  // namespace worm

#endif
