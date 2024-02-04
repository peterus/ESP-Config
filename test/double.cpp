#include <gtest/gtest.h>

#include "ESP-Config.h"

constexpr unsigned int MAX_BUFFER = 255;

TEST(double, save_0) {
  DynamicJsonDocument doc(50);

  DoubleElement b("test", 0.0);
  JsonObject    obj = doc.to<JsonObject>();
  b.save(obj);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":0}");
}

TEST(double, save_1) {
  DynamicJsonDocument doc(50);

  DoubleElement b("test", 0.1);
  JsonObject    obj = doc.to<JsonObject>();
  b.save(obj);

  char buffer[MAX_BUFFER];
  serializeJson(doc, buffer, MAX_BUFFER);

  ASSERT_STREQ(buffer, "{\"test\":0.1}");
}

TEST(double, load_0) {
  DynamicJsonDocument doc(50);

  DoubleElement b("test", 1.0);

  char buffer[] = "{\"test\":0}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(b.load(obj), SUCCESS);

  ASSERT_EQ(b.get(), 0.0);
}

TEST(double, load_1) {
  DynamicJsonDocument doc(50);

  DoubleElement b("test", 0.0);

  char buffer[] = "{\"test\":0.1}";

  deserializeJson(doc, buffer);
  JsonObject obj = doc.as<JsonObject>();
  ASSERT_EQ(b.load(obj), SUCCESS);

  ASSERT_EQ(b.get(), 0.1);
}
