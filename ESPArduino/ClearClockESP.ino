#include "ClearDefine.h"

void setup() {
  Serial.begin(115200);
  Serial.println();

  WS2812.Begin();  //ws2812
  WS2812.ClearTo(black);
  WS2812.Show();

  display.init();  // OLED
  display.clear();
  display.display();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setContrast(255);//设置对比度

  //sensors.begin(); //初始化总线18b20
  //sensors.setWaitForConversion(false); //设置为非阻塞模式

  loadConfig();
  if (config.OPEN_WEATHER_MAP_APP_ID == "ClearReset") //重置模式
  {
    strcpy(config.WIFI_SSID, "Clear9700");
    strcpy(config.WIFI_PWD, "918888888");
    strcpy(config.OPEN_WEATHER_MAP_APP_ID, "3213ac05f30cc2f7d8d8da6d2b03f2e8");
    strcpy(config.OPEN_WEATHER_MAP_LOCATION_ID, "1816670");
    for (int i = 0 ; i < 18 ; i++)
    {
      config.EEPROM_Colors[i] = 255;
    }
    config.WS2812Brightnesslimit = 255;
    saveConfig();
  }

  for (int i = 0; i < 6 ; i++)
  {
    ClearRGB[i][0] = config.EEPROM_Colors[i * 3 + 0];
    ClearRGB[i][1] = config.EEPROM_Colors[i * 3 + 1];
    ClearRGB[i][2] = config.EEPROM_Colors[i * 3 + 2];
  }

  WifiConnect();

  ui.setTargetFPS(30);//为了妥协WS2812的性能  可适当降低
  ui.setActiveSymbol(activeSymbole);
  ui.setInactiveSymbol(inactiveSymbole);
  ui.setIndicatorPosition(BOTTOM);// TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorDirection(LEFT_RIGHT);// 定义第一帧在栏中的位置
  ui.setFrameAnimation(SLIDE_LEFT);//动画效果/ SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrames(frames, 3);//页面数量
  ui.setOverlays(overlays, 1);//numberOfOverlays
  ui.init();

  OLED_updateData(&display);
}

void loop() {

  if (millis() - timeSinceLastWUpdate > (1000L * UPDATE_INTERVAL_SECS))
    //如果距离上次更新时间超过设定  millis()函数用于返回的Arduino板开始运行当前程序时的毫秒数。这个数字在大约50天后溢出，即回到零。
  {
    OLED_setReadyForWeatherUpdate();//准备更新标志启用
    timeSinceLastWUpdate = millis();
  }

  if (readyForWeatherUpdate && ui.getUiState()->frameState == FIXED)
  {
    OLED_updateData(&display);
  }

  int remainingTimeBudget = ui.update();//剩余时间预算

  if (remainingTimeBudget > 0)
  {
    SerialSlot();

    //Update18b20();

    now = time(nullptr);//更新WS2812的时间
    struct tm* timeInfo;
    timeInfo = localtime(&now);
    char buff[14];
    sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);
    WS2812LED(timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);



    //    if (     (    (millis() - timeSinceLastWUpdate - (1000L * UPDATE_INTERVAL_SECS))  > (0 - (1000L * 30))    ) && (    (millis() - timeSinceLastWUpdate - (1000L * UPDATE_INTERVAL_SECS))  > ((1000L * 30))    ) )
    //    {
    //      WifiConnect();
    //    }
    //
    //    else
    //    {
    //      if ((WiFi.status() == WL_CONNECTED) && (String(timeInfo->tm_year + 1900) == "1970")   )
    //      {
    //        WiFi.stop;
    //      }
    //    }

    if ((timeInfo->tm_sec % 10) != 9)
    {
      int sensorValue = analogRead(A0);
      float  WS2812Brightness = sensorValue / 1023.0;
      WS2812.SetBrightness((int)((float)(config.WS2812Brightnesslimit) * (1.0 - WS2812Brightness)));
      //WS2812.SetBrightness((int)((float)(config.WS2812Brightnesslimit) * (WS2812Brightness)));
    }




  }





}






