/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0

#include <test/tools/ossfuzz/ValueGenerator.h>

#include <liblangutil/Exceptions.h>

#include <regex>
#include <iostream>

using namespace std;

void ValueGenerator::initialiseType(TypeInfo& _t)
{
	switch (_t.type)
	{
	case Type::Boolean:
		_t.value += "true";
		break;
	case Type::Integer:
		_t.value += "12";
		break;
	case Type::UInteger:
		_t.value += "23";
		break;
	case Type::String:
		_t.value += "0xdeadbeef";
		break;
	case Type::Bytes:
		_t.value += "0xc0de";
		break;
	case Type::FixedBytes:
		_t.value += "0x" + std::string(static_cast<size_t>(_t.fixedByteWidth) * 2, 'a');
		break;
	case Type::Address:
		_t.value += "0x" + std::string(static_cast<size_t>(FixedBytesWidth::Bytes20) * 2, 'a');
		break;
	case Type::Function:
		_t.value += "0x" + std::string(static_cast<size_t>(FixedBytesWidth::Bytes24) * 2, 'a');
		break;
	default:
		solAssert(false, "Value Generator: Invalid value type.");
	}
}

void ValueGenerator::initialiseTuple(TypeInfo& _tuple)
{
	_tuple.value += "(";
	std::string separator;
	for (auto& c: _tuple.tupleInfo)
	{
		_tuple.value += separator + c.value;
//		if (c.arrayInfo.empty())
//		{
//			if (c.type == Type::Tuple)
//				initialiseTuple(c);
//			else
//				initialiseType(c);
//		}
//		else
//		{
//			initialiseArray(c.arrayInfo, c);
//			cout << c.arrayInfo.size() << endl;
//			cout << c.arrayInfo.back().numElements << endl;
//			cout << c.value << endl;
//		}
		if (separator.empty())
			separator = ",";
	}
	_tuple.value += ")";
}

void ValueGenerator::initialiseArray(
	ArrayInfo& _arrayInfo,
	TypeInfo& _typeInfo
)
{
	cout << "Init 1D array" << endl;
	cout << _typeInfo.value << endl;
	_typeInfo.value += "[";
	std::string separator;
	for (size_t j = 0; j < _arrayInfo.numElements; j++)
	{
		_typeInfo.value += separator;
		if (_typeInfo.type == Type::Tuple) {
			cout << "Tuple inside array" << endl;
			initialiseTuple(_typeInfo);
		}
		else
			initialiseType(_typeInfo);
		if (separator.empty())
			separator = ",";
	}
	_typeInfo.value += "]";
	cout << _typeInfo.value << endl;
}

void ValueGenerator::initialiseArray(
	vector<ArrayInfo>& _arrayInfo,
	TypeInfo& _typeInfo
)
{
	if (_arrayInfo.size() == 1)
		initialiseArray(_arrayInfo[0], _typeInfo);
	else
	{
		vector<ArrayInfo> copy = _arrayInfo;
		auto k = copy.back();
		copy.pop_back();
		_typeInfo.value += "[";
		std::string separator;
		for (size_t i = 0; i < k.numElements; i++)
		{
			_typeInfo.value += separator;
			initialiseArray(copy, _typeInfo);
			if (separator.empty())
				separator = ",";
		}
		_typeInfo.value += "]";
	}
}

void ValueGenerator::initialiseArrayOfTuple(
	vector<ArrayInfo>&,
	TypeInfo&
)
{

}

void ValueGenerator::typeHelper(Json::Value const& _type, TypeInfo& _typeInfo)
{
	std::string jsonTypeString = _type["type"].asString();
	/*
	 * Index | Match description
	 * 0 | Entire type string e.g., bool[1][2][]
	 * 1 | Type string e.g., bool, uint256, address etc.
	 * 2 | Base type e.g., uint, int, bytes
	 * 3 | Type width e.g., 256, 64, 1...32
	 * 4 | First array bracket e.g., [1] in uint256[1][2][3]
	 * 5 | First array dimension e.g., 1 in uint256[1][2][3]
	 * 6 | Second array bracket e.g., [2] in uint256[1][2][3]
	 * 7 | Second array dimension e.g., 2 in uint256[1][2][3]
	 * 8 | Third array bracket e.g., [3] in uint256[1][2][3]
	 * 9 | Third array dimension e.g., 3 in uint256[1][2][3]
	 */
	regex r = regex(
		"(bool|(uint|int|bytes)(\\d+)|address|bytes|string|function|tuple)"
		"(\\[(\\d+)?\\])?(\\[(\\d+)?\\])?(\\[(\\d+)?\\])?"
	);
	smatch matches;
	auto match = regex_search(jsonTypeString, matches, r);
	solAssert(match, "Value generator: Regex match failed.");
	solAssert(
		!matches[1].str().empty(),
		"Value generator: Invalid type"
	);
	auto typeString = matches[1].str();
	size_t width = 0;
	if (matches[3].matched)
		width = stoul(matches[3].str());

	if (typeString.find("bool") != string::npos)
	{
		_typeInfo.type = Type::Boolean;
		_typeInfo.name = "bool";
	}
	else if (typeString.find("function") != string::npos)
	{
		_typeInfo.type = Type::Function;
		_typeInfo.name = "function";
	}
	else if (typeString.find("address") != string::npos)
	{
		_typeInfo.type = Type::Address;
		_typeInfo.name = "address";
	}
	else if (typeString.find("tuple") != string::npos)
	{
		_typeInfo.type = Type::Tuple;
		tuple(_type["components"], _typeInfo);
	}
	else if (typeString.find("bytes") != string::npos)
	{
		if (matches[3].matched)
		{
			solAssert(
				width >= 1 && width <= 32,
				"Value generator: Invalid fixed bytes type."
			);
			_typeInfo.type = Type::FixedBytes;
			_typeInfo.fixedByteWidth = static_cast<FixedBytesWidth>(width);
			_typeInfo.name = "bytes" + matches[3].str();
		}
		else
		{
			solAssert(width == 0, "Value generator: Invalid width.");
			_typeInfo.type = Type::Bytes;
			_typeInfo.name = "bytes";
		}
	}
	else if (typeString.find("string") != string::npos)
	{
		_typeInfo.type = Type::String;
		_typeInfo.name = "string";
	}
	else
	{
		std::string baseType = matches[2].str();
		solAssert(
			baseType == "int" || baseType == "uint",
			"Value generator: Invalid integer type."
		);
		solAssert(
			width >=8 && width <= 256 && (width % 8 == 0),
			"Value generator: Invalid integer width."
		);
		if (baseType == "int")
		{
			_typeInfo.type = Type::Integer;
			_typeInfo.intType = {true, static_cast<IntegerWidth>(width)};
			_typeInfo.name = "int" + matches[3].str();
		}
		else
		{
			_typeInfo.type = Type::UInteger;
			_typeInfo.intType = {false, static_cast<IntegerWidth>(width)};
			_typeInfo.name = "uint" + matches[3].str();
		}
	}

	for (unsigned i = 4; i < 10; i += 2)
	{
		if (matches[i].matched)
		{
			size_t arraySize;
			if (matches[i + 1].matched)
			{
				std::string arraySizeString = matches[i + 1].str();
				arraySize = stoul(arraySizeString);
				_typeInfo.arrayInfo.push_back({true, arraySize});
				_typeInfo.name += "[" + arraySizeString + "]";
			}
			else
			{
				// TODO: Assign pseudo randomly chosen dynamic size.
				arraySize = 2;
				_typeInfo.arrayInfo.push_back({false, arraySize});
				_typeInfo.name += "[]";
			}
		}
	}
	if (_typeInfo.arrayInfo.empty())
	{
		if (_typeInfo.type == Type::Tuple) {
			cout << "Init tuple" << endl;
			initialiseTuple(_typeInfo);
		}
		else
			initialiseType(_typeInfo);
	}
	else
	{
		cout << "Init array" << endl;
		initialiseArray(_typeInfo.arrayInfo, _typeInfo);
	}
}

void ValueGenerator::tuple(Json::Value const& _type, TypeInfo& _typeInfo)
{
	_typeInfo.name += "(";
	for (auto component = _type.begin(); component != _type.end();)
	{
		TypeInfo componentType;
		typeHelper(*component, componentType);
		_typeInfo.tupleInfo.emplace_back(componentType);
		_typeInfo.name += _typeInfo.tupleInfo.back().name;
		if (++component != _type.end())
		{
			_typeInfo.name += ",";
		}
	}
	_typeInfo.name += ")";
}

ValueGenerator::TypeInfo ValueGenerator::type(Json::Value const& _value)
{
	solAssert(
		_value.isMember("type"),
		"Value generator: Invalid function input parameter type."
	);
	TypeInfo result;
	typeHelper(_value, result);
	return result;
}

std::pair<std::string, std::string> ValueGenerator::type()
{
	std::string typeString = "(";
	std::string valueString = "(";
	std::string separator = "";
	for (auto const& param: m_type)
	{
		auto l = type(param);
		typeString += separator + l.name;
		valueString += separator + l.value;
		if (separator.empty())
			separator = ",";
	}
	return {typeString + ")", valueString + ")"};
}
