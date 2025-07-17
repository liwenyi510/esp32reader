//gongneng = 0
void zhu(void * pvParams) {
  uint32_t ulNotificationValue; //用来存放本任务的4个字节的notification value
  u8g2Fonts.setFont(gancao);
 // display.setTextColor(GxEPD_BLACK); // 设置文本颜色
  uint8_t i = 1;
  uint8_t yemian = 0;
  uint8_t jilu =1;
  String jilucaidan[] = {"确定","取消"};   //字体预览  
  uint8_t jilucaidanjuli[2] = {48,221};  //  255大小范围，以内，注意。。。。。。。。


      TickType_t timeOut = 1000;
      while(1){
          xResult = xTaskNotifyWait(0x00, //在运行前这个命令之前，先清除这几位；
                                    0x00, //运行后，重置所有的bits 0x00 or ULONG_MAX or 0xFFFFFFFF
                                    &ulNotificationValue, //重置前的notification value.
                                    timeOut ); //一直等待
          if (xResult == pdTRUE) {
              if(ulNotificationValue==0){          //任务数据传输0
                    yemian = 0;
                  display.clearDisplay();
                  zhuyetubiao(i);
                  display.display();

                }
              else if(ulNotificationValue==1){     //任务数据传输1
                switch(yemian){
                  case 0:
                          i++;
                          if(i==5)i=1;
                  display.clearDisplay();
                  zhuyetubiao(i);
                  display.display();


                  break;
                  case 1:
                          jilu++;
                          if(jilu==3)jilu=1;

                                                   
                                                     //  display.clearDisplay();
                                                          //外围圆角框
                                                          
                                                      msgbox_yn("是否打开上次阅读文件？",eepUserSet.txtNameLastTime,jilu);

                                                      display.display(); 
                                                  
                                                          
                        
                                                                  
  
                  break;
                }


                }
              else if(ulNotificationValue==2){    //任务数据传输2
                switch(yemian){
                  case 0:
                            i--;
                            if(i==0)i=4;
                          display.clearDisplay();
                          zhuyetubiao(i);
                          display.display();               
                  break;
                  case 1:
                          jilu--;
                          if(jilu==0)jilu=2;

                                                     // display.clearDisplay();
                                                          //外围圆角框
                                          msgbox_yn("是否打开上次阅读文件？",eepUserSet.txtNameLastTime,jilu);

                                                      display.display(); 
                        
                                                        
                                          break;
                                        }


                        }
              else if(ulNotificationValue==3){    //任务数据传输3
                  switch(yemian){
                      case 0:
                                  switch (i){
                              
                                    case 1: //阅读

                                                if(strlen(eepUserSet.txtNameLastTime)> 0){
                                                    yemian = 1;
                                                   
                                                     //display.clearDisplay();
                                                          //外围圆角框
                                                         

                                                         // display.fillRect(60, 35, 236, 88, 0);
                                                         // display.drawRoundRect(60 + 2, 35+1, 232, 86, 5, 1);
                                                          //电压显示
                                              
                                                          //功能项


                                                          //  display_partialLineCenter(23,eepUserSet.txtNameLastTime);  

                                                            //    u8g2Fonts.setCursor(77, 95);
                                                           //     u8g2Fonts.print("是否打开上次阅读文件？");


                                                          //      u8g2Fonts.setCursor(80, 115);
                                                           //     u8g2Fonts.print("确定");
                                                      
                                                        
                                                           //     u8g2Fonts.setCursor(253, 115);
                                                          //      u8g2Fonts.print("取消");  

                                                           //     display.drawRoundRect(80 - 2, 115, u8g2Fonts.getUTF8Width("返回")+5, 18, 5, 1);                                                                                        
                                                          
                                                     // display.display(); 
                        
                                                                                                      
                                                   // txtnamea =String(eepUserSet.txtNameLastTime);
                                                   // gongneng = 2;
                                                   // xTaskNotify(xyuedu, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值



                                                      msgbox_yn("是否打开上次阅读文件？",eepUserSet.txtNameLastTime,jilu);

                                                      display.display(); 






                                                }
                                                else{
                                                  gongneng = 1;
                                                  xTaskNotify(xtxtlist, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值

                                                }

                                                  
                                    break;
                                    case 2://天气
                                                gongneng = 3;
                                                xResult=xTaskNotify(xtianqi, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值
                                                
                                    break;
                                    case 3://音乐
                                                gongneng = 4;
                                                xTaskNotify(xmusiclist, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值

                                    break;
                                    case 4:  //设置
                                               // gongneng = 5;
                                               // fasong.shifou =1;
                                                //xResult=xTaskNotify(xshezhi, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值    
                                                //  TickType_t timeOut = 2000;
                                                //if (xQueueSend(queueSensor, &fasong, timeOut) != pdPASS) {
                                               ///   Serial.println("DHT22: Queue is full.");
                                               // }                               
                                    break;                    
                                    }
                      break;
                      case 1:
                            if(jilucaidan[jilu - 1] == "确定"){
                                            jilu = 1;
                                            txtnamea = String(eepUserSet.txtNameLastTime);
                                            yemian = 0;
                                            gongneng = 2;
                                         
                                            xTaskNotify(xyuedu, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值

                            }
                            else if(jilucaidan[jilu - 1] == "取消"){
                                                  jilu = 1;
                                                  yemian = 0;
                                                  gongneng = 1;
                                                  xTaskNotify(xtxtlist, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值
                            }
                      break;


                  }

                }
              else if(ulNotificationValue==4){    //任务数据传输4

              }
          }
      }
}

        void zhuyetubiao(uint8_t i){

            display.drawBitmap(19,(display.height() - 64) / 2,yuedu, 64, 64, 1);
            display.drawBitmap(19,((display.height() - 64) / 2) +64,yueduzi, 64, 32, 1);
            display.drawBitmap(113,(display.height() - 64) / 2,tianqi, 64, 64, 1);
            display.drawBitmap(113,((display.height() - 64) / 2) +64,tianqizi, 64, 32, 1);
            display.drawBitmap(207,(display.height() - 64) / 2,yinyue, 64, 64, 1);
            display.drawBitmap(207,((display.height() - 64) / 2) +64,yinyuezi, 64, 32, 1);
            display.drawBitmap(301,(display.height() - 64) / 2,shezhi, 64, 64, 1);
            display.drawBitmap(301,((display.height() - 64) / 2 )+64,shezhizi, 64, 32, 1);

            if(i==1){display.drawFilledRectangle(19, 149, 83, 154, ST7305_COLOR_BLACK);}  //两点法，左上角，右下角
            else if(i==2){display.drawFilledRectangle(113, 149, 177, 154, ST7305_COLOR_BLACK);}  
            else if(i==3){display.drawFilledRectangle(207, 149, 271, 154, ST7305_COLOR_BLACK);}  
            else if(i==4){display.drawFilledRectangle(301, 149, 365, 154, ST7305_COLOR_BLACK);}  

            }

    void display_partialLineCenter(uint16_t y, String zf) ////发送局部刷新的显示信息到屏幕,带居中
    {
      /*
        display_partialLine()
        发送局部刷新的显示信息到屏幕,带居中

        line        行数（0-7）
        zf          字符内容
        lineRefresh 整行刷新 1-是 0-仅刷新字符长度的区域
      */
      const char *character = zf.c_str();                            //String转换char
      uint16_t zf_width = u8g2Fonts.getUTF8Width(character);         //获取字符的像素长度
      uint16_t x = (display.width() / 2) - (zf_width / 2);           //计算字符居中的X坐标（屏幕宽度/2-字符宽度/2）

      u8g2Fonts.setCursor(x, y + 13);
      u8g2Fonts.print(character);

    }





