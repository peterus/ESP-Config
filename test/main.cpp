#include <gtest/gtest.h>

#ifdef ARDUINO
#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(1000);

  ::testing::InitGoogleTest();

  if (RUN_ALL_TESTS())
    ;
}

void loop() {
}

#else
int main(int argc, char **argv) {

  ::testing::InitGoogleTest(&argc, argv);

  if (RUN_ALL_TESTS())
    ;

  return 0;
}
#endif
