#include <HomeSpan.h>

void setup() {

homeSpan.begin(Category::Lighting, "MyHomeSpan");

}

void loop() {

homeSpan.poll();

}
