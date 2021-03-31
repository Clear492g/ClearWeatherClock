//void Update18b20() {
//  if (millis() - previousMillis >= interval) //如果和前次时间大于等于时间间隔
//  {
//    previousMillis = millis(); //更新时间记录
//
//    float tempC = sensors.getTempCByIndex(0); //获取索引号0的传感器摄氏温度数据
//    if (tempC != DEVICE_DISCONNECTED_C)       //如果获取到的温度正常
//    {
//      Temp_18b20 = tempC;
//    }
//    sensors.requestTemperatures(); //发起新的温度转换
//  }
//}

void WS2812LED(int hours, int minutes, int seconds)
{
  if (seconds != WS2812Second)
  //if (1)
  {
    int NumberToShow[6 ] = {hours / 10, hours % 10, minutes / 10, minutes % 10, seconds / 10, seconds % 10};

    
/***************  动画尝试  ************** */
  if(NumberToShow[5]==0)
  {
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 9 ), RgbColor(0));
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 8 ), RgbColor(ClearRGB[5][1], ClearRGB[5][0], ClearRGB[5][2])); //12:34:56中的1  
     WS2812.Show();delay(50);
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 8 ), RgbColor(0));
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 7 ), RgbColor(ClearRGB[5][1], ClearRGB[5][0], ClearRGB[5][2])); //12:34:56中的1  
     WS2812.Show();delay(50);
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 7 ), RgbColor(0));
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 6 ), RgbColor(ClearRGB[5][1], ClearRGB[5][0], ClearRGB[5][2])); //12:34:56中的1  
     WS2812.Show();delay(50);
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 6 ), RgbColor(0));
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 5 ), RgbColor(ClearRGB[5][1], ClearRGB[5][0], ClearRGB[5][2])); //12:34:56中的1  
     WS2812.Show();delay(50);
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 5 ), RgbColor(0));
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 4 ), RgbColor(ClearRGB[5][1], ClearRGB[5][0], ClearRGB[5][2])); //12:34:56中的1  
     WS2812.Show();delay(50);
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 4 ), RgbColor(0));
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 3 ), RgbColor(ClearRGB[5][1], ClearRGB[5][0], ClearRGB[5][2])); //12:34:56中的1  
     WS2812.Show();delay(50);
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 3 ), RgbColor(0));
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 2 ), RgbColor(ClearRGB[5][1], ClearRGB[5][0], ClearRGB[5][2])); //12:34:56中的1  
     WS2812.Show();delay(50);
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 2 ), RgbColor(0));
     WS2812.SetPixelColor(WS2812GetLEDNoByNum(6, 1 ), RgbColor(ClearRGB[5][1], ClearRGB[5][0], ClearRGB[5][2])); //12:34:56中的1  
     WS2812.Show();delay(50);
    }
    


    WS2812.ClearTo(black);

    for (int i = 1; i <= 6; i++)
    {
      int LED_NoNum = WS2812GetLEDNoByNum(i, NumberToShow[i - 1] );
      WS2812.SetPixelColor(LED_NoNum, RgbColor(ClearRGB[i - 1][1], ClearRGB[i - 1][0], ClearRGB[i - 1][2])); //12:34:56中的1
      WS2812.SetPixelColor(LED_NoNum + 10, RgbColor(ClearRGB[i - 1][1], ClearRGB[i - 1][0], ClearRGB[i - 1][2])); //12:34:56中的1
    }
    WS2812.Show();
    WS2812Second = seconds;

    if (NumberToShow[5] % 5 == 2)
    {
      int err = SimpleDHTErrSuccess;
      if ((err = dht11.read(&DHT11_temperature, &DHT11_humidity, NULL)) != SimpleDHTErrSuccess)
      {
        Serial.print("Read DHT11 failed, err="); Serial.println(err);
       }       
    }


  }

}

int WS2812GetLEDNoByNum(int i, int num)//获取LED序号  i为1-6    num为1-10
{
  if ((i + 1) % 2 == 0)
  {
    switch (num)
    {
      case 0 : return (i - 1) * 20 + 5;
      case 1 : return (i - 1) * 20 + 4;
      case 2 : return (i - 1) * 20 + 6;
      case 3 : return (i - 1) * 20 + 3;
      case 4 : return (i - 1) * 20 + 7;
      case 5 : return (i - 1) * 20 + 2;
      case 6 : return (i - 1) * 20 + 8;
      case 7 : return (i - 1) * 20 + 1;
      case 8 : return (i - 1) * 20 + 9;
      case 9 : return (i - 1) * 20 + 0;
    }
  }
  else
  {
    switch (num)
    {
      case 0 : return (i - 1) * 20 + 4;
      case 1 : return (i - 1) * 20 + 5;
      case 2 : return (i - 1) * 20 + 3;
      case 3 : return (i - 1) * 20 + 6;
      case 4 : return (i - 1) * 20 + 2;
      case 5 : return (i - 1) * 20 + 7;
      case 6 : return (i - 1) * 20 + 1;
      case 7 : return (i - 1) * 20 + 8;
      case 8 : return (i - 1) * 20 + 0;
      case 9 : return (i - 1) * 20 + 9;
    }
  }

}



