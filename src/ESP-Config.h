#ifndef ESP_CONFIG_H_
#define ESP_CONFIG_H_

#include <list>
#include <vector>

#include <Arduino.h>

#include <ArduinoJson.h>

enum ESPConfigStatus {
  SUCCESS,
  ERROR_NOT_ENOUGH_SPACE,
  ERROR_ARRAY_SIZE_NOT_MATCH,
};

enum ElementType {
  BOOL,
  INT,
  DOUBLE,
  STRING,
  OBJECT,
  ARRAY,
};

class Element {
public:
  Element(const ElementType type, const String &name);
  virtual ~Element();

  ElementType getType() const;
  String      getName() const;

  virtual ESPConfigStatus save(JsonObject &doc) = 0;
  virtual ESPConfigStatus load(JsonObject &doc) = 0;

  virtual ESPConfigStatus save(JsonArray &array)     = 0;
  virtual ESPConfigStatus load(JsonVariant &variant) = 0;

protected:
  const ElementType _type;
  const String      _name;
};

class BoolElement : public Element {
public:
  BoolElement(const String &name, const bool defaultData);

  void set(const bool data);
  bool get() const;

  ESPConfigStatus save(JsonObject &doc) override;
  ESPConfigStatus load(JsonObject &doc) override;

  ESPConfigStatus save(JsonArray &array) override;
  ESPConfigStatus load(JsonVariant &variant) override;

private:
  bool _data;
};

class IntElement : public Element {
public:
  IntElement(const String &name, const int defaultData);

  void set(const int data);
  int  get() const;

  ESPConfigStatus save(JsonObject &doc) override;
  ESPConfigStatus load(JsonObject &doc) override;

  ESPConfigStatus save(JsonArray &array) override;
  ESPConfigStatus load(JsonVariant &variant) override;

private:
  int _data;
};

class DoubleElement : public Element {
public:
  DoubleElement(const String &name, const double defaultData);

  void   set(const double data);
  double get() const;

  ESPConfigStatus save(JsonObject &doc) override;
  ESPConfigStatus load(JsonObject &doc) override;

  ESPConfigStatus save(JsonArray &array) override;
  ESPConfigStatus load(JsonVariant &variant) override;

private:
  double _data;
};

class StringElement : public Element {
public:
  StringElement(const String &name, const String &defaultData);

  void   set(const String &data);
  String get() const;

  ESPConfigStatus save(JsonObject &doc) override;
  ESPConfigStatus load(JsonObject &doc) override;

  ESPConfigStatus save(JsonArray &array) override;
  ESPConfigStatus load(JsonVariant &variant) override;

private:
  String _data;
};

class ObjectElement : public Element {
public:
  ObjectElement(const String &name);

  void register_element(Element *element);

  ESPConfigStatus save(JsonObject &doc) override;
  ESPConfigStatus load(JsonObject &doc) override;

  ESPConfigStatus save(JsonArray &array) override;
  ESPConfigStatus load(JsonVariant &variant) override;

private:
  std::list<Element *> _elements;
};

template <typename T> class ArrayElement : public Element {
public:
  ArrayElement(const String &name) : Element(ARRAY, name) {
  }

  void add(T *object) {
    _objects.push_back(object);
  }

  T get(size_t i) {
    return _objects[i];
  }

  void remove(size_t i) {
    _objects.remove(i);
  }

  size_t size() const {
    return _objects.size();
  }

  ESPConfigStatus save(JsonObject &doc) override {
    JsonArray array = doc.createNestedArray(_name);
    if (array.isNull()) {
      Serial.println("Not enough space in JSON document!");
      return ERROR_NOT_ENOUGH_SPACE;
    }
    for (Element *elem : _objects) {
      auto status = elem->save(array);
      if (status != SUCCESS) {
        return status;
      }
    }
    return SUCCESS;
  }

  ESPConfigStatus load(JsonObject &doc) override {
    JsonArray array = doc[_name];
    if (array.size() != _objects.size()) {
      return ERROR_ARRAY_SIZE_NOT_MATCH;
    }
    for (size_t i = 0; i < array.size(); i++) {
      Element    *elem   = _objects[i];
      JsonVariant obj    = array[i].as<JsonVariant>();
      auto        status = elem->load(obj);
      if (status != SUCCESS) {
        return status;
      }
    }
    return SUCCESS;
  }

  ESPConfigStatus save(JsonArray &array) override {
    return SUCCESS;
  }

  ESPConfigStatus load(JsonVariant &variant) override {
    return SUCCESS;
  }

private:
  std::vector<T *> _objects;
};

#endif
