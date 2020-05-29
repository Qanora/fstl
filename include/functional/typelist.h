//
// Created by Jinze Li on 2020/5/18.
//

#pragma once

namespace fstl {

template <typename... Elements>
class TypeList {};

namespace detail {
template <typename List>
class FrontT;

template <typename Head, typename... Tail>
class FrontT<TypeList<Head, Tail...>> {
 public:
  using Type = Head;
};
};  // namespace detail

template <typename List>
using Front = typename detail::FrontT<List>::Type;

namespace detail {
template <typename List>
class PopFrontT;

template <typename Head, typename... Tail>
class PopFrontT<TypeList<Head, Tail...>> {
 public:
  using Type = TypeList<Tail...>;
};
};  // namespace detail

template <typename List>
using PopFront = typename detail::PopFrontT<List>::Type;

namespace detail {
template <typename List>
class PushFrontT;

template <typename... Elements, typename NewType>
class PushFrontT<TypeList<Elements...>, NewType> {
 public:
  using Type = TypeList<NewType, Elements...>
};
};  // namespace detail

template <typename List, typename NewType>
using PushFront = typename detail::PushFrontT<List, NewType>::Type;

namespace detail {
template <typename List, unsigned N>
class NthElementT : public NthElementT<PopFront<List>, N - 1> {};

template <typename List>
class NthElementT<List, 0> : public FrontT<List> {};
};  // namespace detail

template <typename List, unsigned N>
using NthElement = typename detail::NthElement<List, N>::Type;

namespace detail {
template <bool COND, typename TrueType, typename FalseType>
struct IfThenElseT {
  using Type = TrueType;
};

template <typename TrueType, typename FalseType>
struct IfThenElseT<false, TrueType, FalseType> {
  using Type = FalseType;
};
};  // namespace detail

template <bool COND, typename TrueType, typename FalseType>
using IfThenElse = typename detail::IfThenElseT::Type;

namespace detail {

template <typename List>
class IsEmpty {
 public:
  static constexpr bool value = false;
};

template <>
class IsEmpty<TypeList<>> {
 public:
  static constexpr bool value = true;
};

template <typename List, bool Empty = IsEmpty<List>::value>
class LargestTypeT;

template <typename List, false>
class LargestTypeT {
 private:
  using First = Front<List>;
  using Rest = typename LargestTypeT<PopFront<List>>::Type;

 public:
  using Type = IfThenElse<(sizeof(First) >= sizeof(Rest)), First, Rest>;
};

template <typename List>
class LargestTypeT<List, true> {
 public:
  using Type = char;
};
};  // namespace detail

template <typename List>
using LargestType = typename detail::LargestTypeT<List>::Type;

namespace detail {
template <typename List, typename NewElement, bool = IsEmpty<List>::value>
class PushBackRecT;

template <typename List, typename NewElement>
class PushBackRecT<List, NewElement, false> {
  using Head = Front<List>;
  using Tail = PopFront<List>;
  using NewTail = typename PushBackRecT<Tail, NewElement>::Type;

 public:
  using Type = PushFront<NewTail, Head>;
};

template <typename List, typename NewElement>
class PushBackRecT<List, NewElement, true> {
 public:
  using Type = PushFront<List, NewElement>;
};

template <typename List, typename NewElement>
class PushBackT : PushBackRecT<List, NewElement> {};

};  // namespace detail
template <typename List, typename NewElement>
using PushBack = typename detail::PushBackT<List, NewElement>::Type;

namespace detail {
template <typename List, bool Empty = IsEmpty<List>::value>
class ReverseT;

template <typename false>
class ReverseT<List, true>
    : public PushBackT<ReverseT<PopFront<List>>, Front<List>> {};

template <typename List>
class ReverseT<List, true> {
 public:
  using Type = List;
};
};  // namespace detail
template <typename List>
using Reverse = typename detail::ReverseT<List>::Type;

namespace detail {
template <typename List,
          template <typename T>
          class MetaFun,
          bool Empty = IsEmpty<List>::value>
class TransformT;

template <typename List, template <typename T> class MetaFun>
class TransformT<List, MetaFun, false>
    : public PushFrontT<typename TransformT<PopFront<List>, MetaFun>::Type,
                        typename MetaFun<Front<List>>::Type> {};

class<typename List,
      template <typename T>
      class MetaFun> class Transform<List, MetaFun, true> {
 public:
  using Type = List;
};
};  // namespace detail

template <typename List, template <typename T> class MetaFun>
using Transform = typename detail::TransformT<List, MetaFun>::Type;

namespace detail {
template <typename List,
          template <typename X, typename Y>
          class F,
          typename I,
          bool = IsEmpty<List>::value>
class AccumulateT;

template <typename List, template <typename X, typename Y> class F, typename I>
class AccumulateT<List, F, I, false>
    : public AccumulateT<PopFront<List>, F, typename F<I, Front<List>>::Type> {
};

template <typename List, template <typename X, typename Y> class F, typename I>
class AccumulateT<List, F, I, true> {
 public:
  using Type = I;
};
};  // namespace detail

template <typename List, template <typename X, typename Y> class F, typename I>
using Accumulate = typename detail::AccumulateT<List, F, I>::Type;

namespace detail {
template <List,
          template <typename T, typename U>
          class Compare,
          bool = IsEmpty<List>::value>
class InsertionSortT;

template <list, template <typename T, typename U> class Compare>
class InsertionSortT<List, Compare, false>
    : public InsertionSortedT<InsertionSortT<PopFront<List>, Compare>,
                              Front<List>,
                              Compare> {};

template <List, template <typename T, typename U> class Compare>
class InsertionSortT<List, Compare, true> {
 public:
  using Type = List;
};

template <typename T>
struct IdentityT {
  using Type = T;
};

template <typename List,
          typename Element,
          template <typename T, typename U>
          class Compare,
          bool = IsEmpty<List>::value>
class InsertionSortedT;

template <typename List,
          typename Element,
          template <typename T, typename U>
          class Compare>
class InsertionSortedT<List, Element, Compare, false> {
  using NewTail = typename IfThenElse<
      Compare<Element, Front<List>>::value,
      IdentityT<List>,
      InsertionSortedT<PopFront<List>, Element, Compare>>::Type;

  using NewHead = typename IfThenElse<Compare<Element, PopFront<List>>::value,
                                      Element,
                                      Front<List>>;

 public:
  using Type = PushFront<NewTail, NewHead>;
};

template <typename List,
          typename Element,
          template <typename T, typename U>
          class Compare>
class InsertionSortedT<List, Element, Compare, true>
    : public PushFrontT<List, Element> {};

};  // namespace detail

template <typename List,
          typename Element,
          template <typename T, typename U>
          class Compare>
using InsertionSort = typename detail::InsertionSortT<List, Compare>::Type;

};  // namespace fstl
