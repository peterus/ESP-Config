#ifndef CONFIG_HTML_H_
#define CONFIG_HTML_H_

#include <Arduino.h>
#include <list>

extern "C" {
#include "esp_spiffs.h"
}

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "ConfigStructure.h"

class ConfigHTML {
public:
  ConfigHTML() : _saved(false) {
  }

  void addPage(ConfigPage *page) {
    _page.push_back(page);
  }

  void addPages(std::list<ConfigPage *> &pages) {
    for (auto page : pages) {
      _page.push_back(page);
    }
  }

  void registerWebServer(AsyncWebServer &server, const String &path = "/") {
    for (auto page : _page) {
      server.on((path + page->getInternalName()).c_str(), HTTP_GET, [this, page, path](AsyncWebServerRequest *request) {
        request->send(200, "text/html", this->createHTMLForm(page, path));
      });

      server.on((path + page->getInternalName() + "/save").c_str(), HTTP_POST, [this, page](AsyncWebServerRequest *request) {
        request->send(200, "text/html", this->handlePostForm(page, request));
      });
    }

    server.on((path + "form.css").c_str(), HTTP_GET, [](AsyncWebServerRequest *request) {
      if (esp_spiffs_mounted(NULL) && SPIFFS.exists("/form.css")) {
        request->send(SPIFFS, "/form.css", "text/css");
      } else {
        String css = "h1{margin-top:0}ul{margin:0;padding:0;list-style:none}form{margin:0 auto;width:400px;padding:1em;border:1px solid #CCC;border-radius:1em}div+div{margin-top:1em}label span{display:inline-block;width:120px;text-align:right}input,textarea{font:1em sans-serif;width:250px;box-sizing:border-box;border:1px solid "
                     "#999}input[type=checkbox],input[type=radio]{width:auto;border:none}input:focus,textarea:focus{border-color:#000}textarea{vertical-align:top;height:5em;resize:vertical}fieldset{width:250px;box-sizing:border-box;margin-left:136px;border:1px solid #999}button{margin:20px 0 0 124px}small{padding-left: 120px;}label{position:relative}label em{position:absolute;right:5px;top:20px}";
        request->send(200, "text/css", css);
      }
    });
  }

  String createHTMLForm(ConfigPage *page, const String &path) {
    String form          = createHTMLcode(page, path);
    String form_sections = "";

    for (auto section : page->getSections()) {
      String form_section = createHTMLSection(section);
      String form_configs = "";
      for (auto c : section->getConfigs()) {
        String form_config = createHTMLConfig(c);
        switch (c->getType()) {
        case StringType:
          form_config.replace("$INPUT$", createHTMLString(static_cast<ConfigString const *const>(c)));
          break;
        case BoolType:
          form_config.replace("$INPUT$", createHTMLBool(static_cast<ConfigBool const *const>(c)));
          break;
        case IntType:
          form_config.replace("$INPUT$", createHTMLInt(static_cast<ConfigInt const *const>(c)));
          break;
        case DecimalType:
          form_config.replace("$INPUT$", createHTMLDecimal(static_cast<ConfigDecimal const *const>(c)));
          break;
        default:
          Serial.println("Type not known in HTML!");
          break;
        }
        form_configs += form_config;
      }
      form_section.replace("$FIELDS$", form_configs);
      form_sections += form_section;
    }
    form.replace("$FORM$", form_sections);

    return form;
  }

  String handlePostForm(ConfigPage *page, AsyncWebServerRequest *request) {
    for (auto section : page->getSections()) {
      for (auto config : section->getConfigs()) {
        Serial.print(config->getInternalName());
        Serial.print(": ");
        if (config->getType() == BoolType) { // special case for bool
          if (request->hasParam(config->getInternalName(), true)) {
            static_cast<ConfigBool *>(config)->setValue(true);
          } else {
            static_cast<ConfigBool *>(config)->setValue(false);
          }
          continue;
        }
        if (request->hasParam(config->getInternalName(), true)) {
          Serial.println(request->getParam(config->getInternalName(), true)->value());
          switch (config->getType()) {
          case StringType:
            static_cast<ConfigString *>(config)->setValue(request->getParam(config->getInternalName(), true)->value());
            break;
          case IntType:
            static_cast<ConfigInt *>(config)->setValue(request->getParam(config->getInternalName(), true)->value().toInt());
            break;
          case DecimalType:
            static_cast<ConfigDecimal *>(config)->setValue(request->getParam(config->getInternalName(), true)->value().toDouble());
            break;
          default:
            Serial.println("Type not known in post form!");
            break;
          }
        } else {
          Serial.println("NOT GIVEN!");
        }
      }
    }
    _saved = true;
    return createHTMLcode(page);
  }

  bool wasSaved() {
    if (_saved) {
      _saved = false;
      return true;
    }
    return false;
  }

private:
  std::list<ConfigPage *> _page;
  bool                    _saved;

  const String _HTMLCode        = "<!DOCTYPE html><head><meta charset=\"utf-8\"><title>$VISIBLE_NAME$</title><link href=\"/form.css\" rel=\"stylesheet\"></head><body><form action=\"$POST_URL$\" method=\"post\"><h1>$VISIBLE_NAME$</h1>\n$FORM$<input type=\"submit\" value=\"Submit\"></form></body></html>";
  const String _HTMLCodeSaved   = "<!DOCTYPE html><head><meta charset=\"utf-8\"><meta http-equiv=\"refresh\" content=\"3;url=.\" /><title>$VISIBLE_NAME$</title><link href=\"/form.css\" rel=\"stylesheet\"></head><body><h1>$VISIBLE_NAME$</h1><h2>Saved, redirecting in 3 seconds!</h2></body></html>";
  const String _HTMLSection     = "<section><h2>$VISIBLE_NAME$</h2>\n$FIELDS$</section>\n";
  const String _HTMLConfig      = "<p><label for=\"$INTERNAL_NAME$\"><span>$VISIBLE_NAME$: </span></label>$INPUT$<br><small>$DESC$</small></p>\n";
  const String _HTMLString      = "<input type=\"text\" id=\"$INTERNAL_NAME$\" name=\"$INTERNAL_NAME$\" value=\"$VALUE$\">";
  const String _HTMLBoolChecked = "<input type=\"checkbox\" id=\"$INTERNAL_NAME$\" name=\"$INTERNAL_NAME$\" value=\"1\" checked>";
  const String _HTMLBool        = "<input type=\"checkbox\" id=\"$INTERNAL_NAME$\" name=\"$INTERNAL_NAME$\" value=\"1\">";
  const String _HTMLInt         = "<input type=\"number\" id=\"$INTERNAL_NAME$\" name=\"$INTERNAL_NAME$\" value=\"$VALUE$\">";
  const String _HTMLDecimal     = "<input type=\"text\" id=\"$INTERNAL_NAME$\" name=\"$INTERNAL_NAME$\" value=\"$VALUE$\">";

  String createHTMLcode(ConfigPage const *const page, const String &path) {
    String html = _HTMLCode;
    html.replace("$VISIBLE_NAME$", page->getVisibleName());
    html.replace("$POST_URL$", path + page->getInternalName() + "/save");
    return html;
  }

  String createHTMLcode(ConfigPage const *const page) {
    String html = _HTMLCodeSaved;
    html.replace("$VISIBLE_NAME$", page->getVisibleName());
    return html;
  }

  String createHTMLSection(ConfigSection const *const section) {
    String html = _HTMLSection;
    html.replace("$VISIBLE_NAME$", section->getVisibleName());
    return html;
  }

  String createHTMLConfig(ConfigBase const *const config) {
    String html = _HTMLConfig;
    html.replace("$VISIBLE_NAME$", config->getVisibleName());
    html.replace("$INTERNAL_NAME$", config->getInternalName());
    html.replace("$DESC$", config->getDesciption());
    return html;
  }

  String createHTMLString(ConfigString const *const config) {
    String html = _HTMLString;
    html.replace("$INTERNAL_NAME$", config->getInternalName());
    html.replace("$VALUE$", config->getValue());
    return html;
  }

  String createHTMLBool(ConfigBool const *const config) {
    String html = _HTMLBool;
    if (config->getValue()) {
      html = _HTMLBoolChecked;
    }
    html.replace("$INTERNAL_NAME$", config->getInternalName());
    return html;
  }

  String createHTMLInt(ConfigInt const *const config) {
    String html = _HTMLInt;
    html.replace("$INTERNAL_NAME$", config->getInternalName());
    html.replace("$VALUE$", String(config->getValue()));
    return html;
  }

  String createHTMLDecimal(ConfigDecimal const *const config) {
    String html = _HTMLDecimal;
    html.replace("$INTERNAL_NAME$", config->getInternalName());
    html.replace("$VALUE$", String(config->getValue(), config->getPrecision()));
    return html;
  }
};

#endif
