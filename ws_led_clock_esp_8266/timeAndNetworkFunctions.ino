void makeAccessPoint() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Digital Clock");
}

void connectWifi() {
  if (WiFi.status() == WL_CONNECTED) return;
  //Manual Wifi
  Serial.printf("Connecting to WiFi %s/%s", WIFI_SSID.c_str(), WIFI_PASS.c_str());
  // WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.hostname(WIFI_HOSTNAME);
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (i > 80) i = 0;
    // drawProgress(i, "Connecting to WiFi '" + String(WIFI_SSID.c_str()) + "'");
    i += 10;
    Serial.print(".");
  }
  // drawProgress(100, "Connected to WiFi '" + String(WIFI_SSID.c_str()) + "'");
  Serial.println("connected.");
  Serial.printf("Connected, IP address: %s/%s\n", WiFi.localIP().toString().c_str(), WiFi.subnetMask().toString().c_str());  //Get ip and subnet mask
  Serial.printf("Connected, MAC address: %s\n", WiFi.macAddress().c_str());                                                  //Get the local mac address
}

void initTime() {
  Serial.println("Initializing time...");
  time_t now;

  Serial.printf("Configuring time for timezone %s\n", TIMEZONE.c_str());
  configTime(TIMEZONE.c_str(), NTP_SERVERS);
  int i = 1;
  while ((now = time(nullptr)) < NTP_MIN_VALID_EPOCH) {
    if (i > 80) i = 0;
    // drawProgress(i, "Updating time...");
    Serial.println("initTime: " + String(now));
    delay(300);
    yield();
    i += 10;
  }
  // drawProgress(100, "Time synchronized");
  Serial.println();

  printf("Local time: %s", asctime(localtime(&now)));  // print formated local time, same as ctime(&now)
  printf("UTC time:   %s", asctime(gmtime(&now)));     // print formated GMT/UTC time
}
