#ifndef CONFIG_FACTORY_H_
#define CONFIG_FACTORY_H_

#include <Arduino.h>
#include <SPIFFS.h>
#include <list>
#ifndef CPPCHECK
#include <ArduinoJson.h>
#endif

#include "ConfigTypes.h"

#include "ConfigStructure.h"

class ConfigFactory {
public:
  ConfigFactory() {
  }

  void addPage(ConfigPage *page) {
    _page.push_back(page);
  }

  void addPages(std::list<ConfigPage *> &pages) {
    for (auto page : pages) {
      _page.push_back(page);
    }
  }

  void saveConfig(const String &filename) {
    File file = SPIFFS.open(filename, "w");
    if (!file) {
      return;
    }
    StaticJsonDocument<2048> data;

    for (auto page : _page) {
      for (auto section : page->getSections()) {
        for (auto config : section->getConfigs()) {
          switch (config->getType()) {
          case StringType:
            data[page->getInternalName()][section->getInternalName()][config->getInternalName()] = static_cast<ConfigString const *const>(config)->getValue();
            break;
          case BoolType:
            data[page->getInternalName()][section->getInternalName()][config->getInternalName()] = static_cast<ConfigBool const *const>(config)->getValue();
            break;
          case IntType:
            data[page->getInternalName()][section->getInternalName()][config->getInternalName()] = static_cast<ConfigInt const *const>(config)->getValue();
            break;
          case DecimalType:
            data[page->getInternalName()][section->getInternalName()][config->getInternalName()] = static_cast<ConfigDecimal const *const>(config)->getValue();
            break;
          default:
            Serial.println("Type not known in config save!");
            break;
          }
        }
      }
    }

    serializeJson(data, file);
    serializeJson(data, Serial);
    Serial.println();
    file.close();
  }

  void loadConfig(const String &filename) {
    File file = SPIFFS.open(filename);
    if (!file) {
      return;
    }
    StaticJsonDocument<2048> data;
    DeserializationError     error = deserializeJson(data, file);
    if (error) {
      Serial.print("Error in deserialization of json file: ");
      Serial.println(error.c_str());
      file.close();
      return;
    }
    file.close();

    for (auto page : _page) {
      for (auto section : page->getSections()) {
        for (auto config : section->getConfigs()) {
          switch (config->getType()) {
          case StringType:
            static_cast<ConfigString *>(config)->setValue(data[page->getInternalName()][section->getInternalName()][config->getInternalName()]);
            break;
          case BoolType:
            static_cast<ConfigBool *>(config)->setValue(data[page->getInternalName()][section->getInternalName()][config->getInternalName()]);
            break;
          case IntType:
            static_cast<ConfigInt *>(config)->setValue(data[page->getInternalName()][section->getInternalName()][config->getInternalName()]);
            break;
          case DecimalType:
            static_cast<ConfigDecimal *>(config)->setValue(data[page->getInternalName()][section->getInternalName()][config->getInternalName()]);
            break;
          default:
            Serial.println("Type not known in config load!");
            break;
          }
        }
      }
    }
  }

  void showConfigOnSerial() {
    for (auto page : _page) {
      Serial.print("Page: ");
      Serial.print(page->getInternalName());
      Serial.print(" - ");
      Serial.println(page->getVisibleName());
      for (auto config : page->getSections()) {
        Serial.print("  Config: ");
        Serial.print(config->getInternalName());
        Serial.print(" - ");
        Serial.println(config->getVisibleName());
        for (auto c : config->getConfigs()) {
          Serial.print("    ");
          Serial.print(c->getInternalName());
          Serial.print(" - ");
          Serial.print(c->getVisibleName());
          Serial.print(", ");
          switch (c->getType()) {
          case StringType:
            Serial.print("StringType, ");
            Serial.println(static_cast<ConfigString const *const>(c)->getValue());
            break;
          case BoolType:
            Serial.print("BoolType, ");
            Serial.println(static_cast<ConfigBool const *const>(c)->getValue());
            break;
          case IntType:
            Serial.print("IntType, ");
            Serial.println(static_cast<ConfigInt const *const>(c)->getValue());
            break;
          case DecimalType:
            Serial.print("DecimalType, ");
            Serial.println(static_cast<ConfigDecimal const *const>(c)->getValue());
            break;
          default:
            Serial.println("Type not known in serial output!");
            break;
          }
        }
      }
    }
  }

private:
  std::list<ConfigPage *> _page;
};

#endif
