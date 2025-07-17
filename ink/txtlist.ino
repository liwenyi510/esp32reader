//gongneng = 1;//功能1
void txtlist(void * pvParams) {

std::vector<std::string>  listDirContents(std::string path);  //列出文件夹和txt文件
std::vector<std::string>  listziti(std::string path);  //字体文件列出
std::vector<std::string>  listDirs(std::string path);  //音乐文件夹列出
std::vector<std::string>  listmp3(std::string path);  //音乐文件夹列出
      uint32_t ulNotificationValue;
      uint8_t i =1; 
      uint8_t linesPerPage = 7;  //  这是每页显示多少行，后期需考虑多设备适应，计算guy.height() 除以
      uint8_t page = 0;
      uint8_t startLine  = 0;
      uint8_t jiange = 19; //行间隔。
      uint8_t qi = 48;
      String sizeStr="未知",fstype1="未知";
      uint8_t tubiaogeli =30;  //让位置给图标
      TickType_t timeOut = 1000;
      u8g2Fonts.setFont(gancao);
    
      while(1){
          xResult = xTaskNotifyWait(0x00, //在运行前这个命令之前，先清除这几位；
                                    0x00, //运行后，重置所有的bits 0x00 or ULONG_MAX or 0xFFFFFFFF
                                    &ulNotificationValue, //重置前的notification value.
                                    portMAX_DELAY ); //一直等待
           if (xResult == pdTRUE) {

              if(ulNotificationValue==0 ){

                display.clearDisplay();

                            
                          //u8g2_for_st73xx.setFontMode(1);                 
                          u8g2Fonts.setCursor(2,13);                
                          u8g2Fonts.print(F("仅支持UTF-8编码文件"));

                          contents =  listDirContents(path.c_str());

                          if(path == "/"){
                          //hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
      //if(!sd.begin(SdSpiConfig(SDCS_PIN, DEDICATED_SPI, 18000000,&hspi)))
                                 //   {
                                //      Serial.println("error");
                                     
                                 //   } 
                                    float size1 = sd.card()->sectorCount() * 512e-9;
                                    sizeStr = String(size1, 2); // 保留两位小数
                                    auto fstype = sd.fatType();  
                                    switch(fstype)
                                    {
                                      case FAT_TYPE_EXFAT:
                                        fstype1 = "ExFat";
                                        Serial.println("ExFat");
                                        break;
                                      case FAT_TYPE_FAT32:
                                        fstype1 = "Fat32";
                                        Serial.println("Fat32");
                                        break;
                                      case FAT_TYPE_FAT16:
                                        fstype1 = "Fat16";
                                        Serial.println("Fat16");
                                        break;
                                      case 0:
                                        Serial.println("error");
                                        break;
                                    }
                                    u8g2Fonts.setCursor(2, 28);
                                    u8g2Fonts.print("剩余");
                                    u8g2Fonts.setCursor(35, 28);
                                    u8g2Fonts.print(sizeStr);
                                    u8g2Fonts.setCursor(75, 28);
                                    u8g2Fonts.print("GB");
                          }
                          else {
                            //if(guy.textWidth(path)>guy.width()){
                            //  guy.drawRightString(path,guy.width(),16);
                          // }else{
                            // guy.drawString(path,2,16);
                          // }
                          }
                      display.drawLine(0, 32, 384, 32, ST7305_COLOR_BLACK);   //横线
                        Serial.println(path);
                        if (contents.empty()) {   //空文件夹
                            u8g2Fonts.setCursor(75, 100);
                            u8g2Fonts.print("无文件");
                            Serial.println("无文件");
                        } 
                        //######空文件夹。
                        else {  //非空文件夹。
                               Serial.println("有文件");
                                startLine = page * linesPerPage;// 计算当前页面的起始行.
                                
                                  // 显示当前页面的行
                                  uint8_t shou = contents.size()-startLine;
                                  if(shou > linesPerPage || shou == 0) {
                                    shou = linesPerPage;
                                  }
                                  for (uint8_t p = 0; p < shou; p++) {
                                    u8g2Fonts.setCursor(tubiaogeli+2, p*jiange+qi);
                                    u8g2Fonts.print(contents[startLine+p].c_str());
                                    if(startLine+p>dirtubiao-1){
                                      display.drawBitmap(0, p*jiange+34-6,shu, 32,32, 1);//书的图标
                                    }else{
                                      display.drawBitmap(0, p*jiange+34-6,wenjianjia, 32,32, 1);//文件夹的图标
                                    }
                                    
                                  }
                                   // int16_t ta = u8g2Fonts.getFontAscent(); // positive
                                  //  int16_t td = u8g2Fonts.getFontDescent(); // negative; in mathematicians view
                                  //  int16_t th = ta - td; // text box height
                                  //display.drawRoundRect(tubiaogeli, jiange*(i-1)+35 , u8g2_for_st73xx.getUTF8Width(contents[startLine+i-1].c_str())+4, th+4, 4, 0); 
                                  display.drawRoundRect(tubiaogeli, jiange*(i-1)+35, u8g2Fonts.getUTF8Width(contents[startLine+i-1].c_str())+4, 18,4, 1);//圆角外框

                        }

                    display.display();
                      
              }
              else if(ulNotificationValue==1 ){
                          i++;
                          display.clearDisplay();
                     if (contents.empty()) {   //空文件夹无操作。
                     }
                    else{

                          u8g2Fonts.setCursor(2, 13);
                          u8g2Fonts.print("仅支持UTF-8编码文件");
                          if(path == "/"){
      //hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
      //if(!sd.begin(SdSpiConfig(SDCS_PIN, DEDICATED_SPI, 18000000,&hspi)))
                                   // {
                                   //   Serial.println("error");
                                      //return;
                                   // } 
                                    float size1 = sd.card()->sectorCount() * 512e-9;
                                    sizeStr = String(size1, 2); // 保留两位小数
                                    auto fstype = sd.fatType();  
                                    switch(fstype)
                                    {
                                      case FAT_TYPE_EXFAT:
                                        fstype1 = "ExFat";
                                        Serial.println("ExFat");
                                        break;
                                      case FAT_TYPE_FAT32:
                                        fstype1 = "Fat32";
                                        Serial.println("Fat32");
                                        break;
                                      case FAT_TYPE_FAT16:
                                        fstype1 = "Fat16";
                                        Serial.println("Fat16");
                                        break;
                                      case 0:
                                        Serial.println("error");
                                        break;
                                    }
                                    u8g2Fonts.setCursor(2, 28);
                                    u8g2Fonts.print("剩余");
                                    u8g2Fonts.setCursor(35, 28);
                                    u8g2Fonts.print(sizeStr);
                                    u8g2Fonts.setCursor(75, 28);
                                    u8g2Fonts.print("GB");
                          }
                          else {
                            //if(guy.textWidth(path)>guy.width()){
                            //  guy.drawRightString(path,guy.width(),16);
                          // }else{
                            // guy.drawString(path,2,16);
                          // }
                          }

                           
                            display.drawLine(0, 32, 384, 32, ST7305_COLOR_BLACK);   //横线
                      //##########################横线#################################################

                    if(contents.size()<linesPerPage+1){
                          if(i==contents.size()+1) i =1;
                          for (uint8_t p = 0; p < contents.size(); p++) {
                              u8g2Fonts.setCursor(tubiaogeli+2, p*jiange+qi);
                              u8g2Fonts.print(contents[startLine+p].c_str());
                            if(p>dirtubiao-1){
                              display.drawBitmap(0, p*jiange+34-6,shu, 32,32, 1);//书的图标
                            }else{
                              display.drawBitmap(0, p*jiange+34-6,wenjianjia, 32,32, 1);//文件夹的图标
                            }
                          }
                    }
                    else {
                        if(i==linesPerPage+1){
                          i=1;
                          page=page+1;
                        }
                          startLine = page * linesPerPage; 
                          if(startLine+i>contents.size()){
                            page = 0;
                            i=1;
                          }

                          startLine = page * linesPerPage;// 计算当前页面的起始行
                            // 显示当前页面的行
                            uint8_t shou = contents.size()-startLine;
                            if(shou>linesPerPage) shou =linesPerPage;
                              for (uint8_t p = 0; p < shou; p++) {
                                  u8g2Fonts.setCursor(tubiaogeli+2, p*jiange+qi);
                                  u8g2Fonts.print(contents[startLine+p].c_str());
                                if(startLine+p>dirtubiao-1){
                                  display.drawBitmap(0, p*jiange+34-6,shu, 32,32, 1);//书的图标
                                }else{
                                  display.drawBitmap(0, p*jiange+34-6,wenjianjia, 32,32, 1);//文件夹的图标
                                }
                              }
                        }
                                    //int16_t ta = u8g2Fonts.getFontAscent(); // positive
                                   // int16_t td = u8g2Fonts.getFontDescent(); // negative; in mathematicians view
                                   // int16_t th = ta - td; // text box height                        
                      display.drawRoundRect(tubiaogeli, jiange*(i-1)+34 , u8g2Fonts.getUTF8Width(contents[startLine+i-1].c_str())+4, 18, 4, 1); //圆角外框

                      
                      
                    display.display();
                    }                           




              }
              else if(ulNotificationValue==2 ){

                    uint8_t wei=0;
                    i--;
                       display.clearDisplay();

                    if (contents.empty()) {   //空文件夹
                     }
                    else{
                     
                          u8g2Fonts.setCursor(2, 13);
                          u8g2Fonts.print("仅支持UTF-8编码文件");
                          if(path == "/"){
                                    float size1 = sd.card()->sectorCount() * 512e-9;
                                    sizeStr = String(size1, 2); // 保留两位小数
                                    auto fstype = sd.fatType();  
                                    switch(fstype)
                                    {
                                      case FAT_TYPE_EXFAT:
                                        fstype1 = "ExFat";
                                        Serial.println("ExFat");
                                        break;
                                      case FAT_TYPE_FAT32:
                                        fstype1 = "Fat32";
                                        Serial.println("Fat32");
                                        break;
                                      case FAT_TYPE_FAT16:
                                        fstype1 = "Fat16";
                                        Serial.println("Fat16");
                                        break;
                                      case 0:
                                        Serial.println("error");
                                        break;
                                    }
                                    u8g2Fonts.setCursor(2, 28);
                                    u8g2Fonts.print("剩余");
                                    u8g2Fonts.setCursor(35, 28);
                                    u8g2Fonts.print(sizeStr);
                                    u8g2Fonts.setCursor(75, 28);
                                    u8g2Fonts.print("GB");
                          }
                          else {

                          }
                        
                          display.drawLine(0, 32, 384, 32, ST7305_COLOR_BLACK);   //横线
                      //##########################横线#################################################


                            if(contents.size()<linesPerPage+1){
                              if(i==0) i = contents.size();

                              for (uint8_t p = 0; p < contents.size(); p++) {
                                  u8g2Fonts.setCursor(tubiaogeli+2, p*jiange+qi);
                                  u8g2Fonts.print(contents[startLine+p].c_str());
                                    if(p>dirtubiao-1){
                                      display.drawBitmap(0, p*jiange+34-6,shu, 32,32, 1);//书的图标
                                    }else{
                                      display.drawBitmap(0, p*jiange+34-6,wenjianjia, 32,32, 1);//文件夹的图标
                                    }
                                }

                            }else{
                              if(i==0){
                                if(page==0){
                                      page=(contents.size()-1)/ linesPerPage; //==1
                                      startLine = page * linesPerPage;   //==9
                                      wei = contents.size()-startLine;  //==9
                                      if(wei==0){i=linesPerPage;
                                      }else{i=wei;}  // ==9
                                }else{
                                      page=page-1;
                                      i=linesPerPage;
                                    }
                              }
                            }
                          startLine = page * linesPerPage;// 计算当前页面的起始行.
                          // 显示当前页面的行
                          uint8_t shou = contents.size()-startLine;
                          if(shou > linesPerPage || shou == 0) {
                            shou = linesPerPage;
                          }

                          for (uint8_t p = 0; p < shou; p++) {
                                  u8g2Fonts.setCursor(tubiaogeli+2, p*jiange+qi);
                                  u8g2Fonts.print(contents[startLine+p].c_str());
                              if(startLine+p>dirtubiao-1){
                                      display.drawBitmap(0, p*jiange+34-6,shu, 32,32, 1);//书的图标
                              }else{
                                      display.drawBitmap(0, p*jiange+34-6,wenjianjia, 32,32, 1);//文件夹的图标
                              }
                          }

                                   // int16_t ta = u8g2Fonts.getFontAscent(); // positive
                                   // int16_t td = u8g2Fonts.getFontDescent(); // negative; in mathematicians view
                                   //int16_t th = ta - td; // text box height                        
                      display.drawRoundRect(tubiaogeli, jiange*(i-1)+34 , u8g2Fonts.getUTF8Width(contents[startLine+i-1].c_str())+4, 18, 4, 1); //圆角外框
                         

                      display.display();
                    }

              }
              else if(ulNotificationValue==3 ){
                  if (contents.empty()) {   //空文件夹


                     }
                    else{
                             String aaaa = contents[startLine + i-1].c_str(); ///这里小心，考虑到有其他文件！！！！！！！！！！！！！！！！！
                                   if(aaaa.endsWith(".txt")){
                                        if (path=="/"){
                                            txtnamea = path.c_str()+aaaa;
                                        }else{
                
                                              // 使用 + 操作符来拼接 "/"
                                              String fullPath = path + "/";
                                              txtnamea =fullPath.c_str()+aaaa;
                                            }
                        
                                            strcpy(eepUserSet.txtNameLastTime, String(txtnamea).c_str()); 
                                            EEPROM.put(eeprom_address0, eepUserSet);
                                            EEPROM.commit(); //保存                                              

                                            i=1;  //u 是回到第一个边框选择。
                                            gongneng = 2;
                                            xTaskNotify(xyuedu, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值
                                    }       //这里要注意文件后缀
                                    else {
                                        i=1;
                                        if (path=="/"){  //作用是去掉 //music 变成 /music
                                            path= path + aaaa;
                                            xTaskNotify(xtxtlist, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值
                                        }
                                        else{
                                              path= path + "/"+ aaaa;
                                              xTaskNotify(xtxtlist, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值
                                        }

                                        }                     
                      
                    }  
      


              }
              else if(ulNotificationValue==4 ){
                if(path == "/"){
                    gongneng =0;
                    i=1;
                    path ="/";
                    xTaskNotify(xzhu, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值 0 就是初始显示
                }else{

                    i=1;
                    path =removeTrailingSlash(path);                     
                    xTaskNotify(xtxtlist, 0b00000000, eSetValueWithOverwrite ); //覆盖原有的值 0 就是初始显示
                }

              }
           }
      }


}


//############################################################################################################

// 定义一个函数，用于去掉路径字符串最后一个'/'及其后面的部分
String removeTrailingSlash(String path) {
  // 找到最后一个'/'的位置
  uint8_t lastSlashIndex = path.lastIndexOf('/');
  
  // 如果找到了'/'，并且它不是字符串的第一个字符
  if (lastSlashIndex != -1 && lastSlashIndex != 0) {
    // 去掉最后一个'/'及其后面的部分
    path = path.substring(0, lastSlashIndex);
  }
  else{

    path = "/";
  }
  
  return path;
}






std::vector<std::string> listDirContents(std::string path) {
  //Serial.println("列出目录下的文件和文件夹");
  // 列出目录下的文件和文件夹
      hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
      if(!sd.begin(SdSpiConfig(SDCS_PIN, DEDICATED_SPI, 40000000,&hspi)))
       {
         Serial.println("error");
        //return;
        }

  //String mulu ="/";
  std::vector<std::string> entries; 

  if (!dir.open(path.c_str())) {
   Serial.println("dir.open failed");
   return entries; // 返回空数组
  }

  dirtubiao=0;
  dir.rewind();
  // Open next file in root.
  // Warning, openNext starts at the current position of dir so a
  // rewind may be necessary in your application.
  bool isDirEmpty = true; // 假设目录是空的
 // 检查目录是否为空
  if (file4.openNext(&dir, O_RDONLY)) {
    Serial.println("不空的");
    isDirEmpty = false; // 目录不为空
  }

  if (isDirEmpty) {
    // 如果目录是空的，直接返回空数组
    Serial.println("检查到空");
    return entries;
  }else{

      dir.rewind();
      char buf[128];
      while (file4.openNext(&dir, O_RDONLY)) {
   
        if (file4.isDir()) {
          dirtubiao++;
          file4.getName(buf, sizeof(buf));
          // Indicate a directory.
        // Serial.println("/"+string(buf));
        std::string name(buf);
   //   if(path == "/"){name = "/" +name;}
   //   else{name = path+"/"+name;}     
        entries.push_back(name);
    }
    //file.close();
  }

  file4.close();
 dir.rewind();

  while (file4.openNext(&dir, O_RDONLY)) {
 
      // 检查文件扩展名是否为.txt
      file4.getName(buf, sizeof(buf));
      if (String(buf).endsWith(".txt")) {
        // 打印文件名称
       // Serial.print("文件(.txt): ");
       // file.printName(&Serial);
      //  Serial.println();
         std::string name(buf);
        entries.push_back(name);
      }
    //
  }
  file4.close();
  dir.close();

  return entries;
                

  }

 


}


