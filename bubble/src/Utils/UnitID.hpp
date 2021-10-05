// Copyright 2019-2021 Cambridge Quantum Computing
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _TKET_UnitID_H_
#define _TKET_UnitID_H_

/**
 * @file
 * @brief Named registers of arrays of (quantum or classical) nodes
 */

#include <map>
#include <optional>
#include <regex>
#include <set>
#include <sstream>
#include <string>

namespace tket {

/** Type of information held */
enum class UnitType { Qubit };

/** The type and dimension of a register */
typedef std::pair<UnitType, unsigned> register_info_t;

typedef std::optional<register_info_t> opt_reg_info_t;

const std::string &q_default_reg();
const std::string &c_default_reg();
const std::string &node_default_reg();

/** Conversion invalid */
class InvalidUnitConversion : public std::logic_error {
 public:
  InvalidUnitConversion(const std::string &name, const std::string &new_type)
      : std::logic_error("Cannot convert " + name + " to " + new_type) {}
};

/**
 * Location holding a bit or qubit of information
 *
 * Each location has a name (signifying the 'register' to which it belongs) and
 * an index within that register (which may be multi-dimensional).
 */
class UnitID {
 public:
  UnitID() : data_(std::make_shared<UnitData>()) {}

  /** String representation including name and index */
  std::string repr() const;

  /** Register name */
  std::string reg_name() const { return data_->name_; }

  /** Index dimension */
  unsigned reg_dim() const { return data_->index_.size(); }

  /** Index */
  std::vector<unsigned> index() const { return data_->index_; }

  /** Unit type */
  UnitType type() const { return data_->type_; }

  /** Register dimension and type */
  register_info_t reg_info() const { return {type(), reg_dim()}; }

  bool operator<(const UnitID &other) const {
    int n = data_->name_.compare(other.data_->name_);
    if (n > 0) return false;
    if (n < 0) return true;
    return data_->index_ < other.data_->index_;
  }
  bool operator==(const UnitID &other) const {
    return (this->data_->name_ == other.data_->name_) &&
           (this->data_->index_ == other.data_->index_);
  }
  bool operator!=(const UnitID &other) const { return !(*this == other); }

 protected:
  UnitID(
      const std::string &name, const std::vector<unsigned> &index,
      UnitType type)
      : data_(std::make_shared<UnitData>(name, index, type)) {}

 private:
  struct UnitData {
    std::string name_;
    std::vector<unsigned> index_;
    UnitType type_;

    UnitData() : name_(), index_(), type_(UnitType::Qubit) {}
    UnitData(
        const std::string &name, const std::vector<unsigned> &index,
        UnitType type)
        : name_(name), index_(index), type_(type) {
    }
  };
  std::shared_ptr<UnitData> data_;
};

/** Location holding a qubit */
class Qubit : public UnitID {
 public:
  Qubit() : UnitID("", {}, UnitType::Qubit) {}

  /** Qubit in default register */
  explicit Qubit(unsigned index)
      : UnitID(q_default_reg(), {index}, UnitType::Qubit) {}

  /** Named register with no index */
  explicit Qubit(const std::string &name) : UnitID(name, {}, UnitType::Qubit) {}

  /** Named register with a one-dimensional index */
  Qubit(const std::string &name, unsigned index)
      : UnitID(name, {index}, UnitType::Qubit) {}

  /** Named register with a two-dimensional index */
  Qubit(const std::string &name, unsigned row, unsigned col)
      : UnitID(name, {row, col}, UnitType::Qubit) {}

  /** Named register with a three-dimensional index */
  Qubit(const std::string &name, unsigned row, unsigned col, unsigned layer)
      : UnitID(name, {row, col, layer}, UnitType::Qubit) {}

  /** Named register with a multi-dimensional index */
  Qubit(const std::string &name, std::vector<unsigned> index)
      : UnitID(name, index, UnitType::Qubit) {}

  /** Copy constructor */
  explicit Qubit(const UnitID &other) : UnitID(other) {
    if (other.type() != UnitType::Qubit) {
      throw InvalidUnitConversion(other.repr(), "Qubit");
    }
  }
};

}  // namespace tket

#endif
