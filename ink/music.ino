//gongneng =4;
void musiclist(void * pvParams) {



      uint32_t ulNotificationValue;

      uint8_t txtMenuCount = 1;    // TXT阅读菜单界面选框位置  
      String yijicaidan[] = { "返回播放", "播放模式", "后台播放", "退出播放" }; // 一级菜单选项
      String erjicaidan[] = { "返回", "字间间距", "行间间距", "字体选择"};
      String sanjicaidan[] = {"返回","-","+","确定"};    //间距预览
      String sijicaidan[] = {"确定","返回"};   //字体预览      
      uint8_t arraySize = sizeof(yijicaidan) / sizeof(yijicaidan[0]);
      uint8_t erjiarraySize = sizeof(erjicaidan) / sizeof(erjicaidan[0]);
      uint8_t sanjiarraySize = sizeof(sanjicaidan) / sizeof(sanjicaidan[0]);
      uint8_t jilu =1;
      String jilucaidan[] = {"确定","返回"};   //字体预览  
      uint8_t jilucaidanjuli[2] = {48,221};  //  255大小范围，以内，注意。。。。。。。。
      uint16_t zitiu =1;
      uint8_t zitilinesPerPage = 6;  //  这是每页显示多少行，后期需考虑多设备适应
      uint8_t zitipage = 0;
      uint8_t zitistartLine  = 0;
      String a1 = "";
      uint16_t randomIndex;
      uint8_t volumeLevel;
      uint16_t gequliebiaoshuzi;
      
      mp3s =  listmp3(musicpath.c_str());

      while(1){
          xResult = xTaskNotifyWait(0x00, //在运行前这个命令之前，先清除这几位；
                                    0x00, //运行后，重置所有的bits 0x00 or ULONG_MAX or 0xFFFFFFFF
                                    &ulNotificationValue, //重置前的notification value.
                                    portMAX_DELAY ); //一直等待
           if (xResult == pdTRUE) {

              if(ulNotificationValue==0 ){

                              
                        if (xmusicplaying != NULL) {
                          eTaskState eState = eTaskGetState(xmusicplaying);
                            if (eState == eDeleted) {
                              xTaskCreatePinnedToCore(musicplaying, "musicplaying", 1024 * 30, NULL, 6, &xmusicplaying, 1);  //mp3播放循环程序
                                Serial.print("任务被删除，正在创建\n");
                            } else {
                            
                                Serial.print("任务在运行状态\n");

                            }
                        } else {
                            printf("任务不存在或尚未创建\n");
                            xTaskCreatePinnedToCore(musicplaying, "musicplaying", 1024 * 30, NULL, 6, &xmusicplaying, 1);  //mp3播放循环程序
                        }
                      musicyemian = 0;
                     display.clearDisplay();  
                     if (musicname=="空"){
                        Serial.print("音乐进来了");
                      //  audioConnecttoFS("/1.mp3");
                        display_linecenter(50,"无音乐播放",0,380);                      
                      }
                      else{
                            gequxingxi();
                        }

                      display.display();
                  }
              else if(ulNotificationValue==1 ){ //++
 

                    switch (musicyemian) {
                      case 0:     
                          display.clearDisplay();  
                              if(mp3s.empty()){
                                  Serial.print("空"); 

                                }else{


                                          a1 = "";
                                          mlists = random(mp3s.size());
                                          musicname = mp3s[mlists].c_str();
                                          mlists=mlists+1;
                                          gequliebiaoshuzi= mlists;
                                        // Serial.print(musicname);
                                          if(musicpath!="/")
                                          {a1  = musicpath + "/" + musicname;}
                                          else{
                                            a1  = musicname;
                                          }
                                        // 播放下一首歌  
                                          //Serial.print(a1); 
                                         // vTaskDelay(300); 
                                        // audio.connecttoFS(SD,aaaa.c_str());                       
                                          audioConnecttoFS(a1.c_str());                                        

                                          }
                                          gequxingxi();

                       display.display();
                      break;
                      case 1://功能选择页面
                            txtMenuCount++;
                            if (txtMenuCount == (arraySize+1)) txtMenuCount = 1;
                               // display.setPartialWindow(53, 0, 180 + 10, 128);
                                yijicaidanfc(txtMenuCount,yijicaidan,arraySize);
                            //display.display();
                      break;
                      case 2:
                      break;
                      case 3:
                      break;
                      case 4://mp3文件列表页面
                              //gequliebiaoshuzi= mlists;
                              gequliebiaoshuzi++;

                              if(gequliebiaoshuzi==mp3s.size()+1) gequliebiaoshuzi =1;
                               display.clearDisplay();

                                  //wenjianliebiaoui("字体文件列表",zitiu);
                                  wenjianliebiaoui("音乐文件列表",gequliebiaoshuzi,mp3s);
  
                              display.display();
  
                      break;
                      case 5:  
                            currentVolume=currentVolume+0.2;
                            //Serial.print(currentVolume); 
                            if (currentVolume>0.9){
                              currentVolume=0.8;
                            }else{

                                      display.drawRectangle(10, 42, 15, 122, 1);
                                     
                                      volumeLevel = static_cast<uint8_t>(currentVolume * 10); // 将 currentVolume 转换为整数

                                      switch (volumeLevel) {
                                          case 0: // currentVolume == 0
                                              break;
                                          case 2: // currentVolume == 0.2
                                              display.drawFilledRectangle(10, 102, 15, 122, 1);
                                              break;
                                          case 4: // currentVolume == 0.4
                                              display.drawFilledRectangle(10, 82, 15, 122, 1);
                                              break;
                                          case 6: // currentVolume == 0.6
                                              display.drawFilledRectangle(10, 62, 15, 122, 1);
                                              break;
                                          case 8: // currentVolume == 0.8
                                              display.drawFilledRectangle(10, 42, 15, 122, 1);
                                              break;
                                          default:
                                              // 处理其他情况
                                          break;
                                      }
                                       display.display();  


                            audioSetVolume(currentVolume);
                            
                            //Serial.print("当前音量"); 
                            //Serial.print(currentVolume);  
                                    }
                      break;
                    }




              }
              else if(ulNotificationValue==2 ){  //--
 
                    switch (musicyemian) {
                      case 0:     
                        display.clearDisplay();  
                          if(mp3s.empty()){
                              Serial.print("空"); 

                          }else{


                           a1 = "";
                          // 定义歌曲列表和播放顺序
                          mlists--;

                          if (mlists == 0){
                            mlists=mp3s.size();
                          } 
                          gequliebiaoshuzi= mlists;
                     
                                          musicname = mp3s[mlists-1].c_str();
                                        // Serial.print(musicname);
                                          if(musicpath!="/")
                                          {a1  = musicpath + "/" + musicname;}
                                          else{
                                            a1  = musicname;
                                          }
                                        // 播放下一首歌  

                                          //Serial.print(a1); 
                                         // vTaskDelay(300); 
                                        // audio.connecttoFS(SD,aaaa.c_str());                       
                                          audioConnecttoFS(a1.c_str());  
                                                                                

                          }
                                 gequxingxi();
                                 display.display();         
                       
                      break;
                      case 1:
                            txtMenuCount--;
                            if (txtMenuCount == 0) txtMenuCount = arraySize;
                                //Serial.println(txtMenuCount);
                               // display.setPartialWindow(53, 0, 180 + 10, 128);
                         yijicaidanfc(txtMenuCount,yijicaidan,arraySize);
        

                      break;
                      case 2:
                      break;
                      case 3:
                      break;
                      case 4:
                                
                                gequliebiaoshuzi--;

                               if(gequliebiaoshuzi==0) gequliebiaoshuzi =mp3s.size();
                               display.clearDisplay();

                                  //wenjianliebiaoui("字体文件列表",zitiu);
                                  wenjianliebiaoui("音乐文件列表",gequliebiaoshuzi,mp3s);
  
                              display.display();
                      break;
                      case 5:  

                            currentVolume=currentVolume-0.2;
                            if (currentVolume<0){
                              currentVolume=0;
                            }else{

                                      display.fillRect(10, 42, 15, 122, 0);  //背景清白
                                      display.drawRectangle(10, 42, 15, 122, 1);
                                     
                                      volumeLevel = static_cast<uint8_t>(currentVolume * 10); // 将 currentVolume 转换为整数

                                      switch (volumeLevel) {
                                          case 0: // currentVolume == 0
                                              break;
                                          case 2: // currentVolume == 0.2
                                              display.drawFilledRectangle(10, 102, 15, 122, 1);
                                              break;
                                          case 4: // currentVolume == 0.4
                                              display.drawFilledRectangle(10, 82, 15, 122, 1);
                                              break;
                                          case 6: // currentVolume == 0.6
                                              display.drawFilledRectangle(10, 62, 15, 122, 1);
                                              break;
                                          case 8: // currentVolume == 0.8
                                              display.drawFilledRectangle(10, 42, 15, 122, 1);
                                              break;
                                          default:
                                              // 处理其他情况
                                          break;
                                      }
                                       display.display();  
                            audioSetVolume(currentVolume);
                            //Serial.print("当前音量"); 
                            //Serial.print(currentVolume);     
                            }


                      break;
                    }


              }
              else if(ulNotificationValue==3 ){

                      switch (musicyemian) {
                        case 0:  

                        break;
                        case 1:  
                                if (yijicaidan[txtMenuCount - 1] == yijicaidan[0]) {      //
                                        musicyemian = 0;
                                          display.clearDisplay();  
                                          if (musicname=="空"){
                                              Serial.print("音乐进来了");
                                            //  audioConnecttoFS("/1.mp3");
                                              display_linecenter(50,"无音乐播放",0,380);                      
                                            }
                                            else{
                                                 gequxingxi();                                                 
                                                  }
                                              display.display();
                                        } 
                                else if (yijicaidan[txtMenuCount - 1] == yijicaidan[1]) {   //
                                        
                                        }
                                else if (yijicaidan[txtMenuCount - 1] == yijicaidan[2]) {   //后台播放

                                          xTaskNotify(xzhu, 0b00000000, eSetValueWithOverwrite);  //覆盖原有的值 0 就是初始显示
                                          txtMenuCount = 1;
                                          musicyemian = 10;
                                          gongneng = 0; 


                                }
                                else if (yijicaidan[txtMenuCount - 1] == yijicaidan[3]) {   //退出播放
                                          vTaskDelete(xmusicplaying);
                                          pinMode(I2S_POW, OUTPUT);    // 确保引脚模式为输出
                                          digitalWrite(I2S_POW, LOW);  // 将引脚电平设置为低电平
                                          xTaskNotify(xzhu, 0b00000000, eSetValueWithOverwrite);  //覆盖原有的值 0 就是初始显示
                                          txtMenuCount = 1;
                                          musicyemian = 10;
                                          gongneng = 0;                                        
                                }


                        break;
                        case 2:  
                        break;
                        case 3:  
                        break;
                        case 4:  
                                          mlists= gequliebiaoshuzi;
                                          musicname = mp3s[mlists-1].c_str();
                                        // Serial.print(musicname);
                                          if(musicpath!="/")
                                          {a1  = musicpath + "/" + musicname;}
                                          else{
                                            a1  = musicname;
                                          }
                                        // 播放下一首歌  
                                          //Serial.print(a1); 
                                         // vTaskDelay(300); 
                                        // audio.connecttoFS(SD,aaaa.c_str());                       
                                          audioConnecttoFS(a1.c_str());                                  


                        break;
                      case 5:  
                                  musicyemian = 0;
                                      display.clearDisplay();  

                                      if (musicname=="空"){
                                          Serial.print("音乐进来了");
                                        //  audioConnecttoFS("/1.mp3");
                                          display_linecenter(50,"无音乐播放",0,380);                      
                                        }
                                        else{
                                            
                                        gequxingxi();                               
                                }

                                display.display();


                      break;
                      }



              }
              else if(ulNotificationValue==4 ){
 
                    switch (musicyemian) {
                      case 0:     
                             musicyemian = 1;  //切换到子菜单一;

                            yijicaidanfc(txtMenuCount,yijicaidan,arraySize);
                      break;
                      case 1:
                      break;
                      case 2:
      
                      break;

                      case 3:
                
                      break;
                      case 4:
                      break;
                    }



              }
              else if(ulNotificationValue==5 ){  ///调出mp3文件列表


                        switch (musicyemian) {
                        case 0: 
                                    musicyemian = 4;
                                      display.clearDisplay();
                                          gequliebiaoshuzi= mlists;
                                          wenjianliebiaoui("音乐文件列表",gequliebiaoshuzi,mp3s);
                                      display.display();   
                        break;
                        case 1:  
                        break;
                        case 2:  
                        break;
                        case 3:  
                        break;
                        case 4:  
                                  musicyemian = 0;
                                      display.clearDisplay();  

                                      if (musicname=="空"){
                                          Serial.print("音乐进来了");
                                        //  audioConnecttoFS("/1.mp3");
                                          display_linecenter(50,"无音乐播放",0,380);                      
                                        }
                                        else{
                                            
                                              gequxingxi();                                
                                }

                                display.display();


                        break;
                         
                        }
              }
              else if(ulNotificationValue==6 ){   ///调出音量选项


                        switch (musicyemian) {
                        case 0: 
                                    musicyemian = 5;
                                     // display.clearDisplay();
                                      
                                      display.drawRectangle(10, 42, 15, 122, 1);
                                     
                                      volumeLevel = static_cast<uint8_t>(currentVolume * 10); // 将 currentVolume 转换为整数

                                      switch (volumeLevel) {
                                          case 0: // currentVolume == 0
                                              break;
                                          case 2: // currentVolume == 0.2
                                              display.drawFilledRectangle(10, 102, 15, 122, 1);
                                              break;
                                          case 4: // currentVolume == 0.4
                                              display.drawFilledRectangle(10, 82, 15, 122, 1);
                                              break;
                                          case 6: // currentVolume == 0.6
                                              display.drawFilledRectangle(10, 62, 15, 122, 1);
                                              break;
                                          case 8: // currentVolume == 0.8
                                              display.drawFilledRectangle(10, 42, 15, 122, 1);
                                              break;
                                          default:
                                              // 处理其他情况
                                          break;
                                      }
                                       display.display();  

                        break;
                        case 1:  
                        break;
                        case 2:  
                        break;
                        case 3:  
                        break;
                        case 4:  
                        break;
                         
                        }
              }
           }
      }



}



// std::vector<std::string> listDirs(std::string path) {
//   //Serial.println("列出目录下的文件和文件夹");
//   // 列出目录下的文件和文件夹
//       //hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
//       if(!sd.begin(SdSpiConfig(SDCS_PIN, DEDICATED_SPI, 18000000,&hspi)))
//        {
//          Serial.println("error");
//         //return;
//         }

//   //String mulu ="/";
//   std::vector<std::string> entries; 

//   if (!dir.open(path.c_str())) {
//    Serial.println("dir.open failed");
//    return entries; // 返回空数组
//   }

//   dirtubiao=0;
//   dir.rewind();
//   // Open next file in root.
//   // Warning, openNext starts at the current position of dir so a
//   // rewind may be necessary in your application.
//   bool isDirEmpty = true; // 假设目录是空的
//  // 检查目录是否为空
//   if (file4.openNext(&dir, O_RDONLY)) {
//     //Serial.println("不空的");
//     isDirEmpty = false; // 目录不为空
//   }

//   if (isDirEmpty) {
//     // 如果目录是空的，直接返回空数组
//     //Serial.println("检查到空");
//     return entries;
//   }else{

//    dir.rewind();
//  char buf[64];
//   while (file4.openNext(&dir, O_RDONLY)) {
   
//     if (file4.isDir()) {
//         dirtubiao++;
//         file4.getName(buf, sizeof(buf));
//           // Indicate a directory.
//         // Serial.println("/"+string(buf));
//         std::string name(buf);
//    //   if(path == "/"){name = "/" +name;}
//    //   else{name = path+"/"+name;}     
//         entries.push_back(name);
//     }
//     //file.close();
//   }

//   file4.close();
//   dir.close();
//   return entries;
                
//   }

// }


// //字体

std::vector<std::string> listmp3(std::string path) {
  // 列出目录下的文件和文件夹
    // hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
      if(!sd.begin(SdSpiConfig(SDCS_PIN, DEDICATED_SPI, 40000000,&hspi)))
       {
       Serial.println("error");
    // //     //return;
       }

  std::vector<std::string> entries; 
  if (!dir.open(musicpath.c_str())) {
   Serial.println("dir.open failed");
   return entries; // 返回空数组
  }
  
  dir.rewind();//用于将目录中的文件列表指针重置到目录的开始位置。

  bool isDirEmpty = true; // 假设目录是空的
 // 检查目录是否为空
  if (file4.openNext(&dir, O_RDONLY)) {
    //Serial.println("不空的");
    isDirEmpty = false; // 目录不为空
  }

  if (isDirEmpty) {
    // 如果目录是空的，直接返回空数组
    //Serial.println("检查到空");
    return entries;
  }else{

  dir.rewind();//用于将目录中的文件列表指针重置到目录的开始位置。

    char buf[128];
  while (file4.openNext(&dir, O_RDONLY)) {
    

      // 检查文件扩展名是否为.bin
      file4.getName(buf, sizeof(buf));
      if (String(buf).endsWith(".mp3")) {
        // 打印文件名称
       // Serial.print("文件(.txt): ");
       // file.printName(&Serial);
      //  Serial.println();
         std::string name(buf);
        entries.push_back(name);
      }
    file4.close();
  }
  dir.close();

  return entries;


  }

}


void gequxingxi(){
          String a2 = "";
          display_linecenter(50,musicname,0,380); 
          a2 = String(mlists) + "/" + String(mp3s.size());
          display_linecenter(70,a2,0,380); 
}



