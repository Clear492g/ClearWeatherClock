void OLED_drawProgress(OLEDDisplay *display, int percentage, String label) 
{
  display->clear();
  display->setTextAlignment(TEXT_ALIGN_CENTER);//设置文本对齐
  display->setFont(ArialMT_Plain_10);//设置字体
  display->drawString(64, 10, label);
  display->drawProgressBar(2, 28, 124, 10, percentage);
  display->display();
}
void OLED_updateData(OLEDDisplay *display) 
{
  if ((WiFi.status() == WL_CONNECTED)) {
    UPDATE_INTERVAL_SECS = 120 * 60;
    OLED_drawProgress(display, 10, "Updating Time...");
    //configTime((config.TZ) * 3600, ((config.DST_MN) * 60), "ntp1.aliyun.com"); //time.windows.com  ntp1.aliyun.com  pool.ntp.org
    configTime((7) * 3600 , (60 * 60), "pool.ntp.org", "time.nist.gov");
    delay(1000);//
    OLED_drawProgress(display, 30, "Updating Weather...");
    currentWeatherClient.setMetric(IS_METRIC);
    currentWeatherClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
    currentWeatherClient.updateCurrentById(&currentWeather, String(config.OPEN_WEATHER_MAP_APP_ID) , String(config.OPEN_WEATHER_MAP_LOCATION_ID));
    OLED_drawProgress(display, 50, "Updating Forecasts...");
    forecastClient.setMetric(IS_METRIC);
    forecastClient.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
    uint8_t allowedHours[] = {12};
    forecastClient.setAllowedHours(allowedHours, sizeof(allowedHours));
    forecastClient.updateForecastsById(forecasts, String(config.OPEN_WEATHER_MAP_APP_ID), String(config.OPEN_WEATHER_MAP_LOCATION_ID), MAX_FORECASTS);
    readyForWeatherUpdate = false;
    OLED_drawProgress(display, 100, "Powered by Clear2020");
    delay(2000);

    now = time(nullptr);
    struct tm* timeInfo;
    timeInfo = localtime(&now);
    //    if ( (String(currentWeather.temp, 1) == "0.0") || (String(timeInfo->tm_year + 1900) == "1970") )
    //    {
    //      //ESP.restart();
    //    }
    if ( String(currentWeather.temp, 1) == "0.0" )
    {
      //ESP.restart();
    }

    else
    {
      delay(500);//我记的当时在这放个延时 作用是等待串口数据
    }

    if ((WiFi.status() != WL_CONNECTED))  UPDATE_INTERVAL_SECS = 30;
  }

  else UPDATE_INTERVAL_SECS = 30;

}
void OLED_drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y)
{
  now = time(nullptr);
  struct tm* timeInfo;
  timeInfo = localtime(&now);
  char buff[16];

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  String date = WDAY_NAMES[timeInfo->tm_wday];

  sprintf_P(buff, PSTR("%s, %04d-%02d-%02d"), WDAY_NAMES[timeInfo->tm_wday].c_str(), timeInfo->tm_year + 1900,  timeInfo->tm_mon + 1, timeInfo->tm_mday);
  display->drawString(64 + x, 0 + y, String(buff));

  display->setFont(ArialMT_Plain_24);
  sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  display->drawString(64 + x, 15 + y, String(buff));
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}
void OLED_drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
{
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 + x, 34 + y, currentWeather.description);

  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = String(currentWeather.temp, 1) + (IS_METRIC ? "°C" : "°F");
  display->drawString(60 + x, 5 + y, temp);

  display->setFont(Meteocons_Plain_36);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(32 + x, 0 + y, currentWeather.iconMeteoCon);
}
void OLED_drawForecast(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
{
  if ((WiFi.status() != WL_CONNECTED))  UPDATE_INTERVAL_SECS = 30;
  OLED_drawForecastDetails(display, x, y, 0);
  OLED_drawForecastDetails(display, x + 44, y, 1);
  OLED_drawForecastDetails(display, x + 88, y, 2);
}
void OLED_drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex) 
{
  time_t observationTimestamp = forecasts[dayIndex].observationTime;
  struct tm* timeInfo;
  timeInfo = localtime(&observationTimestamp);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 20, y, WDAY_NAMES[timeInfo->tm_wday]);

  display->setFont(Meteocons_Plain_21);
  display->drawString(x + 20, y + 12, forecasts[dayIndex].iconMeteoCon);
  String temp = String(forecasts[dayIndex].temp, 0) + (IS_METRIC ? "°C" : "°F");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 20, y + 34, temp);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}
void OLED_drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) 
{
  now = time(nullptr);
  struct tm* timeInfo;
  timeInfo = localtime(&now);
  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min);

  display->setColor(WHITE);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  //display->drawString(0, 54, String(buff));//显示时间
  display->drawString(0, 54, String(DHT11_humidity)+"%");
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  //String temp = String(currentWeather.temp, 1) + (IS_METRIC ? "°C" : "°F");
  //String temp = String(Temp_18b20, 1) + (IS_METRIC ? "°C" : "°F");
  display->drawString(128, 54, String(DHT11_temperature-1)+ (IS_METRIC ? "°C" : "°F"));
  display->drawHorizontalLine(0, 52, 128);
}
void OLED_setReadyForWeatherUpdate() 
{
  //Serial.println("Setting readyForUpdate to true");
  readyForWeatherUpdate = true;
}
