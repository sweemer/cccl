//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
// SPDX-FileCopyrightText: Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES.
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCUDACXX___TUPLE_MAKE_TUPLE_TYPES_H
#define _LIBCUDACXX___TUPLE_MAKE_TUPLE_TYPES_H

#ifndef __cuda_std__
#  include <__config>
#endif // __cuda_std__

#include "../__fwd/array.h"
#include "../__fwd/tuple.h"
#include "../__tuple_dir/apply_cv.h"
#include "../__tuple_dir/tuple_element.h"
#include "../__tuple_dir/tuple_indices.h"
#include "../__tuple_dir/tuple_size.h"
#include "../__tuple_dir/tuple_types.h"
#include "../__type_traits/remove_cv.h"
#include "../__type_traits/remove_reference.h"
#include "../cstddef"

#if defined(_LIBCUDACXX_USE_PRAGMA_GCC_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCUDACXX_BEGIN_NAMESPACE_STD

// __make_tuple_types<_Tuple<_Types...>, _Ep, _Sp>::type is a
// __tuple_types<_Types...> using only those _Types in the range [_Sp, _Ep).
// _Sp defaults to 0 and _Ep defaults to tuple_size<_Tuple>.  If _Tuple is a
// lvalue_reference type, then __tuple_types<_Types&...> is the result.

template <class _TupleTypes, class _TupleIndices>
struct __make_tuple_types_flat;

template <template <class...> class _Tuple, class... _Types, size_t... _Idx>
struct __make_tuple_types_flat<_Tuple<_Types...>, __tuple_indices<_Idx...>>
{
  // Specialization for pair, tuple, and __tuple_types
  template <class _Tp, class _ApplyFn = __apply_cv_t<_Tp>>
  using __apply_quals _LIBCUDACXX_NODEBUG_TYPE =
      __tuple_types< typename _ApplyFn::template __apply<__type_pack_element<_Idx, _Types...>>... >;
};

template <class _Vt, size_t _Np, size_t... _Idx>
struct __make_tuple_types_flat<array<_Vt, _Np>, __tuple_indices<_Idx...>>
{
  template <size_t>
  using __value_type = _Vt;
  template <class _Tp, class _ApplyFn = __apply_cv_t<_Tp>>
  using __apply_quals = __tuple_types< typename _ApplyFn::template __apply<__value_type<_Idx>>... >;
};

template <class _Tp, size_t _Ep = tuple_size<__libcpp_remove_reference_t<_Tp>>::value, size_t _Sp = 0,
    bool _SameSize = (_Ep == tuple_size<__libcpp_remove_reference_t<_Tp>>::value)>
struct __make_tuple_types
{
  static_assert(_Sp <= _Ep, "__make_tuple_types input error");
  using _RawTp = __remove_cv_t<__libcpp_remove_reference_t<_Tp>>;
  using _Maker = __make_tuple_types_flat<_RawTp, __make_tuple_indices_t<_Ep, _Sp>>;
  using type   = typename _Maker::template __apply_quals<_Tp>;
};

template <class... _Types, size_t _Ep>
struct __make_tuple_types<tuple<_Types...>, _Ep, 0, true>
{
  typedef _LIBCUDACXX_NODEBUG_TYPE __tuple_types<_Types...> type;
};

template <class... _Types, size_t _Ep>
struct __make_tuple_types<__tuple_types<_Types...>, _Ep, 0, true>
{
  typedef _LIBCUDACXX_NODEBUG_TYPE __tuple_types<_Types...> type;
};

template <class _Tp, size_t _Ep = tuple_size<__libcpp_remove_reference_t<_Tp>>::value, size_t _Sp = 0>
using __make_tuple_types_t = typename __make_tuple_types<_Tp, _Ep, _Sp>::type;

_LIBCUDACXX_END_NAMESPACE_STD

#endif // _LIBCUDACXX___TUPLE_MAKE_TUPLE_TYPES_H
