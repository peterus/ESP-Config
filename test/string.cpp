#include <gtest/gtest.h>

#include "ESP-Config.h"

constexpr unsigned int MAX_BUFFER = 255;

TEST(string, save_0) {
  DynamicJsonDocument doc(50);

  StringElement b("test", "");
  JsonObject    obj = doc.to<JsonObject>();
  b.save(obj);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":\"\"}");
}

TEST(string, save_1) {
  DynamicJsonDocument doc(50);

  StringElement b("test", "Hello World!");
  JsonObject    obj = doc.to<JsonObject>();
  b.save(obj);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":\"Hello World!\"}");
}

TEST(string, load_0) {
  DynamicJsonDocument doc(50);

  StringElement b("test", "Hello World!");

  char buffer[] = "{\"test\":\"\"}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(b.load(obj), SUCCESS);

  ASSERT_STREQ(b.get().c_str(), "");
}

TEST(string, load_1) {
  DynamicJsonDocument doc(50);

  StringElement b("test", "");

  char buffer[] = "{\"test\":\"Hello World!\"}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(b.load(obj), SUCCESS);

  ASSERT_STREQ(b.get().c_str(), "Hello World!");
}
