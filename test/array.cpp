#include <gtest/gtest.h>

#include "ESP-Config.h"

constexpr unsigned int MAX_BUFFER = 255;

TEST(array, save) {
  DynamicJsonDocument doc(256);

  ArrayElement<BoolElement> b("test");

  JsonObject obj = doc.to<JsonObject>();
  ASSERT_EQ(b.save(obj), SUCCESS);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":[]}");
}

TEST(array, save_simple) {
  DynamicJsonDocument doc(256);

  ArrayElement<BoolElement> array("test");
  BoolElement               b1("boolean", true);
  BoolElement               b2("foo", false);

  array.add(&b1);
  array.add(&b2);

  JsonObject obj = doc.to<JsonObject>();
  ASSERT_EQ(array.save(obj), SUCCESS);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":[true,false]}");
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

TEST(array, save_class) {
  DynamicJsonDocument doc(256);

  ArrayElement<WifiConfig> array("test");
  WifiConfig               config1;
  WifiConfig               config2;

  config1.SSID.set("SSID_Kurt");
  config1.Password.set("Secret123");

  config2.SSID.set("foo123");
  config2.Password.set("bar123");

  array.add(&config1);
  array.add(&config2);

  JsonObject obj = doc.to<JsonObject>();
  ASSERT_EQ(array.save(obj), SUCCESS);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":[{\"SSID\":\"SSID_Kurt\",\"password\":\"Secret123\"},{\"SSID\":\"foo123\",\"password\":\"bar123\"}]}");
}

TEST(array, load) {
  DynamicJsonDocument doc(256);

  ArrayElement<BoolElement> array("test");

  char buffer[] = "{\"test\":[]}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(array.load(obj), SUCCESS);
  ASSERT_EQ(array.size(), 0);
}

TEST(array, load_simple) {
  DynamicJsonDocument doc(256);

  ArrayElement<BoolElement> array("test");
  BoolElement               b1("boolean", false);
  BoolElement               b2("foo", true);

  array.add(&b1);
  array.add(&b2);

  char buffer[] = "{\"test\":[true,false]}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(array.load(obj), SUCCESS);
  ASSERT_EQ(array.size(), 2);

  ASSERT_TRUE(b1.get());
  ASSERT_FALSE(b2.get());
}

TEST(array, load_class) {
  DynamicJsonDocument doc(256);

  ArrayElement<WifiConfig> array("test");
  WifiConfig               config1;
  WifiConfig               config2;

  array.add(&config1);
  array.add(&config2);

  char buffer[] = "{\"test\":[{\"SSID\":\"SSID_Kurt\",\"password\":\"Secret123\"},{\"SSID\":\"foo123\",\"password\":\"bar123\"}]}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(array.load(obj), SUCCESS);
  ASSERT_EQ(array.size(), 2);

  ASSERT_STREQ(config1.SSID.get().c_str(), "SSID_Kurt");
  ASSERT_STREQ(config1.Password.get().c_str(), "Secret123");

  ASSERT_STREQ(config2.SSID.get().c_str(), "foo123");
  ASSERT_STREQ(config2.Password.get().c_str(), "bar123");
}
