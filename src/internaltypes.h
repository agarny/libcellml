/*
Copyright libCellML Contributors

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include <map>

#include <unordered_set>

#include <variant>
#include <vector>

#include "libcellml/variable.h"

namespace libcellml {

using ComponentNameMap = std::map<std::string, ComponentPtr>; /**< Type definition for map of component name to component pointer. */

using IndexStack = std::vector<size_t>; /**< Type definition for tracking indicies. */
using EquivalenceMap = std::map<IndexStack, std::vector<IndexStack>>; /**< Type definition for map of variable equivalences defined over model. */

using NameList = std::vector<std::string>; /**< Type definition for list of names. */
using StringStringMap = std::map<std::string, std::string>; /**< Type definition for map of string to string. */

// VariableMap
using VariablePair = std::pair<VariablePtr, VariablePtr>; /**< Type definition for VariablePtr pair.*/
using VariableMap = std::vector<VariablePair>; /**< Type definition for vector of VariablePair.*/
using VariableMapIterator = VariableMap::const_iterator; /**< Type definition of const iterator for vector of VariablePair.*/
// ComponentMap
using ComponentPair = std::pair<ComponentPtr, ComponentPtr>; /**< Type definition for Component pointer pair.*/
using ComponentMap = std::vector<ComponentPair>; /**< Type definition for vector of ComponentPair.*/
using ComponentMapIterator = ComponentMap::const_iterator; /**< Type definition of const iterator for vector of ComponentPair.*/

using InterfaceTypePair = std::pair<Variable::InterfaceType, Variable::InterfaceType>; /**< Type definition for pair of variable interface types. */

using VariablePtrs = std::vector<VariablePtr>; /**< Type definition for list of variables. */

using IdList = std::unordered_set<std::string>; /**< Type definition for list of ids. */

using AnyItem = std::pair<std::string, std::variant<ModelPtr, ImportSourcePtr, UnitsPtr, ComponentPtr,
                                                    VariablePtr, ResetPtr, VariablePair, std::string,
                                                    IssuePtr>>; /**< Type definition for AnyType structure.  The first string is the type of item. **/

using ItemList = std::map<std::string, AnyItem>; /**< Type definition for list of id-able items.  The first is the id of the item. */

} // namespace libcellml
