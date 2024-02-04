#include <gtest/gtest.h>

#include "ESP-Config.h"

constexpr unsigned int MAX_BUFFER = 255;

TEST(integer, save_0) {
  DynamicJsonDocument doc(50);

  IntElement b("test", 0);
  JsonObject obj = doc.to<JsonObject>();
  b.save(obj);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":0}");
}

TEST(integer, save_1) {
  DynamicJsonDocument doc(50);

  IntElement b("test", 1);
  JsonObject obj = doc.to<JsonObject>();
  b.save(obj);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":1}");
}

TEST(integer, load_0) {
  DynamicJsonDocument doc(50);

  IntElement b("test", 1);

  char buffer[] = "{\"test\":0}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(b.load(obj), SUCCESS);

  ASSERT_EQ(b.get(), 0);
}

TEST(integer, load_1) {
  DynamicJsonDocument doc(50);

  IntElement b("test", 0);

  char buffer[] = "{\"test\":1}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(b.load(obj), SUCCESS);

  ASSERT_EQ(b.get(), 1);
}
