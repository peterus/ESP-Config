#ifndef CONFIG_STRUCTURE_H_
#define CONFIG_STRUCTURE_H_

#include <Arduino.h>
#include <list>

class ConfigBase;
class ConfigSection;

class ConfigPage {
public:
  ConfigPage(const String &internalName, const String &visibleName, const String &description = "") : _internalName(internalName), _visibleName(visibleName), _description(description) {
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

  void addSection(ConfigSection *section) {
    _sections.push_back(section);
  }

  std::list<ConfigSection *> getSections() {
    return _sections;
  }

private:
  const String               _internalName;
  const String               _visibleName;
  const String               _description;
  std::list<ConfigSection *> _sections;
};

class ConfigSection {
public:
  ConfigSection(const String &internalName, const String &visibleName, const String &description = "") : _internalName(internalName), _visibleName(visibleName), _description(description) {
  }

  ConfigSection(ConfigPage &page, const String &internalName, const String &visibleName, const String &description = "") : _internalName(internalName), _visibleName(visibleName), _description(description) {
    page.addSection(this);
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

  void addConfig(ConfigBase *config) {
    _configs.push_back(config);
  }

  std::list<ConfigBase *> getConfigs() {
    return _configs;
  }

private:
  const String            _internalName;
  const String            _visibleName;
  const String            _description;
  std::list<ConfigBase *> _configs;
};

#endif
