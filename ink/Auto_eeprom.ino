//首次开机自动刷写EEPROM
void auto_eeprom()
{
  //自动计算需要用多少eeprom数量
  EEPROM.begin(sizeof(EEPROMStruct) + sizeof(eepClock));
  //获取eeprom数据
  EEPROM.get(eeprom_address0, eepUserSet);
  EEPROM.get(eeprom_address1, eepUserClock);
  if (eepUserSet.auto_state != 1) eepUserSet.auto_state = 0;
  if (eepUserSet.auto_state == 0)
  {

    eepUserSet.auto_state = 1; // 自动刷写eeprom状态 0-需要 1-不需要
    //保存wifi参数到flash

    // 为将要存储在EEPROM中的内容设置初始值(默认值)
    strcpy(eepUserSet.city, String("成都").c_str());             // 城市
    strcpy(eepUserSet.weatherKey, String("").c_str());          // 天气key
    eepUserSet.binziti = 0;                                 // 夜间更新 1-更新 0-不更新
    strcpy(eepUserSet.binzitiname, String("空").c_str());     // bin字体名称
    eepUserSet.zijianju = 0;                                    //字间距
    eepUserSet.hangjianju = 0;                                  //行间距
    eepUserSet.inAWord_mod = 1;                                 // 自定义一句话的模式 0-联网获取句子 1-自定义句子 2-天数倒计时 3-B站粉丝
    eepUserSet.batDisplayType = 1;                              // 电池显示类型 0-电压 1-百分比
    eepUserSet.runMode = 0;                                     // 0-模式选择页面 1-天气 2-阅读 3-时钟 4-配网
    strcpy(eepUserSet.txtNameLastTime, String("").c_str());     // 上一次打开的txt文件名
    strcpy(eepUserSet.customBmp, String("").c_str());           // 要显示的自定义图片
    eepUserSet.outputPower = 19.0;                              // 设置发射功率
    eepUserSet.setRotation = 1;                                 // 屏幕旋转方向0-90度（1） 1-270度（3）
    eepUserSet.clock_display_state = 1;                         // 时钟模式是否开启日期显示
    eepUserSet.clock_calibration_state = 1;                     // 时钟模式是否开启强制校准
    eepUserSet.clockJZJG = 60;                                  // 时钟模式校准间隔 分钟
    eepUserSet.albumAuto = 0;                                   // 相册自动动播放 0-关闭 1-开启
    eepUserSet.fastFlip = 1;                                    // 快速翻页 0-关闭 1-开启
    eepUserSet.clockQSJG = 19;                                   // 时钟模式全局刷新间隔
    eepUserSet.sdState = 0;                                     // 启用SD卡 1-启用 0-未启用
    strcpy(eepUserClock.year, String("").c_str());      // 年
    strcpy(eepUserClock.month, String("").c_str());     // 月
    strcpy(eepUserClock.day, String("").c_str());       // 日
    strcpy(eepUserClock.week, String("").c_str());      // 星期几
    strcpy(eepUserClock.festival, String("").c_str());  // 节日名
    /*eepUserClock.hour = 90;     // 时
      eepUserClock.minute = 90;   // 分
      eepUserClock.seconds = 90;  // 秒*/

    EEPROM.put(eeprom_address0, eepUserSet);
    EEPROM.put(eeprom_address1, eepUserClock);
    EEPROM.commit(); // 首次保存覆盖掉旧的数值
    Serial.println("EEPROM put");
  }
  if (eepUserSet.auto_state == 1)
  {
    EEPROM.get(eeprom_address0, eepUserSet);
    EEPROM.get(eeprom_address1, eepUserClock);
    Serial.println("EEPROM get");
  }
}

