%module(package="libcellml") annotator

#define LIBCELLML_EXPORT

%import "createconstructor.i"
%import "enums.i"
%import "logger.i"
%import "types.i"

%include <std_pair.i>
%include <std_vector.i>

%feature("docstring") libcellml::Annotator
"Interacts with CellML objects using their identifier attribute.";

%feature("docstring") libcellml::Annotator::setModel
"Set the model for this Annotator to work with.";

%feature("docstring") libcellml::Annotator::item
"Return the item with the given identifier.";

%feature("docstring") libcellml::Annotator::component
"Return the component with the given identifier.";

%feature("docstring") libcellml::Annotator::encapsulation
"Return the model with the given encapsulation identifier.";

%feature("docstring") libcellml::Annotator::model
"Return the model assigned to this Annotator or the model with the given identifier.";

%feature("docstring") libcellml::Annotator::variable
"Return the variable with the given identifier.";

%feature("docstring") libcellml::Annotator::importSource
"Return the import source with the given identifier.";

%feature("docstring") libcellml::Annotator::units
"Return the units with the given identifier.";

%feature("docstring") libcellml::Annotator::reset
"Return the reset with the given identifier.";

%feature("docstring") libcellml::Annotator::testValue
"Return the ResetPtr whose test_value has the given identifier.";

%feature("docstring") libcellml::Annotator::resetValue
"Return the ResetPtr whose reset_value has the given identifier.";

%feature("docstring") libcellml::Annotator::componentRef
"Return the ComponentPtr with the given component_ref identifier.";

%feature("docstring") libcellml::Annotator::unit
"Returns a Unit, a pair consisting of UnitsPtr and index which defines the Unit with the given identifier.";

%feature("docstring") libcellml::Annotator::connection
"Return a VariablePair that defines a connection with given identifier.";

%feature("docstring") libcellml::Annotator::mapVariables
"Return a VariablePair that defines a map_variables with given identifier.";

%feature("docstring") libcellml::Annotator::assignAllIds
"Traverse the stored model and set any blank identifier fields to an automatically generated identifier.";

%feature("docstring") libcellml::Annotator::assignIds
"Set all items of the given type in the stored model to automatically generated, unique strings.";

%feature("docstring") libcellml::Annotator::clearAllIds
"Clear all the identifier strings in the given or stored model.";

%feature("docstring") libcellml::Annotator::isUnique
"Return `true` if the given string occurs exactly once as an identifier in the stored model, `false` otherwise.";

%feature("docstring") libcellml::Annotator::ids
"Return a list of all identifier strings within the stored model.";

%feature("docstring") libcellml::Annotator::duplicateIds
"Return a list of identifier strings which are duplicated within the stored model.";

%feature("docstring") libcellml::Annotator::assignComponentId
"Assign an automatically generated, unique identifier to the given component.";

%feature("docstring") libcellml::Annotator::assignComponentRefId
"Assign an automatically generated, unique identifier to the given component's encapsulation.";

%feature("docstring") libcellml::Annotator::assignConnectionId
"Assign an automatically generated, unique identifier to the connection defined by the VariablePair.";

%feature("docstring") libcellml::Annotator::assignEncapsulationId
"Assign an automatically generated, unique identifier to the encapsulation of the given model.";

%feature("docstring") libcellml::Annotator::assignImportSourceId
"Assign an automatically generated, unique identifier to the given import source.";

%feature("docstring") libcellml::Annotator::assignMapVariablesId
"Assign an automatically generated, unique identifier to the equivalence defined by the VariablePair.";

%feature("docstring") libcellml::Annotator::assignModelId
"Assign an automatically generated, unique identifier to the given model.";

%feature("docstring") libcellml::Annotator::assignResetId
"Assign an automatically generated, unique identifier to the given reset.";

%feature("docstring") libcellml::Annotator::assignResetValueId
"Assign an automatically generated, unique identifier to the reset value of the given reset.";

%feature("docstring") libcellml::Annotator::assignTestValueId
"Assign an automatically generated, unique identifier to the test value of the given reset.";

%feature("docstring") libcellml::Annotator::assignUnitId
"Assign an automatically generated, unique identifier to the unit given by the units and index supplied.";

%feature("docstring") libcellml::Annotator::assignUnitsId
"Assign an automatically generated, unique identifier to the given units.";

%feature("docstring") libcellml::Annotator::assignVariableId
"Assign an automatically generated, unique identifier to the given variable.";

%feature("docstring") libcellml::Annotator::itemCount
"Return the number of items in the stored model with the given identifier.";

%feature("docstring") libcellml::Annotator::hasModel
"Report whether the annotator has a model assigned or not.";

// PRIVATE: Functions only written to support bindings. They are not
// intended to be called from anywhere other than this file.

%feature("docstring") libcellml::Annotator::_itemCellmlElement
"Private: Utility function to retrieve item type integer based on identifier.";

%feature("docstring") libcellml::Annotator::_assignId
"Private: Utility function to assign an automatic identifier.";

%{
#include "libcellml/annotator.h"
%}

%create_constructor(Annotator)

%ignore libcellml::Annotator::item;
%ignore libcellml::Annotator::items;
%ignore libcellml::Annotator::assignId;

%pythoncode %{
# libCellML generated wrapper code starts here.

from libcellml.enums import CellmlElementType
from libcellml.types import AnyCellmlElement
%}

%template() std::vector<std::string>;

%include "libcellml/annotator.h"
%include "libcellml/types.h"

%extend libcellml::Annotator {

    CellmlElementType _itemCellmlElement(const std::string &id, size_t index)
    {
        auto itemInfo = $self->item(id, index);
        return itemInfo->type();
    }
    std::string _assignId(const AnyCellmlElementPtr &item)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::ModelPtr const &item, libcellml::CellmlElementType type)
    {
        return $self->assignId(item, type);
    }
    std::string _assignId(libcellml::ModelPtr const &item)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::ComponentPtr const &item, libcellml::CellmlElementType type)
    {
        return $self->assignId(item, type);
    }
    std::string _assignId(libcellml::ComponentPtr const &item)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::ImportSourcePtr const &item)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::ImportSourcePtr const &item, libcellml::CellmlElementType type)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::ResetPtr const &item, libcellml::CellmlElementType type)
    {
        return $self->assignId(item, type);
    }
    std::string _assignId(libcellml::ResetPtr const &item)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::UnitsPtr const &item)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::UnitPtr const &item)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::UnitPtr const &item, libcellml::CellmlElementType type)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::VariablePtr const &item)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::VariablePtr const &item, libcellml::CellmlElementType type)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::VariablePairPtr const &item, libcellml::CellmlElementType type)
    {
        return $self->assignId(item, type);
    }
    std::string _assignId(libcellml::VariablePairPtr const &item)
    {
        return $self->assignId(item);
    }
    std::string _assignId(libcellml::VariablePtr const &item1, libcellml::VariablePtr const &item2, libcellml::CellmlElementType type)
    {
        return $self->assignId(item1, item2, type);
    }
    std::string _assignId(libcellml::VariablePtr const &item1,libcellml::VariablePtr const &item2)
    {
        return $self->assignId(item1, item2);
    }
    std::string _assignId(libcellml::UnitsPtr const &item, size_t index)
    {
        return $self->assignId(item, index);
    }
    std::string _assignId(libcellml::UnitsPtr const &item, libcellml::CellmlElementType type)
    {
        return $self->assignId(item);
    }

%pythoncode %{
    def assignId(self, *args):
        r"""Set the identifier of the given item to an automatically generated, unique string, and return it."""
        if len(args) == 1:
            return _annotator.Annotator__assignId(self, args[0])
        return _annotator.Annotator__assignId(self, args[0], args[1])

    def item(self, id, index=-1):
        r"""Retrieve a unique item with the given identifier."""

        from libcellml.types import AnyCellmlElement

        type_dict = {
            CellmlElementType.COMPONENT: _annotator.Annotator_component,
            CellmlElementType.COMPONENT_REF: _annotator.Annotator_componentRef,
            CellmlElementType.CONNECTION: _annotator.Annotator_connection,
            CellmlElementType.ENCAPSULATION: _annotator.Annotator_encapsulation,
            CellmlElementType.IMPORT: _annotator.Annotator_importSource,
            CellmlElementType.MAP_VARIABLES: _annotator.Annotator_mapVariables,
            CellmlElementType.MODEL: _annotator.Annotator_model,
            CellmlElementType.RESET: _annotator.Annotator_reset,
            CellmlElementType.RESET_VALUE: _annotator.Annotator_resetValue,
            CellmlElementType.TEST_VALUE: _annotator.Annotator_testValue,
            CellmlElementType.UNIT: _annotator.Annotator_unit,
            CellmlElementType.UNITS: _annotator.Annotator_units,
            CellmlElementType.VARIABLE: _annotator.Annotator_variable,
        }

        if index == -1:
            num = _annotator.Annotator_itemCount(self, id)
            if num > 1:
                from libcellml import Issue
                issue = Issue()
                issue.setDescription("The identifier '" + id + "' occurs " + str(num) + " times in the model so a unique item cannot be located.")
                issue.setLevel(Issue.Level.WARNING)
                self.addIssue(issue)
                return AnyCellmlElement()

            if num == 0:
                from libcellml import Issue
                issue = Issue()
                issue.setDescription("Could not find an item with an identifier of '" + id + "' in the model.")
                issue.setLevel(Issue.Level.WARNING)
                self.addIssue(issue)
                return AnyCellmlElement()

        if index == -1:
            index = 0

        type = _annotator.Annotator__itemCellmlElement(self, id, index)
        if type in type_dict:
            return AnyCellmlElement(type, type_dict[type](self, id, index))
        return AnyCellmlElement(CellmlElementType.UNDEFINED, None)

    def items(self, id):
        r"""Returns everything with the given identifier as as list of (type, item) tuples."""
        count = _annotator.Annotator_itemCount(self, id)
        itemsList = []
        for c in range(0, count):
            items_with_id = self.item(id, c)
            itemsList.append(items_with_id)
        return itemsList
%}
}
