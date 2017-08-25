void Safe_write(int safepos) {
  myservo.attach(ServoPin);
  myservo.write(safepos);
  delay(500);
  myservo.detach();
}
