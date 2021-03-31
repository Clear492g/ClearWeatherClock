#include <ESP8266WiFi.h>
#include <ESPHTTPClient.h>
#include <JsonListener.h>
#include <time.h>                       // time() ctime()函数
#include <sys/time.h>                   // struct timeval//time结构体
#include "ArduinoJson.h"
//#include <coredecls.h>                  // settimeofday_cb()
#include "SSD1306Wire.h"
#include "OLEDDisplayUi.h"
#include "Wire.h"
#include "OpenWeatherMapCurrent.h"
#include "OpenWeatherMapForecast.h"
#include "WeatherStationFonts.h"
#include "WeatherStationImages.h"
#include <EEPROM.h>
//#include <DallasTemperature.h>//18B20
#include <NeoPixelBus.h>//WS2812
#include <NeoPixelBrightnessBus.h>//WS2812
#include <SimpleDHT.h>

//函数申明
void OLED_drawProgress(OLEDDisplay *display, int percentage, String label);//画进度条 屏幕对象 百分比 文字
void OLED_updateData(OLEDDisplay *display);  //绘制更新天气时间画面进度 屏幕对象
void OLED_drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);//第一页把日期和时间绘制上去
void OLED_drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);//第二页绘制当前天气
void OLED_drawForecast(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);//第三页绘制所有天气预报
void OLED_drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex);//第三页绘制某天的天气状况
void OLED_drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);//绘制底部的时间和气温
void OLED_setReadyForWeatherUpdate();

void WifiConnect();//连接wifi
void saveConfig();//EEPROM Config信息保存
void loadConfig();//EEPROM Config信息加载
void SerialSlot();//串口接收处理
//void Update18b20();//达到某值时更新18B20数据
void WS2812LED(int hours, int minutes, int seconds);//WS2812彩色LED底层
int  WS2812GetLEDNoByNum(int i, int num) ;//获取LED序号  i为1-6    num为1-10


//Config变量
struct config_type
{
  char WIFI_SSID[32];
  char WIFI_PWD[64];
  char OPEN_WEATHER_MAP_APP_ID[256];
  char OPEN_WEATHER_MAP_LOCATION_ID[32] ;
  int EEPROM_Colors[18];
  int WS2812Brightnesslimit;
};

//全局变量
config_type config;//系统配置文件 EEPROM

int UPDATE_INTERVAL_SECS = 120 * 60; // 每120/60=2小时更新一次
time_t now;//time_t类型的变量  now
const int I2C_DISPLAY_ADDRESS = 0x3c;
const int SDC_PIN = D2;
const int SDA_PIN = D3;
String OPEN_WEATHER_MAP_LANGUAGE = "en";//"zh_cn"
const uint8_t MAX_FORECASTS = 3;//预报的天数  包括当天
const boolean IS_METRIC = true;//应该是采用摄氏度
const String WDAY_NAMES[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
const String MONTH_NAMES[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);
OLEDDisplayUi   ui( &display );
OpenWeatherMapCurrentData currentWeather;
OpenWeatherMapCurrent currentWeatherClient;
OpenWeatherMapForecastData forecasts[MAX_FORECASTS];
OpenWeatherMapForecast forecastClient;
bool readyForWeatherUpdate = false;
long long timeSinceLastWUpdate = 0;
FrameCallback frames[] = { OLED_drawDateTime, OLED_drawCurrentWeather, OLED_drawForecast };
OverlayCallback overlays[] = { OLED_drawHeaderOverlay };


String SerialRecData = "";
DynamicJsonDocument  JosnSerialRecData(200);


//unsigned long previousMillis = 0; //毫秒时间记录  18B20
//const long interval = 3000;       //时间间隔 18B20
//float Temp_18b20 = 0;
//OneWire oneWire(D4);       ////1-wire数据总线连接在IO4  18B20
//DallasTemperature sensors(&oneWire); //声明   18B20

const uint16_t PixelCount = 120; 
const uint8_t PixelPin = 2;
//NeoPixelBus<NeoGrbFeature, NeoEsp8266Dma400KbpsMethod> strip(PixelCount, PixelPin);  //D9/GPIO3
//NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart0Ws2812xMethod> strip(PixelCount, PixelPin);//GPIO1/TXDO/TX
NeoPixelBrightnessBus<NeoRgbFeature, NeoEsp8266Uart1Ws2812xMethod> WS2812(PixelCount, PixelPin);//GPIO2/TXD1/D4
RgbColor white(255,255,255);
RgbColor black(0);
int ClearRGB[6][3]={{142,68,173},{52,152,219 },{26,188,156 },
                    {241,196,15},{211,84,0 },{247,249,249 }};
int WS2812Second=0;//最低位置更新再刷  不更新则不刷  节省性能开销

SimpleDHT11 dht11(D1);
byte DHT11_temperature = 0;
byte DHT11_humidity = 0;




