#include "ESP-Config.h"

Element::Element(const ElementType type, const String &name) : _type(type), _name(name) {
}

Element::~Element() {
}

ElementType Element::getType() const {
  return _type;
}

String Element::getName() const {
  return _name;
}

BoolElement::BoolElement(const String &name, const bool defaultData) : Element(BOOL, name), _data(defaultData) {
}

void BoolElement::set(const bool data) {
  _data = data;
}

bool BoolElement::get() const {
  return _data;
}

ESPConfigStatus BoolElement::save(JsonObject &doc) {
  doc[_name] = _data;
  if (doc[_name].isNull()) {
    Serial.println("Not enough space in JSON document!");
    return ERROR_NOT_ENOUGH_SPACE;
  }
  return SUCCESS;
}

ESPConfigStatus BoolElement::load(JsonObject &doc) {
  _data = doc[_name] | _data;
  return SUCCESS;
}

ESPConfigStatus BoolElement::save(JsonArray &array) {
  array.add(_data);
  return SUCCESS;
}

ESPConfigStatus BoolElement::load(JsonVariant &variant) {
  _data = variant;
  return SUCCESS;
}

IntElement::IntElement(const String &name, const int defaultData) : Element(INT, name), _data(defaultData) {
}

void IntElement::set(const int data) {
  _data = data;
}

int IntElement::get() const {
  return _data;
}

ESPConfigStatus IntElement::save(JsonObject &doc) {
  doc[_name] = _data;
  if (doc[_name].isNull()) {
    Serial.println("Not enough space in JSON document!");
    return ERROR_NOT_ENOUGH_SPACE;
  }
  return SUCCESS;
}

ESPConfigStatus IntElement::load(JsonObject &doc) {
  _data = doc[_name] | _data;
  return SUCCESS;
}

ESPConfigStatus IntElement::save(JsonArray &array) {
  array.add(_data);
  return SUCCESS;
}

ESPConfigStatus IntElement::load(JsonVariant &variant) {
  _data = variant;
  return SUCCESS;
}

DoubleElement::DoubleElement(const String &name, const double defaultData) : Element(DOUBLE, name), _data(defaultData) {
}

void DoubleElement::set(const double data) {
  _data = data;
}

double DoubleElement::get() const {
  return _data;
}

ESPConfigStatus DoubleElement::save(JsonObject &doc) {
  doc[_name] = _data;
  return SUCCESS;
}

ESPConfigStatus DoubleElement::load(JsonObject &doc) {
  _data = doc[_name] | _data;
  return SUCCESS;
}

ESPConfigStatus DoubleElement::save(JsonArray &array) {
  array.add(_data);
  return SUCCESS;
}

ESPConfigStatus DoubleElement::load(JsonVariant &variant) {
  _data = variant;
  return SUCCESS;
}

StringElement::StringElement(const String &name, const String &defaultData) : Element(STRING, name), _data(defaultData) {
}

void StringElement::set(const String &data) {
  _data = data;
}

String StringElement::get() const {
  return _data;
}

ESPConfigStatus StringElement::save(JsonObject &doc) {
  doc[_name] = _data;
  return SUCCESS;
}

ESPConfigStatus StringElement::load(JsonObject &doc) {
  if (doc.containsKey(_name)) {
    _data = doc[_name].as<String>();
  }
  return SUCCESS;
}

ESPConfigStatus StringElement::save(JsonArray &array) {
  array.add(_data);
  return SUCCESS;
}

ESPConfigStatus StringElement::load(JsonVariant &variant) {
  _data = variant.as<String>();
  return SUCCESS;
}

ObjectElement::ObjectElement(const String &name) : Element(OBJECT, name) {
}

void ObjectElement::register_element(Element *element) {
  _elements.push_back(element);
}

ESPConfigStatus ObjectElement::save(JsonObject &doc) {
  JsonObject obj = doc.createNestedObject(_name);
  if (obj.isNull()) {
    Serial.println("Not enough space in JSON document!");
    return ERROR_NOT_ENOUGH_SPACE;
  }
  for (Element *elem : _elements) {
    auto status = elem->save(obj);
    if (status != SUCCESS) {
      return status;
    }
  }
  return SUCCESS;
}

ESPConfigStatus ObjectElement::load(JsonObject &doc) {
  JsonObject obj = doc[_name];
  for (Element *elem : _elements) {
    auto status = elem->load(obj);
    if (status != SUCCESS) {
      return status;
    }
  }
  return SUCCESS;
}

ESPConfigStatus ObjectElement::save(JsonArray &array) {
  JsonObject obj = array.createNestedObject();
  if (obj.isNull()) {
    Serial.println("Not enough space in JSON document!");
    return ERROR_NOT_ENOUGH_SPACE;
  }
  for (Element *elem : _elements) {
    auto status = elem->save(obj);
    if (status != SUCCESS) {
      return status;
    }
  }
  return SUCCESS;
}

ESPConfigStatus ObjectElement::load(JsonVariant &variant) {
  JsonObject obj = variant.as<JsonObject>();
  for (Element *elem : _elements) {
    auto status = elem->load(obj);
    if (status != SUCCESS) {
      return status;
    }
  }
  return SUCCESS;
}
