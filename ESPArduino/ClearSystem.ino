void WifiConnect() {

  WiFi.begin(config.WIFI_SSID, config.WIFI_PWD);
  int counter = 0;//一段时间连不上溢出 停止持续重连
  while (WiFi.status() != WL_CONNECTED)
  {
    SerialSlot();
    delay(500);
    Serial.print(".");
    display.clear();
    display.drawString(64, 10, "Connecting to WiFi");
    display.drawXbm(46, 30, 8, 8, counter % 3 == 0 ? activeSymbole : inactiveSymbole);
    display.drawXbm(60, 30, 8, 8, counter % 3 == 1 ? activeSymbole : inactiveSymbole);
    display.drawXbm(74, 30, 8, 8, counter % 3 == 2 ? activeSymbole : inactiveSymbole);
    display.display();
    counter++;
    if (counter >= 20)
    {
      display.clear();
      display.drawString(64, 30, "No WiFi Available! ");
      display.display();
      delay(1500);
      break;
    }
  }
}
void saveConfig() {

  EEPROM.begin(1024);
  uint8_t *p = (uint8_t*)(&config);
  for (int i = 0; i < sizeof(config); i++)
  {
    EEPROM.write(i, *(p + i));
  }
  EEPROM.commit();
}
void loadConfig() {
  EEPROM.begin(1024);
  uint8_t *p = (uint8_t*)(&config);
  for (int i = 0; i < sizeof(config); i++)
  {
    *(p + i) = EEPROM.read(i);
  }
  EEPROM.commit();

}
void SerialSlot() {

  while (Serial.available() > 0)
  {
    SerialRecData += char(Serial.read());
    delay(10);
  }
  if (SerialRecData.length() > 0)
  {
    DeserializationError JsonError = deserializeJson(JosnSerialRecData, String(SerialRecData));

    if (!JsonError) //检查反序列化是否成功
    {
      JsonObject root = JosnSerialRecData.as<JsonObject>();

      /*功能1：修改RGB颜色*/
      if (root.containsKey("RGB11"))
      {
        ClearRGB[0][0] = JosnSerialRecData["RGB11"].as<int>();
        ClearRGB[0][1] = JosnSerialRecData["RGB12"].as<int>();
        ClearRGB[0][2] = JosnSerialRecData["RGB13"].as<int>();

        config.EEPROM_Colors[0]= ClearRGB[0][0];
        config.EEPROM_Colors[1]= ClearRGB[0][1];
        config.EEPROM_Colors[2]= ClearRGB[0][2];
        saveConfig();
      }
      if (root.containsKey("RGB21"))
      {
        ClearRGB[1][0] = JosnSerialRecData["RGB21"].as<int>();
        ClearRGB[1][1] = JosnSerialRecData["RGB22"].as<int>();
        ClearRGB[1][2] = JosnSerialRecData["RGB23"].as<int>();

        config.EEPROM_Colors[3]= ClearRGB[1][0];
        config.EEPROM_Colors[4]= ClearRGB[1][1];
        config.EEPROM_Colors[5]= ClearRGB[1][2];
        saveConfig();
      }
      if (root.containsKey("RGB31"))
      {
        ClearRGB[2][0] = JosnSerialRecData["RGB31"].as<int>();
        ClearRGB[2][1] = JosnSerialRecData["RGB32"].as<int>();
        ClearRGB[2][2] = JosnSerialRecData["RGB33"].as<int>();

        config.EEPROM_Colors[6]= ClearRGB[2][0];
        config.EEPROM_Colors[7]= ClearRGB[2][1];
        config.EEPROM_Colors[8]= ClearRGB[2][2];
        saveConfig();
      }
      if (root.containsKey("RGB41"))
      {
        ClearRGB[3][0] = JosnSerialRecData["RGB41"].as<int>();
        ClearRGB[3][1] = JosnSerialRecData["RGB42"].as<int>();
        ClearRGB[3][2] = JosnSerialRecData["RGB43"].as<int>();

        config.EEPROM_Colors[9]= ClearRGB[3][0];
        config.EEPROM_Colors[10]= ClearRGB[3][1];
        config.EEPROM_Colors[11]= ClearRGB[3][2];
        saveConfig();
      }
      if (root.containsKey("RGB51"))
      {
        ClearRGB[4][0] = JosnSerialRecData["RGB51"].as<int>();
        ClearRGB[4][1] = JosnSerialRecData["RGB52"].as<int>();
        ClearRGB[4][2] = JosnSerialRecData["RGB53"].as<int>();

        config.EEPROM_Colors[12]= ClearRGB[4][0];
        config.EEPROM_Colors[13]= ClearRGB[4][1];
        config.EEPROM_Colors[14]= ClearRGB[4][2];
        saveConfig();
      }
      if (root.containsKey("RGB61"))
      {
        ClearRGB[5][0] = JosnSerialRecData["RGB61"].as<int>();
        ClearRGB[5][1] = JosnSerialRecData["RGB62"].as<int>();
        ClearRGB[5][2] = JosnSerialRecData["RGB63"].as<int>();

        config.EEPROM_Colors[15]= ClearRGB[5][0];
        config.EEPROM_Colors[16]= ClearRGB[5][1];
        config.EEPROM_Colors[17]= ClearRGB[5][2];
        saveConfig();
      }

      /*功能2：修改WIFI/密码*/
      if (root.containsKey("SSID"))
      {
        //读取json节点
        const char * WIFI_SSID_JSON =  JosnSerialRecData["SSID"].as<char*>();
        const char * WIFI_PWD_JSON = JosnSerialRecData["PWD"].as<char*>();

        strcpy(config.WIFI_SSID, WIFI_SSID_JSON);
        strcpy(config.WIFI_PWD, WIFI_PWD_JSON);

        saveConfig();
        delay(100);
        ESP.reset();
      }

      /*功能3：天气API城市和APPID*/
      if (root.containsKey("WEATHER_ID"))
      {
        const char * OPEN_WEATHER_MAP_APP_ID_JSON = JosnSerialRecData["WEATHER_ID"].as<char*>();
        const char * OPEN_WEATHER_MAP_LOCATION_ID_JSON = JosnSerialRecData["LOCATION"].as<char*>();
        
        strcpy(config.OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_APP_ID_JSON);
        strcpy(config.OPEN_WEATHER_MAP_LOCATION_ID, OPEN_WEATHER_MAP_LOCATION_ID_JSON);

        saveConfig();
        delay(100);
        ESP.reset();
      }

      /*功能4：读取当前各个数字的RGB值以及其他参数*/
      if (root.containsKey("ReadRGB"))
      {
        String  RGB_String = "A,";
        for (int i = 0 ; i < 6 ; i++)
        {
          RGB_String = RGB_String   +   String(ClearRGB[i][0])   +   ",";
          RGB_String = RGB_String   +   String(ClearRGB[i][1])   +   ",";
          RGB_String = RGB_String   +   String(ClearRGB[i][2])   +   ",";
        }
        RGB_String = RGB_String   +   config.WIFI_SSID   +   ",";
        RGB_String = RGB_String   +   config.OPEN_WEATHER_MAP_APP_ID   +   ",";
        RGB_String = RGB_String   +   config.OPEN_WEATHER_MAP_LOCATION_ID   +   ",";
        RGB_String = RGB_String   +   String(config.WS2812Brightnesslimit)   +   ",";//亮度限制
        
        RGB_String = RGB_String   + "B";
        Serial.print(RGB_String);//向上位机发出
      }

      /*功能5：调整WS2812 LED的亮度上限*/
      if (root.containsKey("Brightnesslimit"))
      {
       config.WS2812Brightnesslimit =  JosnSerialRecData["Brightnesslimit"].as<int>();
       saveConfig();
      }
      /*功能6：上位机控制重启*/
      if (root.containsKey("EspReboot"))
      {
        delay(100);
        ESP.reset();
      }            
    }
    SerialRecData = "";
  }
}
