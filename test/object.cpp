#include <gtest/gtest.h>

#include "ESP-Config.h"

constexpr unsigned int MAX_BUFFER = 255;

TEST(object, save) {
  DynamicJsonDocument doc(256);

  ObjectElement b("test");

  JsonObject obj = doc.to<JsonObject>();
  ASSERT_EQ(b.save(obj), SUCCESS);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":{}}");
}

TEST(object, save_simple_nested) {
  DynamicJsonDocument doc(256);

  ObjectElement o("test");
  BoolElement   b("boolean", false);
  o.register_element(&b);

  JsonObject obj = doc.to<JsonObject>();
  ASSERT_EQ(o.save(obj), SUCCESS);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":{\"boolean\":false}}");
}

TEST(object, save_simple_nested_nested) {
  DynamicJsonDocument doc(256);

  ObjectElement obj1("obj1");
  IntElement    integer("integer", 42);
  ObjectElement obj2("obj2");
  BoolElement   boolean("boolean", false);
  obj1.register_element(&integer);
  obj1.register_element(&obj2);
  obj2.register_element(&boolean);

  JsonObject obj = doc.to<JsonObject>();
  ASSERT_EQ(obj1.save(obj), SUCCESS);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"obj1\":{\"integer\":42,\"obj2\":{\"boolean\":false}}}");
}

TEST(object, load) {
  DynamicJsonDocument doc(256);

  ObjectElement b("test");

  char buffer[] = "{\"test\":{}}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(b.load(obj), SUCCESS);
}

TEST(object, load_simple_nested) {
  DynamicJsonDocument doc(50);

  ObjectElement o("test");
  BoolElement   b("boolean", true);
  o.register_element(&b);

  char buffer[] = "{\"test\":{\"boolean\":false}}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(o.load(obj), SUCCESS);

  ASSERT_FALSE(b.get());
}

class WifiConfig : public ObjectElement {
public:
  WifiConfig() : ObjectElement("WifiConfig"), SSID("SSID", ""), Password("password", "") {
    register_element(&SSID);
    register_element(&Password);
  }

  StringElement SSID;
  StringElement Password;
};

TEST(object, save_class) {
  DynamicJsonDocument doc(256);

  WifiConfig _config;
  _config.SSID.set("SSID_Kurt");
  _config.Password.set("Secret123");

  JsonObject obj = doc.to<JsonObject>();
  ASSERT_EQ(_config.save(obj), SUCCESS);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"WifiConfig\":{\"SSID\":\"SSID_Kurt\",\"password\":\"Secret123\"}}");
}

TEST(object, load_class) {
  DynamicJsonDocument doc(256);

  WifiConfig _config;

  char buffer[] = "{\"WifiConfig\":{\"SSID\":\"SSID_Kurt\",\"password\":\"Secret123\"}}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(_config.load(obj), SUCCESS);

  ASSERT_STREQ(_config.SSID.get().c_str(), "SSID_Kurt");
  ASSERT_STREQ(_config.Password.get().c_str(), "Secret123");
}
