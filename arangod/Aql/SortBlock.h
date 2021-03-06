////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2016 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Max Neunhoeffer
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGOD_AQL_SORT_BLOCK_H
#define ARANGOD_AQL_SORT_BLOCK_H 1

#include "Basics/Common.h"
#include "Aql/ExecutionBlock.h"
#include "Aql/SortNode.h"

#ifdef USE_IRESEARCH
#include "search/sort.hpp"
#endif // USE_IRESEARCH

namespace arangodb {
namespace transaction {
class Methods;
}

namespace aql {

class AqlItemBlock;

class ExecutionEngine;

#ifdef USE_IRESEARCH
  typedef int(*CompareFunc)(
    irs::sort::prepared const* comparer,
    transaction::Methods* trx,
    AqlValue const& lhs,
    AqlValue const& rhs
  );

  typedef std::tuple<RegisterId, bool, irs::sort::prepared::ptr, CompareFunc> SortRegister;
#else
  typedef std::tuple<RegisterId, bool> SortRegister;
#endif

class SortBlock final : public ExecutionBlock {
 public:
  SortBlock(ExecutionEngine*, SortNode const*);

  ~SortBlock();

  int initializeCursor(AqlItemBlock* items, size_t pos) override final;

  int getOrSkipSome(size_t atMost, bool skipping, AqlItemBlock*&, size_t& skipped) override final;

  /// @brief dosorting
 private:
  void doSorting();

  /// @brief pairs, consisting of variable and sort direction
  /// (true = ascending | false = descending)
  std::vector<SortRegister> _sortRegisters;

  /// @brief whether or not the sort should be stable
  bool _stable;

  bool _mustFetchAll;
};

}  // namespace arangodb::aql
}  // namespace arangodb

#endif
