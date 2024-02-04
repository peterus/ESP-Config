#include <gtest/gtest.h>

#include "ESP-Config.h"

constexpr unsigned int MAX_BUFFER = 255;

TEST(boolean, save_true) {
  DynamicJsonDocument doc(50);

  BoolElement b("test", true);
  JsonObject  obj = doc.to<JsonObject>();
  b.save(obj);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":true}");
}

TEST(boolean, save_false) {
  DynamicJsonDocument doc(50);

  BoolElement b("test", false);
  JsonObject  obj = doc.to<JsonObject>();
  b.save(obj);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":false}");
}

TEST(boolean, load_true) {
  DynamicJsonDocument doc(50);

  BoolElement b("test", false);

  char buffer[] = "{\"test\":true}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(b.load(obj), SUCCESS);

  ASSERT_TRUE(b.get());
}

TEST(boolean, load_false) {
  DynamicJsonDocument doc(50);

  BoolElement b("test", true);

  char buffer[] = "{\"test\":false}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(b.load(obj), SUCCESS);

  ASSERT_FALSE(b.get());
}
