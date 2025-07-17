#include <Arduino.h>
#include <SPI.h>
#include "ST73XX_UI.h"
#include "ST7305_2p9_BW_DisplayDriver.h"
#include "U8g2_for_ST73XX.h"
#include "MyIMG.h"  //图标文件
#include "OneButton.h"


#include "SdFat.h"
#include <vector>
#include <iostream>
#include <string>
#include <Regexp.h>  //正则库
#include <EEPROM.h>


//#include "Audio.h"
#include "SD.h"
#include "FS.h"


//*****板子音频接线*******
#define I2S_POW       2
#define I2S_DOUT      41
#define I2S_BCLK      42
#define I2S_LRC       40

//板子sd卡接线
#define SDSCK_PIN  39
#define SDMISO_PIN 38
#define SDMOSI_PIN  48
#define SDCS_PIN 47


//板子显示接线
#define DC_PIN  18
#define RES_PIN 13
#define CS_PIN  17
#define SCLK_PIN 16
#define SDIN_PIN 15


ST7305_2p9_BW_DisplayDriver display(DC_PIN, RES_PIN, CS_PIN, SCLK_PIN, SDIN_PIN, SPI);
U8G2_FOR_ST73XX u8g2Fonts;

uint8_t gongneng = 0;  //切换按键所属功能
uint8_t dirtubiao = 0; // 文件夹图标数量


//****** ↓↓↓温度湿度传感器I2C↓↓↓ ******
#include "Wire.h"
#include "SHT31.h"

#define SHT31_ADDRESS   0x44
SHT31 sht;
String wenduo;
String shiduo;

//****** ↑↑↑温度湿度传感器I2C↑↑↑ ******


//*************
#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceBuffer.h"
#include "AudioOutputBuffer.h"
#include "AudioFileSourceID3.h"

AudioFileSourceSD *source;
AudioGeneratorMP3 *mp3;
AudioOutputI2S *out;
AudioFileSourceBuffer *bufferedSource; // 缓冲包装器
AudioOutputBuffer *bufferedOutput;
AudioFileSourceID3 *id3;



String musicpath = "/music"; //音乐目录设置
String musicname="空";
uint8_t musicyemian = 0;
float currentVolume = 0.2; // 默认音量为 20%
uint16_t mlists= 1;


struct audioMessage{
    uint8_t     cmd;
    const char* txt;
    uint32_t    value;
    uint32_t    ret;
} audioTxMessage, audioRxMessage;

enum : uint8_t { SET_VOLUME, GET_VOLUME, CONNECTTOHOST, CONNECTTOFS };

QueueHandle_t audioSetQueue = NULL;
QueueHandle_t audioGetQueue = NULL;

void CreateQueues(){
    audioSetQueue = xQueueCreate(10, sizeof(struct audioMessage));
    audioGetQueue = xQueueCreate(10, sizeof(struct audioMessage));
}



//*************



SPIClass hspi(HSPI);
SdFat sd;
FsFile folder,fileSY, fileSY2,txtFile,txtFile1,file1,dir,root,file4,file2,fileSY1,fileSY3,fileSY4;


std::vector<std::string>  listmp3(std::string path);  //音乐文件夹列出
std::vector<std::string> mp3s;
std::vector<std::string> dirs;

std::vector<std::string> zitis;
std::vector<std::string> contents;

String path ="/";  // 文件路径
String txtnamea = "";  // txt 文件名
MatchState ms;
uint8_t zijianju = 0; //行间距全局
uint8_t hangjianju = 0; // 字间距全局
uint8_t binzitikuan = 0;  //外部字体宽。
uint8_t binzitigao = 0;  //外部字体高。

uint16_t zijiejisuan = 0;  //一个字多少个字节，
String binzitiname;  // bin外部字体文件名。
bool binziti =0; //是否外部字体全局。0为内部字体，1为外部字体。


//****** ↓↓↓EEPROM地址和定义↓↓↓ ******
#define eeprom_address0 0    //起始地址
struct EEPROMStruct
{
  uint8_t auto_state;        // 自动刷写eeprom状态 0-需要 1-不需要
  char  city[30];            // 城市
  char  weatherKey[24];      // 天气KEY
  boolean binziti;       // 夜间更新 1-更新 0-不更新
  char  binzitiname[100];         // 自定义一句话
  uint8_t zijianju;             //字间距
  uint8_t hangjianju;             //行间距
  uint8_t inAWord_mod;       // 自定义一句话的模式 0-联网获取句子 1-自定义句子 2-天数倒计时 3-B站粉丝
  boolean batDisplayType;    // 电量显示类型 0-电压 1-百分比
  uint8_t runMode;           // 0-模式选择页面 1-天气 2-阅读 3-时钟 4-配网
  char txtNameLastTime[300];  // 上一次打开的txt文件名
  char customBmp[32];        // 要显示的自定义图片
  float outputPower;         // 设置发射功率
  boolean setRotation;       // 屏幕旋转方向0-90度（1） 1-270度（3）
  boolean clock_display_state;     // 时钟模式是否开启日期显示
  boolean clock_calibration_state; // 时钟模式是否开启强制校准
  uint8_t clockJZJG;               // 时钟模式校准间隔 分钟
  boolean albumAuto;               // 相册自动动播放 0-关闭 1-开启
  boolean fastFlip;                // 快速翻页 0-关闭 1-开启
  uint8_t clockQSJG;               // 时钟模式全局刷新间隔
  boolean sdState;                 // 启用SD卡 1-启用 0-未启用
} eepUserSet;

#define eeprom_address1 sizeof(EEPROMStruct)    //起始地址
struct eepClock {
  char year[5];       // 年
  char month[3];      // 月
  char day[3];        // 日
  /*uint8_t hour;       // 时
    uint8_t minute;     // 分
    uint8_t seconds;    // 秒*/
  char week[12];      // 星期几
  char festival[64];  // 节日名
} eepUserClock;

//****** ↑↑↑EEPROM地址和定义↑↑↑ ******


static TaskHandle_t xzhu,xyuedu,xtxtlist,xTaskGive,xshezhi,xmusicplaying,xtianqi,xmusiclist= NULL;   //任务句柄
BaseType_t xResult;




void setup() {
    Serial.begin(115200);
    Serial.println("Hello Arduino!");
    auto_eeprom();
   

   display.initialize();
    //display.Low_Power_Mode();
    display.High_Power_Mode();
   display.display_on(true);
   display.display_Inversion(false);

   u8g2Fonts.begin(display);                 // connect u8g2 procedures to ST73XX
    //u8g2_for_st73xx.setFontDirection(1); 
   display.setRotation(1);
   display.clearDisplay();
   display.display();

  hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
  binzitiname = String(eepUserSet.binzitiname);



  xTaskCreatePinnedToCore(zhu, "zhu", 1024 * 3, NULL, 3, &xzhu,1);  //开机主页显示4个图标
  xTaskCreatePinnedToCore(txtlist, "txtlist", 1024 * 10, NULL, 3, &xtxtlist,1);  //txt 文件列表 选择界面
 xTaskCreatePinnedToCore(yuedutxt, "yuedutxt", 1024 * 20, NULL, 3, &xyuedu,1);  //txt  排版显示阅读程序，
 // xTaskCreatePinnedToCore(shezhii, "shezhii", 1024 * 2, NULL, 2, &xshezhi, 1);  //未完成
  //xTaskCreatePinnedToCore(tianqii, "tianqii", 1024 * 2, NULL, 6, &xtianqi, 1);  //未完成
  xTaskCreatePinnedToCore(musiclist, "musiclist", 1024 * 6, NULL, 6, &xmusiclist, 1);   // mp3文件列表显示选择界面
 
  xTaskCreatePinnedToCore(onebutton, "onebutton", 1024 * 1, NULL, 2, &xTaskGive,1);   //按键获取任务。  
  xTaskNotify(xzhu, 0b00000000, eSetValueWithOverwrite ); //任务通知-传输数据
 // audioSetVolume(15);

  vTaskDelete(NULL); //没我啥事了，自宫ba




}

void loop() {

}
