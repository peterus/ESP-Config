#ifndef CONFIG_TYPES_H_
#define CONFIG_TYPES_H_

#include <Arduino.h>

class ConfigSection;
class ConfigPage;

enum ConfigType
{
  StringType,
  BoolType,
  IntType,
  DecimalType,
};

class ConfigBase {
public:
  ConfigBase(const ConfigType type, const String &internalName, const String &visibleName, const String &description) : _type(type), _internalName(internalName), _visibleName(visibleName), _description(description) {
  }

  ConfigBase(ConfigSection &section, const ConfigType type, const String &internalName, const String &visibleName, const String &description) : _type(type), _internalName(internalName), _visibleName(visibleName), _description(description) {
    section.addConfig(this);
  }

  virtual ~ConfigBase() {
  }

  ConfigType getType() const {
    return _type;
  }

  String getInternalName() const {
    return _internalName;
  }

  String getVisibleName() const {
    return _visibleName;
  }

  String getDesciption() const {
    return _description;
  }

private:
  const ConfigType _type;
  const String     _internalName;
  const String     _visibleName;
  const String     _description;
};

class ConfigString : public ConfigBase {
public:
  ConfigString(const String &internalName, const String &visibleName, const String &description, const String &initValue) : ConfigBase(StringType, internalName, visibleName, description), _value(initValue) {
  }

  ConfigString(ConfigSection &section, const String &internalName, const String &visibleName, const String &description, const String &initValue) : ConfigBase(section, StringType, internalName, visibleName, description), _value(initValue) {
  }

  virtual ~ConfigString() {
  }

  void setValue(const String &val) {
    _value = val;
  }

  String getValue() const {
    return _value;
  }

  String operator()() const {
    return getValue();
  }

private:
  String _value;
};

class ConfigBool : public ConfigBase {
public:
  ConfigBool(const String &internalName, const String &visibleName, const String &description, const bool &initValue) : ConfigBase(BoolType, internalName, visibleName, description), _value(initValue) {
  }

  ConfigBool(ConfigSection &section, const String &internalName, const String &visibleName, const String &description, const bool &initValue) : ConfigBase(section, BoolType, internalName, visibleName, description), _value(initValue) {
  }

  virtual ~ConfigBool() {
  }

  void setValue(const bool &val) {
    _value = val;
  }

  bool getValue() const {
    return _value;
  }

  bool operator()() const {
    return getValue();
  }

private:
  bool _value;
};

class ConfigInt : public ConfigBase {
public:
  ConfigInt(const String &internalName, const String &visibleName, const String &description, const int &initValue) : ConfigBase(IntType, internalName, visibleName, description), _value(initValue) {
  }

  ConfigInt(ConfigSection &section, const String &internalName, const String &visibleName, const String &description, const int &initValue) : ConfigBase(section, IntType, internalName, visibleName, description), _value(initValue) {
  }

  virtual ~ConfigInt() {
  }

  void setValue(const int &val) {
    _value = val;
  }

  long getValue() const {
    return _value;
  }

  long operator()() const {
    return getValue();
  }

private:
  int _value;
};

class ConfigDecimal : public ConfigBase {
public:
  ConfigDecimal(const String &internalName, const String &visibleName, const String &description, const double &initValue, const int precision) : ConfigBase(DecimalType, internalName, visibleName, description), _value(initValue), _precision(precision) {
  }

  ConfigDecimal(ConfigSection &section, const String &internalName, const String &visibleName, const String &description, const double &initValue, const int precision) : ConfigBase(section, DecimalType, internalName, visibleName, description), _value(initValue), _precision(precision) {
  }

  virtual ~ConfigDecimal() {
  }

  void setValue(const double &val) {
    _value = val;
  }

  double getValue() const {
    return _value;
  }

  double operator()() const {
    return getValue();
  }

  int getPrecision() const {
    return _precision;
  }

private:
  double _value;
  int    _precision;
};

#endif
