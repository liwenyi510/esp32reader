//gongneng = 2;//功能2
void yuedutxt(void * pvParams) {

std::vector<std::string>  listDirContents(std::string path);  //列出文件夹和txt文件
std::vector<std::string>  listziti(std::string path);  //字体文件列出
std::vector<std::string>  listDirs(std::string path);  //音乐文件夹列出
std::vector<std::string>  listmp3(std::string path);  //音乐文件夹列出

      uint32_t ulNotificationValue;
      uint8_t yemian = 0;
      //uint8_t i = 1;
      uint8_t iii=1;
      uint8_t er = 1;
      uint8_t san = 1;
      uint8_t si =1;
      uint16_t beishu = 0;
      uint8_t zhangjiemeiye = 7;
      uint8_t txtMenuCount = 1;    // TXT阅读菜单界面选框位置  
      String yijicaidan[] = { "继续阅读", "选择章节", "阅读调整","重建索引", "退出阅读" }; // 一级菜单选项
      String erjicaidan[] = { "返回", "字间间距", "行间间距", "字体选择"};
      String sanjicaidan[] = {"返回","-","+","确定"};    //间距预览
      String sijicaidan[] = {"返回","确定"};   //字体预览 
      String jilucaidan[] = {"确定","取消"};   //字体预览 
        uint8_t jilu =1;      
      uint8_t arraySize = sizeof(yijicaidan) / sizeof(yijicaidan[0]);
      uint8_t erjiarraySize = sizeof(erjicaidan) / sizeof(erjicaidan[0]);
      uint8_t sanjiarraySize = sizeof(sanjicaidan) / sizeof(sanjicaidan[0]);
      uint8_t sanjicaidanjuli[4] = {58, 103, 173, 203};  //  255大小范围，以内，注意。。。。。。。。
      uint8_t sijicaidanjuli[2] = {58,203};  //  255大小范围，以内，注意。。。。。。。。

      //###############字体选择###################

      uint8_t zitiu =1;

                          //hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
      //if(!sd.begin(SdSpiConfig(SDCS_PIN, DEDICATED_SPI, 18000000,&hspi)))
                    // {
                    //  Serial.println("错误1");
                    //   //return;
                    // } 
      while(1){
          xResult = xTaskNotifyWait(0x00, //在运行前这个命令之前，先清除这几位；
                                    0x00, //运行后，重置所有的bits 0x00 or ULONG_MAX or 0xFFFFFFFF
                                    &ulNotificationValue, //重置前的notification value.
                                    portMAX_DELAY ); //一直等待
           if (xResult == pdTRUE) {

              if(ulNotificationValue==0 ){
                        //hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
      if(!sd.begin(SdSpiConfig(SDCS_PIN, DEDICATED_SPI, 40000000,&hspi)))
                   {
                     Serial.println("错误1");
                      return;
                   } 
                    //utf8编码文件检查。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
                    //#####################utf-8文件检查############
                        if (txtFile.open(txtnamea.c_str(), O_RDONLY)) {
                          bool isUTF81 = isUTF8(txtnamea.c_str(), txtFile);
                          txtFile.close();
                          if (isUTF81) {
                           
                            //Serial.print("是utf8文件");
                            //mulujiancha();
                            // SYjianliandjiancha();
                            
                            zhangjieshengcheng();  //章节目录生成。
                            zjymsc(0);
                            zjymcj();             //当前章节页面信息生成
                            display.clearDisplay();        
                            xiayiye(0);
                            display.display();
                          } else {
                            txtFile.close();
                            //Serial.print("非UTF-8编码文件");
                            
                                display_partialLine(2, "该文件非UTF-8编码文件");
                                display_partialLine(4, "请重新选择文件");
                            
                            vTaskDelay(1200);  // 延迟一点时间，显示编码检查结果。
                            gongneng = 2;
                            xTaskNotify(xtxtlist, 0b00000000, eSetValueWithOverwrite);  //转移到txt文件选择界面。
                          }
                        }

              }
              else if(ulNotificationValue==1 ){
                    String shangmiancaidan[] = { "返回", "上一页", "下一页" };
                    String txtnameSY = txtnamea + ".i";
                    String txt_syys_str = "";  // 索引记录的页数
                    String txt_sydx_str = "";  // 索引记录的文件大小
                    uint16_t dqzj_uint16;
                    uint16_t zzj_uint16; 
                    switch (yemian) {
                      case 0:  //ui阅读下一页   

                          display.clearDisplay();                    
                          xiayiye(1);
                          display.display();
                       
                      break;
                      case 1: // 一级菜单选择

                            txtMenuCount++;
                            if (txtMenuCount == (arraySize+1)) txtMenuCount = 1;

                               // display.setPartialWindow(53, 0, 180 + 10, 128);
                                yijicaidanfc(txtMenuCount,yijicaidan,arraySize);


                      break;
                      case 2: // //章节选择，++
                                        //    每页显示7节
                                        //startLine =
                                        Serial.println(txtnamea);
                                        // Serial.println(txtnameSY.c_str());
                                        if (!fileSY.open(txtnameSY.c_str(), O_RDWR)) {  // 打开索引文件
                                          Serial.println("所选txt章节文件打不开!");
                                        }
                                        // 移动到文件末尾
                                        fileSY.seekEnd();

                                        // 从文件末尾开始偏移，例如偏移-10字节
                                      
                                        fileSY.seekCur(-16);
                                        // Serial.println(fileSY.position());


                                        for (uint8_t i = 0; i < 8; i++)  // 获取索引末16-9位，记录章节总数用
                                        {
                                          char c = fileSY.read();
                                          txt_sydx_str += c;
                                        }
                                        zzj_uint16 = atol(txt_sydx_str.c_str());  // 转换成int格式
                                                                                          // Serial.println(zzj_uint16);

                                        for (uint8_t i = 0; i < 8; i++)  // 获取索引末8-1位，记录上一次打开的章节位置
                                        {
                                          char c = fileSY.read();
                                          txt_syys_str += c;
                                        }
                                        dqzj_uint16 = atol(txt_syys_str.c_str());  // 转换成int格式
                                                                                            //Serial.println(dqzj_uint16);
                                           // Serial.println("当前章节");
                                          //Serial.println(dqzj_uint16);
                                        iii++;
                                        uint8_t zhuyi;
                                        if ( zzj_uint16 - beishu*zhangjiemeiye < zhangjiemeiye) {// 最后一页，当章节数目小于8的时候。

                                          //Serial.println("注意注意");
                                          //Serial.println("ok");
                                          //Serial.println(beishu);
                                          zhuyi = zzj_uint16 - beishu*zhangjiemeiye;

                                          if(iii==zhuyi+4)iii=1;
                                            
                                        }else{
                                          zhuyi = zhangjiemeiye;
                                          if(iii==11)iii=1;
                                            
                                        }

                                          //   Serial.println("倍数");
                                          // Serial.println(yiye8);
                                      //display.setPartialWindow(0, 0, display.width(), display.height());  //局部整行刷新不闪屏  display.setPartialWindow(x, y, w, h);  
                                      
                                        display.clearDisplay();
                                          for (uint8_t tt = 0; tt < zhuyi; tt++) {

                                                  fileSY.seekSet((zhangjiemeiye* beishu+tt)*8);  //目前章节所在位置的第几个8位值。。 5*8 *beishu 
                                                                          
                                                  String gbwz_str = "";

                                                  for (uint8_t ii = 0; ii < 8; ii++) {
                                                    char c = fileSY.read();
                                                    gbwz_str += c;
                                                  }
                                                  uint32_t gbwz_uint32 = atol(gbwz_str.c_str());  //装换成int格式

                                                  txtFile.open(txtnamea.c_str(), O_RDONLY);
                                                  txtFile.seekSet(gbwz_uint32);
                                                  String line1 = txtFile.readStringUntil('\n');

                                                  u8g2Fonts.setCursor(2, tt * 18 + 30);
                                                  u8g2Fonts.print(line1);
                                
                                          }

                                        fileSY.close();  // 关闭文件
                                        txtFile.close();                                          


                                                      u8g2Fonts.setCursor(3, 14);
                                                      u8g2Fonts.print("返回");
                                                      u8g2Fonts.setCursor(80, 14);
                                                      u8g2Fonts.print("下一页");
                                                      u8g2Fonts.setCursor(160, 14);
                                                      u8g2Fonts.print("上一页");     
                                            if(iii>3){

                                                        display.drawRoundRect(0, (iii-5)*18 + 30+2, display.width()-1, 19, 5, 1);
                                            }else{
                                                  if(iii==1){
                                                    
                                                        display.drawRoundRect(1, 0, u8g2Fonts.getUTF8Width("返回")+6, 17, 5, 1);
                                                  }
                                                  else if(iii==2){
                                                       display.drawRoundRect(78, 0, u8g2Fonts.getUTF8Width("下一页")+6, 17, 5, 1);
                                                  }
                                                  else if(iii==3){
                                                       display.drawRoundRect(158, 0, u8g2Fonts.getUTF8Width("上一页")+6, 17, 5, 1);
                                                  }
                                              
                                            }
                                          
                                       display.display();

                                        
                      break;
                      case 3:  // 阅读设置 ++

                          er++;
                          if (er == (erjiarraySize+1)) er = 1;

                              
                            
                               erjicaidanfc(er,erjicaidan,erjiarraySize);



                      break;
                      case 4: //字间距++
                        san++;

                       // display.clearDisplay();
                       // xiayiye(0); 
                        if(san ==(sanjiarraySize+1))san=1;
                             // display.setPartialWindow(50, 100, 180 + 13, 22);
                              display.fillRect(50, 100, 193, 22, 0);             //背景清白
                                                        
                               
                                  //外围圆角框
                                  display.drawRoundRect(53 , 100, 183 , 20, 5, 1);
                                                          u8g2Fonts.setCursor(58, 115);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(103, 115);
                                                          u8g2Fonts.print("-");
                                                          
                                                          u8g2Fonts.setCursor(133, 115);
                                                          u8g2Fonts.print(String(zijianju));

                                                          u8g2Fonts.setCursor(173, 115);
                                                          u8g2Fonts.print("+");  
                                                          u8g2Fonts.setCursor(203, 115);
                                                          u8g2Fonts.print("确定");  

                                  display.drawRoundRect(sanjicaidanjuli[san - 1] - 2, 101, u8g2Fonts.getUTF8Width(sanjicaidan[san - 1].c_str())+5, 18, 5, 1);

                                display.display();

                                                                             
                      break;
                      case 5: // 行间距++
                        san++;
                        //xiayiye(0); 
                        if(san ==(sanjiarraySize+1))san=1;
                              //display.setPartialWindow(50, 100, 180 + 13, 22);
                             // display.clearDisplay();                          
                               display.fillRect(50, 100, 193, 22, 0);             //背景清白
                                  //外围圆角框
                                  display.drawRoundRect(53 , 100, 183 , 20, 5, 1);
                                                          u8g2Fonts.setCursor(58, 115);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(103, 115);
                                                          u8g2Fonts.print("-");
                                                          
                                                          u8g2Fonts.setCursor(133, 115);
                                                          u8g2Fonts.print(String(hangjianju));

                                                          u8g2Fonts.setCursor(173, 115);
                                                          u8g2Fonts.print("+");  
                                                          u8g2Fonts.setCursor(203, 115);
                                                          u8g2Fonts.print("确定");  

                                  display.drawRoundRect(sanjicaidanjuli[san - 1] - 2, 101, u8g2Fonts.getUTF8Width(sanjicaidan[san - 1].c_str())+5, 18, 5, 1);


                                 display.display();                           


                      break;
                      case 6:  //字体选择++

                              zitiu++;
                              if(zitiu==zitis.size()+1) zitiu =1;
                               display.clearDisplay();

                                  //wenjianliebiaoui("字体文件列表",zitiu);
                                  wenjianliebiaoui("字体文件列表",zitiu,zitis);
  
                              display.display();



                      break;
                      case 7:   ///字体预览选择。
                              si++;
                              //xiayiye(0); 
                              if(si ==(3))si=1; 
                              yulanui(si,0);
                              display.display();

                      break;
                      case 8:
                          jilu++;
                          if(jilu==3)jilu=1;   
                                      msgbox_yn("是否启动外部字体？","SD/字体",jilu);
                                      display.display();                    
                      break;   
                    }



              }
              else if(ulNotificationValue==2 ){
                      String shangmiancaidan[] = { "返回", "上一页", "下一页" };
                      String txtnameSY = txtnamea + ".i";
                      String txt_syys_str = "";  // 索引记录的页数
                      String txt_sydx_str = "";  // 索引记录的文件大小
                      uint16_t zzj_uint16;
                      uint16_t dqzj_uint16;
                      uint8_t zhuyi;
                      uint8_t wei=0;  
                      uint8_t shou ;                
                    switch (yemian) {
                      case 0:  //ui阅读上一页   

                     display.clearDisplay();                   
                          shangyiye();
                       
                      break;
                      case 1: // 一级菜单选择--
                            txtMenuCount--;
                            if (txtMenuCount == 0) txtMenuCount = arraySize;
                                //Serial.println(txtMenuCount);
                               // display.setPartialWindow(53, 0, 180 + 10, 128);
                         yijicaidanfc(txtMenuCount,yijicaidan,arraySize);
                      break;
                      case 2:  //章节选择 --
                                        //    每页显示7节
                                        //startLine =
                                        Serial.println(txtnamea);
                                        // Serial.println(txtnameSY.c_str());
                                        if (!fileSY.open(txtnameSY.c_str(), O_RDWR)) {  // 打开索引文件
                                          Serial.println("所选txt章节文件打不开!");
                                        }
                                        // 移动到文件末尾
                                        fileSY.seekEnd();

                                        // 从文件末尾开始偏移，例如偏移-10字节
                                      
                                        fileSY.seekCur(-16);
                                        // Serial.println(fileSY.position());


                                        for (uint8_t i = 0; i < 8; i++)  // 获取索引末16-9位，记录章节总数用
                                        {
                                          char c = fileSY.read();
                                          txt_sydx_str += c;
                                        }
                                        zzj_uint16 = atol(txt_sydx_str.c_str());  // 转换成int格式
                                                                                          // Serial.println(zzj_uint16);

                                        for (uint8_t i = 0; i < 8; i++)  // 获取索引末8-1位，记录上一次打开的章节位置
                                        {
                                          char c = fileSY.read();
                                          txt_syys_str += c;
                                        }
                                        dqzj_uint16 = atol(txt_syys_str.c_str());  // 转换成int格式
                                                                                            //Serial.println(dqzj_uint16);
                                           // Serial.println("当前章节");
                                          //Serial.println(dqzj_uint16);
                                        iii--;
                                        if ( zzj_uint16 - beishu*zhangjiemeiye < zhangjiemeiye) {// 最后一页，当章节数目小于8的时候。

                                          //Serial.println("注意注意");
                                          //Serial.println("ok");
                                          //Serial.println(beishu);
                                          zhuyi = zzj_uint16 - beishu*zhangjiemeiye;

                                          if(iii==0)iii=zhuyi+3;
                                            
                                        }else{
                                          zhuyi = zhangjiemeiye;
                                          if(iii==0)iii=10;
                                            
                                        }

                                          //   Serial.println("倍数");
                                          // Serial.println(yiye8);
                                      //display.setPartialWindow(0, 0, display.width(), display.height());  //局部整行刷新不闪屏  display.setPartialWindow(x, y, w, h);  
                                      display.clearDisplay();
                                          
                                          for (uint8_t tt = 0; tt < zhuyi; tt++) {

                                                  fileSY.seekSet((zhangjiemeiye* beishu+tt)*8);  //目前章节所在位置的第几个8位值。。 5*8 *beishu 
                                                                          
                                                  String gbwz_str = "";

                                                  for (uint8_t ii = 0; ii < 8; ii++) {
                                                    char c = fileSY.read();
                                                    gbwz_str += c;
                                                  }
                                                  uint32_t gbwz_uint32 = atol(gbwz_str.c_str());  //装换成int格式

                                                  txtFile.open(txtnamea.c_str(), O_RDONLY);
                                                  txtFile.seekSet(gbwz_uint32);
                                                  String line1 = txtFile.readStringUntil('\n');

                                                  u8g2Fonts.setCursor(2, tt * 18 + 30);
                                                  u8g2Fonts.print(line1);
                                
                                          }

                                        fileSY.close();  // 关闭文件
                                        txtFile.close();
                                                      u8g2Fonts.setCursor(3, 14);
                                                      u8g2Fonts.print("返回");
                                                      u8g2Fonts.setCursor(80, 14);
                                                      u8g2Fonts.print("下一页");
                                                      u8g2Fonts.setCursor(160, 14);
                                                      u8g2Fonts.print("上一页");     
                                            if(iii>3){

                                                        display.drawRoundRect(0, (iii-5)*18 + 30+2, display.width()-1, 19, 5, 1);
                                            }else{
                                                  if(iii==1){
                                                    
                                                        display.drawRoundRect(1, 0, u8g2Fonts.getUTF8Width("返回")+6, 17, 5, 1);
                                                  }
                                                  else if(iii==2){
                                                       display.drawRoundRect(78, 0, u8g2Fonts.getUTF8Width("下一页")+6, 17, 5, 1);
                                                  }
                                                  else if(iii==3){
                                                       display.drawRoundRect(158, 0, u8g2Fonts.getUTF8Width("上一页")+6, 17, 5, 1);
                                                  }
                                              
                                            }
                                          
                                     display.display();



                      break;
                      case 3:   //阅读设置--
                              er--;
                              if (er == 0) er = erjiarraySize;          
                                //display.setPartialWindow(53, 0, 180 + 10, 128);

                              
                                erjicaidanfc(er,erjicaidan,erjiarraySize);
                                         
                      break;
                      case 4:  //字间距--

                        san--;
                        //xiayiye(0); 
                        if(san == 0)san=sanjiarraySize;

                             // display.setPartialWindow(50, 100, 180 + 13, 22);
                        
                                  //外围圆角框
                            display.fillRect(50, 100, 193, 22, 0);             //背景清白
                                  display.drawRoundRect(53 , 100, 183 , 20, 5, 1);
                                                          u8g2Fonts.setCursor(58, 115);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(103, 115);
                                                          u8g2Fonts.print("-");
                                                          
                                                          u8g2Fonts.setCursor(133, 115);
                                                          u8g2Fonts.print(String(zijianju));

                                                          u8g2Fonts.setCursor(173, 115);
                                                          u8g2Fonts.print("+");  
                                                          u8g2Fonts.setCursor(203, 115);
                                                          u8g2Fonts.print("确定");  

                                  display.drawRoundRect(sanjicaidanjuli[san - 1] - 2, 101, u8g2Fonts.getUTF8Width(sanjicaidan[san - 1].c_str())+5, 18, 5, 1);


                                  display.display();

                      break;
                      case 5: // 行间距--
                        san--;
                        //xiayiye(0); 
                        if(san == 0)san=sanjiarraySize;

                             // display.setPartialWindow(50, 100, 180 + 13, 22);
                            
                                  //外围圆角框
                            display.fillRect(50, 100, 193, 22, 0);             //背景清白
                                  display.drawRoundRect(53 , 100, 183 , 20, 5, 1);
                                                          u8g2Fonts.setCursor(58, 115);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(103, 115);
                                                          u8g2Fonts.print("-");
                                                          
                                                          u8g2Fonts.setCursor(133, 115);
                                                          u8g2Fonts.print(String(hangjianju));

                                                          u8g2Fonts.setCursor(173, 115);
                                                          u8g2Fonts.print("+");  
                                                          u8g2Fonts.setCursor(203, 115);
                                                          u8g2Fonts.print("确定");  

                                  display.drawRoundRect(sanjicaidanjuli[san - 1] - 2, 101, u8g2Fonts.getUTF8Width(sanjicaidan[san - 1].c_str())+5, 18, 5, 1);


                                  display.display();                    
                      break;
                      case 6:  //字体选择--
                               zitiu--;
                              if(zitiu==0) zitiu =zitis.size();
                               display.clearDisplay();
                                 // wenjianliebiaoui("字体文件列表",zitiu);
                                  wenjianliebiaoui("字体文件列表",zitiu,zitis);
                                display.display();
                            
                      break;
                      case 7:  //字体预览--
                            si--;
                            //xiayiye(0); 
                            if(si ==0)si=2;

                                yulanui(si,0);
                                 
                                display.display();
                      break;
                      case 8:
                          jilu--;
                          if(jilu==0)jilu=2;
                                      msgbox_yn("是否启动外部字体？","SD/字体",jilu);
                                      display.display(); 
                      break; 
                    }

              }
              else if(ulNotificationValue==3 ){
                        String zijianjusting = "";
                        String hangjianjusting = "";
                        String aaaa = "";
                        uint32_t dqym;
                        uint16_t dqymbj;  //比较页面大小用。
                        String zjSY = txtnamea + ".ii";
                        // Serial.println(zjSY);
                        String num1,num2,zijianju1,hangjianju1;
                        uint8_t length,zijie;
                    switch (yemian) {      
                      case 0:  //单击中间键  
                      break;
                      case 1: // 一级菜单选择

                                if (yijicaidan[txtMenuCount - 1] == yijicaidan[0]) {      //继续阅读
                                          txtMenuCount = 1;
                                          yemian = 0;
                                         display.clearDisplay();
                                          xiayiye(0);//  0代表页面信息不加1;
                                         display.display();
                                          
                                          //guy.display(READGUY_FAST);  //用快速刷新比较完美
                                        } 
                                else if (yijicaidan[txtMenuCount - 1] == yijicaidan[4]) {   //退出阅读
                                          xTaskNotify(xzhu, 0b00000000, eSetValueWithOverwrite);  //覆盖原有的值 0 就是初始显示
                                          txtMenuCount = 1;
                                          yemian = 0;
                                          gongneng = 0;

                                          //guy.display(READGUY_FAST);   //用快速刷新比较完美
                                        }
                                else if (yijicaidan[txtMenuCount - 1] == yijicaidan[3]) {   //删除章节索引信息

                                          String txtnameSY = txtnamea + ".i";
                                          if (!fileSY.open(txtnameSY.c_str(), O_RDWR)) {
                                            Serial.println("打开文件失败，或文件不存在。");
                                            return;
                                          }
                                          // 检查文件是否成功打开
                                          if (fileSY.isOpen()) {
                                            if (fileSY.remove()) {  // 删除文件
                                              Serial.println("文件删除成功。");
                                            } else {
                                              Serial.println("删除文件失败。");
                                            }
                                          } else {
                                            Serial.println("文件未打开，无法删除。");
                                          }
                                          fileSY.close();  // 关闭文件
                                          yemian = 0;
                                          gongneng = 1;
                                          txtMenuCount = 1;
                                          xTaskNotify(xtxtlist, 0b00000000, eSetValueWithOverwrite);  //覆盖原有的值0
                                        }
                                else if (yijicaidan[txtMenuCount - 1] == yijicaidan[1]) {   //选择章节
                                                                    txtMenuCount = 1;
                                                                    yemian = 2;
                                                                    String txtnameSY = txtnamea + ".i";    //    每页显示8节
                                                                    //startLine =
                                                                    String txt_syys_str = "";  // 索引记录的页数
                                                                    String txt_sydx_str = "";  // 索引记录的文件大小
                                                                    //Serial.println(txtnamea);
                                                                    // Serial.println(txtnameSY.c_str());
                                                                    if (!fileSY.open(txtnameSY.c_str(), O_RDWR)) {  // 打开索引文件
                                                                      Serial.println("所选txt章节文件打不开!");
                                                                    }
                                                                    // 移动到文件末尾
                                                                    fileSY.seekEnd();
                                                                    // 从文件末尾开始偏移，例如偏移-10字节
                                                                
                                                                    fileSY.seekCur(-16);
                                                                    // Serial.println(fileSY.position());

                                                                    for (uint8_t i = 0; i < 8; i++)  // 获取索引末16-9位，记录章节总数用
                                                                    {
                                                                      char c = fileSY.read();
                                                                      txt_sydx_str += c;
                                                                    }
                                                                    uint16_t zzj_uint16 = atol(txt_sydx_str.c_str());  // 转换成int格式
                                                                                                                      // Serial.println(zzj_uint16);

                                                                    for (uint8_t i = 0; i < 8; i++)  // 获取索引末8-1位，记录上一次打开的章节位置
                                                                    {
                                                                      char c = fileSY.read();
                                                                      txt_syys_str += c;
                                                                    }
                                                                    uint16_t dqzj_uint16 = atol(txt_syys_str.c_str());  // 转换成int格式  当前章节
                                                                                                                        //Serial.println(dqzj_uint16);
                                                                    //   Serial.println("当前章节");
                                                                    // Serial.println(dqzj_uint16);

                                                                        uint16_t yiye = dqzj_uint16/zhangjiemeiye; //多少页
                                                                        beishu = yiye;
                                                                      //Serial.println("倍数");
                                                                      //Serial.println(beishu);

                                                                    uint8_t zhuyi = zhangjiemeiye;
                                                                    if ( zzj_uint16 - beishu*zhangjiemeiye < zhangjiemeiye) {  //注意

                                                                      //Serial.println("注意注意");
                                                                      //Serial.println("ok");
                                                                      //Serial.println(beishu);
                                                                      zhuyi = zzj_uint16 - beishu*zhangjiemeiye;
                                                                    }else{
                                                                      zhuyi = zhangjiemeiye;
                                                                    }  
                                                                  display.clearDisplay();
                                                                                                
                                                                      for (uint8_t tt = 0; tt < zhuyi; tt++) {

                                                                              fileSY.seekSet((zhangjiemeiye* beishu+tt)*8);  //目前章节所在位置的第几个8位值。。 5*8 *beishu 
                                                                                                      
                                                                              String gbwz_str = "";

                                                                              for (uint8_t ii = 0; ii < 8; ii++) {
                                                                                char c = fileSY.read();
                                                                                gbwz_str += c;
                                                                              }
                                                                              uint32_t gbwz_uint32 = atol(gbwz_str.c_str());  //装换成int格式

                                                                              txtFile.open(txtnamea.c_str(), O_RDONLY);
                                                                              txtFile.seekSet(gbwz_uint32);
                                                                              String line1 = txtFile.readStringUntil('\n');


                                                                              u8g2Fonts.setCursor(2, tt * 18 + 30);
                                                                              u8g2Fonts.print(line1);

                                                                              //Serial.println(line1);
                                                                      }
                                                                      iii = (dqzj_uint16%zhangjiemeiye)+4;
                                                                  
                                                                      display.drawRoundRect(0, ((dqzj_uint16%zhangjiemeiye)-1)*18 + 30+2, display.width()-1, 18, 5, 1);

                                                                      u8g2Fonts.setCursor(3, 14);
                                                                      u8g2Fonts.print("返回");
                                                                      u8g2Fonts.setCursor(80, 14);
                                                                      u8g2Fonts.print("下一页");
                                                                      u8g2Fonts.setCursor(160, 14);
                                                                      u8g2Fonts.print("上一页");    

                                                                    fileSY.close();  // 关闭文件
                                                                    txtFile.close();


                                                     display.display();


                                                              
                                        }
                                else if (yijicaidan[txtMenuCount - 1] == yijicaidan[2]){    //继续阅读

                                    yemian = 3;  //切换到子菜单二;

                                      erjicaidanfc(er,erjicaidan,erjiarraySize);   //ui.ino


                            }
                                    
                                   
                      break;
                      case 2:  //章节选择

                        if(iii==1){    //返回

                          yemian = 0;

                          display.clearDisplay();                          
                          xiayiye(0);//  0代表页面信息不加1;
                          display.display();
                        
                        }
                        else if(iii==2){    //下一页

                                            String txtnameSY = txtnamea + ".i";    //    每页显示7节
                                  
                                            //startLine =
                                            String txt_syys_str = "";  // 索引记录的页数
                                            String txt_sydx_str = "";  // 索引记录的文件大小

                                            Serial.println(txtnamea);
                                            // Serial.println(txtnameSY.c_str());
                                            if (!fileSY.open(txtnameSY.c_str(), O_RDWR)) {  // 打开索引文件
                                              Serial.println("所选txt章节文件打不开!");
                                            }
                                            // 移动到文件末尾
                                            fileSY.seekEnd();

                                            // 从文件末尾开始偏移，例如偏移-10字节
                                          
                                            fileSY.seekCur(-16);
                                            // Serial.println(fileSY.position());


                                            for (uint8_t i = 0; i < 8; i++)  // 获取索引末16-9位，记录章节总数用
                                            {
                                              char c = fileSY.read();
                                              txt_sydx_str += c;
                                            }
                                            uint16_t zzj_uint16 = atol(txt_sydx_str.c_str());  // 转换成int格式
                                                                                              // Serial.println(zzj_uint16);

                                            for (uint8_t i = 0; i < 8; i++)  // 获取索引末8-1位，记录上一次打开的章节位置
                                            {
                                              char c = fileSY.read();
                                              txt_syys_str += c;
                                            }
                                            uint16_t dqzj_uint16 = atol(txt_syys_str.c_str());  // 转换成int格式
                                                                                                //Serial.println(dqzj_uint16);
                                        
                                            beishu++; //   ##################################################以后判断，大于章节最后，一倍的时候。
                                            if(beishu>zzj_uint16/zhangjiemeiye)  beishu=zzj_uint16/zhangjiemeiye;
                                            uint8_t zhuyi = zhangjiemeiye;
                                            if ( zzj_uint16 - beishu*zhangjiemeiye < zhangjiemeiye) {

                                              Serial.println("注意注意");
                                              Serial.println("ok");
                                              Serial.println(beishu);
                                              zhuyi = zzj_uint16 - beishu*zhangjiemeiye;
                                            }else{
                                              zhuyi = zhangjiemeiye;
                                            }
                                              

                                              Serial.println("ok");
                                              Serial.println(beishu);
                                    display.clearDisplay();
                                              for (uint8_t tt = 0; tt < zhuyi; tt++) {

                                                      fileSY.seekSet((zhangjiemeiye* beishu+tt)*8);  //目前章节所在位置的第几个8位值。。 5*8 *beishu 
                                                                              
                                                      String gbwz_str = "";

                                                      for (uint8_t ii = 0; ii < 8; ii++) {
                                                        char c = fileSY.read();
                                                        gbwz_str += c;
                                                      }
                                                      uint32_t gbwz_uint32 = atol(gbwz_str.c_str());  //装换成int格式

                                                      txtFile.open(txtnamea.c_str(), O_RDONLY);
                                                      txtFile.seekSet(gbwz_uint32);

                                                      //Serial.println(gbwz_uint32);//##############################

                                                      String line1 = txtFile.readStringUntil('\n');
                                                      u8g2Fonts.setCursor(2, tt * 18 + 30);
                                                      u8g2Fonts.print(line1);

                                              }
                                            fileSY.close();  // 关闭文件
                                            txtFile.close();                                              
                                
                                            display.drawRoundRect(78, 0, u8g2Fonts.getUTF8Width("下一页")+6, 17, 5, 1);

                                                          u8g2Fonts.setCursor(3, 14);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(80, 14);
                                                          u8g2Fonts.print("下一页");
                                                          u8g2Fonts.setCursor(160, 14);
                                                          u8g2Fonts.print("上一页");    


                                           display.display();

                                      


                        }
                        else if(iii==3){        //上一页


                                            String txtnameSY = txtnamea + ".i";    //    每页显示8节
                                

                                            //startLine =
                                            String txt_syys_str = "";  // 索引记录的页数
                                            String txt_sydx_str = "";  // 索引记录的文件大小

                                            Serial.println(txtnamea);
                                            // Serial.println(txtnameSY.c_str());
                                            if (!fileSY.open(txtnameSY.c_str(), O_RDWR)) {  // 打开索引文件
                                              Serial.println("所选txt章节文件打不开!");
                                            }
                                            // 移动到文件末尾
                                            fileSY.seekEnd();

                                            // 从文件末尾开始偏移，例如偏移-10字节
                                          
                                            fileSY.seekCur(-16);
                                            // Serial.println(fileSY.position());


                                            for (uint8_t i = 0; i < 8; i++)  // 获取索引末16-9位，记录章节总数用
                                            {
                                              char c = fileSY.read();
                                              txt_sydx_str += c;
                                            }
                                            uint16_t zzj_uint16 = atol(txt_sydx_str.c_str());  // 转换成int格式
                                                                                              // Serial.println(zzj_uint16);

                                            for (uint8_t i = 0; i < 8; i++)  // 获取索引末8-1位，记录上一次打开的章节位置
                                            {
                                              char c = fileSY.read();
                                              txt_syys_str += c;
                                            }
                                            uint16_t dqzj_uint16 = atol(txt_syys_str.c_str());  // 转换成int格式
                                                                                                //Serial.println(dqzj_uint16);
                                                Serial.println("当前章节");
                                              Serial.println(dqzj_uint16);
                                              // uint16_t yiye8 = dqzj_uint16/8;

                                              // Serial.println("倍数");
                                              // Serial.println(yiye8);

                                              beishu--;   //   ##################################################注意判断，倍数等于0的情况。
                                              if(beishu == 65535)beishu =0;
                                              Serial.println(beishu);
                                   display.clearDisplay();
                                          
                                              for (uint8_t tt = 0; tt < zhangjiemeiye; tt++) {

                                                      fileSY.seekSet((zhangjiemeiye* beishu+tt)*8);  //目前章节所在位置的第几个8位值。。 5*8 *beishu 
                                                                              
                                                      String gbwz_str = "";

                                                      for (uint8_t ii = 0; ii < 8; ii++) {
                                                        char c = fileSY.read();
                                                        gbwz_str += c;
                                                      }
                                                      uint32_t gbwz_uint32 = atol(gbwz_str.c_str());  //装换成int格式

                                                      txtFile.open(txtnamea.c_str(), O_RDONLY);
                                                      txtFile.seekSet(gbwz_uint32);

                                                      //Serial.println(gbwz_uint32);//##############################

                                                      String line1 = txtFile.readStringUntil('\n');
                                                      u8g2Fonts.setCursor(2, tt * 18 + 30);
                                                      u8g2Fonts.print(line1);

                                                    
                                              }
                                                      fileSY.close();  // 关闭文件
                                                      txtFile.close();                                        
                                          
                                                    display.drawRoundRect(158, 0, u8g2Fonts.getUTF8Width("上一页")+6, 17, 5, 1);

                                                          u8g2Fonts.setCursor(3, 14);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(80, 14);
                                                          u8g2Fonts.print("下一页");
                                                          u8g2Fonts.setCursor(160, 14);
                                                          u8g2Fonts.print("上一页");  

                                                  
                                            display.display();

                                      







                        }
                        else{          //  确定选择章节
                              yemian = 0;
                              String txtnameSY = txtnamea + ".i";
                              fileSY.open(txtnameSY.c_str(), O_RDWR);

                              fileSY.seekEnd();

                              // 从文件末尾开始偏移，例如偏移-8字节
                              fileSY.seekCur(-8);
                              String gbwz_str = "";
                              for (uint8_t ii = 0; ii < 8; ii++) {
                                char c = fileSY.read();
                                gbwz_str += c;
                              }
                              uint16_t zjjilushu = atol(gbwz_str.c_str());  //章节索引最后，记录的数量。

                              //  Serial.println(beishu);
                              if(beishu == zjjilushu/zhangjiemeiye){
                                          if(iii-4==(zjjilushu%zhangjiemeiye)){//当前的章节。。。。。。。直接返回当前页面
                                              fileSY.close();

                                             display.clearDisplay();

                                              xiayiye(0);//  0代表页面信息不加1;
                                              display.display();
                                        
                                          }
                                      
                                      else{
                                              uint16_t  yswz_uint321 = (beishu)*zhangjiemeiye +iii-4;
                                              String yswz_str = "";

                                              if (yswz_uint321 >= 10000000) yswz_str += String(yswz_uint321);
                                              else if (yswz_uint321 >= 1000000) yswz_str += String("0") + String(yswz_uint321);
                                              else if (yswz_uint321 >= 100000) yswz_str += String("00") + String(yswz_uint321);
                                              else if (yswz_uint321 >= 10000) yswz_str += String("000") + String(yswz_uint321);
                                              else if (yswz_uint321 >= 1000) yswz_str += String("0000") + String(yswz_uint321);
                                              else if (yswz_uint321 >= 100) yswz_str += String("00000") + String(yswz_uint321);
                                              else if (yswz_uint321 >= 10) yswz_str += String("000000") + String(yswz_uint321);
                                              else yswz_str += String("0000000") + String(yswz_uint321);

                                              fileSY.seekEnd();
                                              // 从文件末尾开始偏移，例如偏移-8字节
                                              fileSY.seekCur(-8);
                                              fileSY.print(yswz_str);  //写入数据
                                              fileSY.close();

                                              //这里要开始下一章节的信息计算，。。。。。。。。###########################################################
                                              zjymsc(1);
                                             display.clearDisplay();                             
                                              xiayiye(0);
                                              display.display();
                                                                        
                                      }
                                  }else{

                                              uint16_t  yswz_uint321 = (beishu)*zhangjiemeiye +iii-4;
                                              String yswz_str = "";

                                              if (yswz_uint321 >= 10000000) yswz_str += String(yswz_uint321);
                                              else if (yswz_uint321 >= 1000000) yswz_str += String("0") + String(yswz_uint321);
                                              else if (yswz_uint321 >= 100000) yswz_str += String("00") + String(yswz_uint321);
                                              else if (yswz_uint321 >= 10000) yswz_str += String("000") + String(yswz_uint321);
                                              else if (yswz_uint321 >= 1000) yswz_str += String("0000") + String(yswz_uint321);
                                              else if (yswz_uint321 >= 100) yswz_str += String("00000") + String(yswz_uint321);
                                              else if (yswz_uint321 >= 10) yswz_str += String("000000") + String(yswz_uint321);
                                              else yswz_str += String("0000000") + String(yswz_uint321);

                                              fileSY.seekEnd();
                                              // 从文件末尾开始偏移，例如偏移-8字节
                                              fileSY.seekCur(-8);
                                              fileSY.print(yswz_str);  //写入数据
                                              fileSY.close();

                                              //这里要开始下一章节的信息计算，。。。。。。。。###########################################################
                                              zjymsc(1);
                                              display.clearDisplay();
                                              xiayiye(0); 
                                              display.display();

                                  }

                        }




                      break;  
                      case 3: //阅读设置
                              if(erjicaidan[er - 1] == erjicaidan[0]){  //返回

                                      yemian = 1;  //切换到子菜单一;

                                         // display.setPartialWindow(53, 0, 180 + 10, 128);
                                          
                                      yijicaidanfc(txtMenuCount,yijicaidan,arraySize);
                             


                              }
                              else if(erjicaidan[er - 1] == erjicaidan[1]){   //字间距

                                yemian = 4;  //调节字间距页面。

                                display.clearDisplay();

                               
                                  //外围圆角框
                                 xiayiye(0); 
                                  //display.setPartialWindow(50, 100, 180 + 13, 22);
                                 
                                  display.fillRect(50, 100, 193, 22, 0);             //背景清白
                                  display.drawRoundRect(53 , 100, 183 , 20, 5, 1);
                                                          u8g2Fonts.setCursor(58, 115);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(103, 115);
                                                          u8g2Fonts.print("-");
                                                          
                                                          u8g2Fonts.setCursor(133, 115);
                                                          u8g2Fonts.print(String(zijianju));

                                                          u8g2Fonts.setCursor(173, 115);
                                                          u8g2Fonts.print("+");  
                                                          u8g2Fonts.setCursor(203, 115);
                                                          u8g2Fonts.print("确定");  

                                  display.drawRoundRect(sanjicaidanjuli[san - 1] - 2, 101, u8g2Fonts.getUTF8Width(sanjicaidan[san - 1].c_str())+5, 18, 5, 1);


                                  display.display();

                            
                              }
                              
                              else if(erjicaidan[er - 1] == erjicaidan[2]){    //行间距

                                yemian = 5;  //调节字间距页面。

                                display.clearDisplay();

                               
                                  //外围圆角框
                                 xiayiye(0); 
                                 // display.setPartialWindow(50, 100, 180 + 13, 22);
                                
                                  display.fillRect(50, 100, 193, 22, 0);             //背景清白
                                  display.drawRoundRect(53 , 100, 183 , 20, 5, 1);
                                                          u8g2Fonts.setCursor(58, 115);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(103, 115);
                                                          u8g2Fonts.print("-");
                                                          
                                                          u8g2Fonts.setCursor(133, 115);
                                                          u8g2Fonts.print(String(hangjianju));

                                                          u8g2Fonts.setCursor(173, 115);
                                                          u8g2Fonts.print("+");  
                                                          u8g2Fonts.setCursor(203, 115);
                                                          u8g2Fonts.print("确定");  

                                  display.drawRoundRect(sanjicaidanjuli[san - 1] - 2, 101, u8g2Fonts.getUTF8Width(sanjicaidan[san - 1].c_str())+5, 18, 5, 1);


                               display.display();




                              }
                              else if(erjicaidan[er - 1] == erjicaidan[3]){    //字体选择

                                      yemian = 8; //内部外部字体选择页面
                                      msgbox_yn("是否启动外部字体？","SD/字体",jilu);
                                      display.display(); 

                                      // yemian = 6; //字体选择页面
                                      // zitis =  listziti("/字体");
                                      // display.clearDisplay();
                                      // //wenjianliebiaoui("字体文件列表",zitiu);
                                      // wenjianliebiaoui("字体文件列表",zitiu,zitis);
                                      // display.display();
                              }


                      break;
                      case 4: // 字间距
                              if(sanjicaidan[san - 1] == "返回"){
                                  san = 1;
                                    yemian = 3;  //切换到子菜单二;

                                erjicaidanfc(er,erjicaidan,erjiarraySize);

                              }
                              else if(sanjicaidan[san - 1] == "-"){

                                zijianju--;
                                if(zijianju==255) zijianju = 0;

                                display.clearDisplay();
                                  //外围圆角框
                                  xiayiye(0); 
                                 // display.setPartialWindow(50, 100, 180 + 13, 22);
                                  display.fillRect(50, 100, 193, 22, 0);             //背景清白
                                  display.drawRoundRect(53 , 100, 183 , 20, 5, 1);
                                                          u8g2Fonts.setCursor(58, 115);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(103, 115);
                                                          u8g2Fonts.print("-");
                                                          
                                                          u8g2Fonts.setCursor(133, 115);
                                                          u8g2Fonts.print(String(zijianju));

                                                          u8g2Fonts.setCursor(173, 115);
                                                          u8g2Fonts.print("+");  
                                                          u8g2Fonts.setCursor(203, 115);
                                                          u8g2Fonts.print("确定");  

                                  display.drawRoundRect(sanjicaidanjuli[san - 1] - 2, 101, u8g2Fonts.getUTF8Width(sanjicaidan[san - 1].c_str())+5, 18, 5, 1);

                                 display.display();


                              }
                              else if(sanjicaidan[san - 1] == "+"){
                                  zijianju++;
                                  if(zijianju==10) zijianju = 9;

                                display.clearDisplay();
                                  //外围圆角框
                                 xiayiye(0); 
                                  //display.setPartialWindow(50, 100, 180 + 13, 22);
                                  
                                  display.fillRect(50, 100, 193, 22, 0);             //背景清白
                                  display.drawRoundRect(53 , 100, 183 , 20, 5, 1);

                                                          u8g2Fonts.setCursor(58, 115);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(103, 115);
                                                          u8g2Fonts.print("-");
                                                          
                                                          u8g2Fonts.setCursor(133, 115);
                                                          u8g2Fonts.print(String(zijianju));

                                                          u8g2Fonts.setCursor(173, 115);
                                                          u8g2Fonts.print("+");  
                                                          u8g2Fonts.setCursor(203, 115);
                                                          u8g2Fonts.print("确定");  

                                  display.drawRoundRect(sanjicaidanjuli[san - 1] - 2, 101, u8g2Fonts.getUTF8Width(sanjicaidan[san - 1].c_str())+5, 18, 5, 1);
                                display.display();                        
                              }
                              else if(sanjicaidan[san - 1] == "确定"){
                                    // yemian = 0;
                                    er =1;//初始话阅读设置菜单，
                                    txtMenuCount = 1;//一级菜单回到1
                                    san = 1;
                                      eepUserSet.zijianju = zijianju;
                                     // eepUserSet.hangjianju = hangjianju;
                                      EEPROM.put(eeprom_address0, eepUserSet);
                                      EEPROM.commit(); //保存  
                               
                                        zjymcj(); //章节信息重建，保持原有阅读位置。

                                        yemian = 0;
                                        display.clearDisplay();                                     
                                        xiayiye(0);//  0代表页面信息不加1;
                                        display.display();
                                      

                              }

                      break;
                      case 5:  //行间距
                              if(sanjicaidan[san - 1] == "返回"){
                                  san = 1;
                                    yemian = 3;  //切换到子菜单二;

                                erjicaidanfc(er,erjicaidan,erjiarraySize);



                              }
                              else if(sanjicaidan[san - 1] == "-"){

                                hangjianju--;
                                if(hangjianju==255) hangjianju = 0;

                               display.clearDisplay();
                                  //外围圆角框
                                  xiayiye(0); 
                                  //display.setPartialWindow(50, 100, 180 + 13, 22);
                                 display.fillRect(50, 100, 193, 22, 0);             //背景清白
                                  display.drawRoundRect(53 , 100, 183 , 20, 5, 1);
                                                          u8g2Fonts.setCursor(58, 115);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(103, 115);
                                                          u8g2Fonts.print("-");
                                                          
                                                          u8g2Fonts.setCursor(133, 115);
                                                          u8g2Fonts.print(String(hangjianju));

                                                          u8g2Fonts.setCursor(173, 115);
                                                          u8g2Fonts.print("+");  
                                                          u8g2Fonts.setCursor(203, 115);
                                                          u8g2Fonts.print("确定");  

                                  display.drawRoundRect(sanjicaidanjuli[san - 1] - 2, 101, u8g2Fonts.getUTF8Width(sanjicaidan[san - 1].c_str())+5, 18, 5, 1);
                                  display.display();


                              }
                              else if(sanjicaidan[san - 1] == "+"){
                                  hangjianju++;
                                  if(hangjianju==10) hangjianju = 9;

                                display.clearDisplay();
                                  //外围圆角框
                                 xiayiye(0); 
                                  //display.setPartialWindow(50, 100, 180 + 13, 22);
                                  
                                    display.fillRect(50, 100, 193, 22, 0);             //背景清白
                                  display.drawRoundRect(53 , 100, 183 , 20, 5, 1);

                                                          u8g2Fonts.setCursor(58, 115);
                                                          u8g2Fonts.print("返回");
                                                          u8g2Fonts.setCursor(103, 115);
                                                          u8g2Fonts.print("-");
                                                          
                                                          u8g2Fonts.setCursor(133, 115);
                                                          u8g2Fonts.print(String(hangjianju));

                                                          u8g2Fonts.setCursor(173, 115);
                                                          u8g2Fonts.print("+");  
                                                          u8g2Fonts.setCursor(203, 115);
                                                          u8g2Fonts.print("确定");  

                                  display.drawRoundRect(sanjicaidanjuli[san - 1] - 2, 101, u8g2Fonts.getUTF8Width(sanjicaidan[san - 1].c_str())+5, 18, 5, 1);
                                  display.display();                          
                              }
                              else if(sanjicaidan[san - 1] == "确定"){
                                  // yemian = 0;
                                  er =1;//初始话阅读设置菜单，
                                  txtMenuCount = 1;//一级菜单回到1
                                  san = 1;  //共用，恢复原始值

                                      //eepUserSet.zijianju = zijianju
                                      eepUserSet.hangjianju = hangjianju;
                                      EEPROM.put(eeprom_address0, eepUserSet);
                                      EEPROM.commit(); //保存                                    

                                      zjymcj(); //章节信息重建，保持原有阅读位置。
                                      yemian = 0;
                                        display.clearDisplay();                                    
                                      xiayiye(0);//  0代表页面信息不加1;
                                      display.display();
                                  
                              }
                      break;
                      case 6: //字体预览确定

                              aaaa = zitis[zitiu-1].c_str();
                              length = aaaa.length(); // 获取字符串的长度
                              num1 = aaaa.substring(length - 10, length -8); // 从倒数第10个字符到倒数
                              num2 = aaaa.substring(length - 6, length - 4); // 从倒数第10个字符到倒数
                                  binzitikuan = num1.toInt();
                                  binzitigao = num2.toInt();
                                  //Serial.print("First number: ");
                                  //Serial.println(num1.toInt()); // 将字符串转换为整数输出
                                  //Serial.print("Second number: ");
                                  //Serial.println(num2.toInt()); // 将字符串转换为整数输出
                                  binzitiname = aaaa;
                                  zijie = binzitikuan/8;
                                  if(binzitikuan%8!=0)  zijie=zijie+1;
                                  zijiejisuan = zijie*binzitigao;
                                  binziti =1;

                                  yemian = 7;

                                display.clearDisplay();
                                  //外围圆角框
                                 xiayiye(0); 
                                  //display.setPartialWindow(50, 100, 180 + 13, 22);
                                 
                                  yulanui(si,0);


                                  display.display();


                      break;
                      case 7:  //字体选择
                            if(sijicaidan[si - 1] == "返回"){
                                      si =1;
                                      binziti =0;
                                      yemian = 6; //字体选择页面

                                      zitis =  listziti("/字体");
                                      
                                      display.clearDisplay();
                                         // wenjianliebiaoui("字体文件列表",zitiu);
                                          wenjianliebiaoui("字体文件列表",zitiu,zitis);
                                      display.display();
                              }
                            else if (sijicaidan[si - 1] == "确定"){
                                      txtMenuCount = 1;
                                      si =1;
                                      er =1;
                                      //Serial.println(binzitiname.length());
                                      //Serial.println("字体上面");
                                      //Serial.println(binzitiname);
                                      binziti =1;
                                      strcpy(eepUserSet.binzitiname, String(binzitiname).c_str()); 
                                      eepUserSet.binziti = 1;
                                      EEPROM.put(eeprom_address0, eepUserSet);
                                      EEPROM.commit(); //保存  
                                   
                                        //Serial.println("章节重建上面");

                                        zjymcj(); //章节信息重建，保持原有阅读位置。
                                        yemian = 0;
                                        display.clearDisplay();                                        
                                        xiayiye(0);//  0代表页面信息不加1;
                                        display.display();
                                    }                      
                      break;
                      case 8:  // 内部外部字体选择页面
                            if(jilucaidan[jilu - 1] == "确定"){
                                            jilu = 1;
                                      yemian = 6; //字体选择页面
                                      zitis =  listziti("/字体");
                                      display.clearDisplay();
                                      //wenjianliebiaoui("字体文件列表",zitiu);
                                      wenjianliebiaoui("字体文件列表",zitiu,zitis);
                                      display.display();
                                      

                            }
                            else if(jilucaidan[jilu - 1] == "取消"){
                                                  jilu = 1;
                                                  yemian = 0;
                                                  binziti =0;
                                                  binzitiname = "空";
                                                  eepUserSet.binzitiname, String("空").c_str(); 
                                                  eepUserSet.binziti = 0;
                                                  EEPROM.put(eeprom_address0, eepUserSet);
                                                  EEPROM.commit(); //保存 
                                        zjymcj(); //章节信息重建，保持原有阅读位置。
                                        yemian = 0;
                                        display.clearDisplay();                                        
                                        xiayiye(0);//  0代表页面信息不加1;
                                        display.display();
                                                  
                            }                      
                      break;
                    }
              }
              else if(ulNotificationValue==4 ){
                    switch (yemian) {
                      case 0:  //ui调出菜单。   
                            yemian = 1;  //切换到子菜单一;

                                //display.setPartialWindow(53, 0, 180 + 10, 128);
                           yijicaidanfc(txtMenuCount,yijicaidan,arraySize);
                          
                       
                      break;
                      case 1: // 一级菜单选择
                      break;
                    }



              }
           }
      }      

}


//#################下一页的页面信息##########################################
void xiayiye(bool jiajia) {
  char c;
  uint16_t ch_count = 0;  // 统计中文等 3个字节的字符
  uint16_t decodedValue;
 // uint8_t i = 0;
  uint8_t uniCode = 0;
  uint16_t StringLength = 0;
  uint8_t line = 1;
  String shou_str = "";
  String gbwz_str = "";
  uint8_t jisuangao = 0;  //  计算字体的高
  uint8_t jisuankuan = 0;  //  计算字体的宽

  Serial.println("进入下一页");
  // 初始化自定义SPI通信，传入SCK、MISO、MOSI引脚及CS控制信号
  //CustomSPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS);
      //hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
      //if(!sd.begin(SdSpiConfig(SDCS_PIN, DEDICATED_SPI, 18000000,&hspi)))
 //   Serial.println("error");
 //   return;
 // }


  String txtnameSY = txtnamea + ".i";
  String zjSY1 = txtnamea + ".ii";


    if(jiajia){

              fileSY1.open(zjSY1.c_str(), O_RDWR);
              fileSY1.seekEnd();

              fileSY1.seekCur(-8);
              shou_str = "";
              for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
                char c = fileSY1.read();
                shou_str += c;
              }
              uint16_t dqym = atol(shou_str.c_str());  //当前页面

          //将当前页存入索引文件的末尾七位
            uint16_t yswz_uint321 = dqym + 1;
            String yswz_str = "";
          // Serial.println("加一页写入文件");
            if (yswz_uint321 >= 10000000) yswz_str += String(yswz_uint321);
            else if (yswz_uint321 >= 1000000) yswz_str += String("0") + String(yswz_uint321);
            else if (yswz_uint321 >= 100000) yswz_str += String("00") + String(yswz_uint321);
            else if (yswz_uint321 >= 10000) yswz_str += String("000") + String(yswz_uint321);
            else if (yswz_uint321 >= 1000) yswz_str += String("0000") + String(yswz_uint321);
            else if (yswz_uint321 >= 100) yswz_str += String("00000") + String(yswz_uint321);
            else if (yswz_uint321 >= 10) yswz_str += String("000000") + String(yswz_uint321);
            else yswz_str += String("0000000") + String(yswz_uint321);

            fileSY1.seekEnd();
            // 从文件末尾开始偏移，例如偏移-8字节

            fileSY1.seekCur(-8);      //相对前面位移
            fileSY1.print(yswz_str);  //写入数据
            fileSY1.close();
    }


 
  if (!fileSY3.open(zjSY1.c_str(), O_RDWR)) {

    Serial.println("下一页里打不开");
  }
  fileSY3.seekEnd();

  // 从文件末尾开始偏移，例如偏移-8字节

  fileSY3.seekCur(-24);
    shou_str = "";
  for (uint8_t ii1 = 0; ii1 < 8; ii1++) {
    char c = fileSY3.read();
    shou_str += c;
  }
  uint32_t zuihouyiye = atol(shou_str.c_str());  //章节最后页面，也就是下一章的开始字符。

  shou_str = "";
  for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
    char c = fileSY3.read();
    shou_str += c;
  }
  uint16_t zongys = atol(shou_str.c_str());  //章节 总页面

  shou_str = "";
  for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
    char c = fileSY3.read();
    shou_str += c;
  }
  uint16_t dqym = atol(shou_str.c_str());  //当前页面 小于65535


  fileSY3.seekSet((dqym - 1) * 8);
  shou_str = "";
  for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
    char c = fileSY3.read();
    shou_str += c;
  }
  uint32_t dqymsuoyin = atol(shou_str.c_str());  //当前页面
    fileSY3.close();

   // Serial.println(dqym);
    //Serial.println(zongys);
  if(dqym!=0 && zongys != 0){
      if (dqym == zongys ) {  //如果当前页之后的页面大于总页数，就修改章节信息的最后8位。
    
        fileSY.open(txtnameSY.c_str(), O_RDWR);

        fileSY.seekEnd();

        // 从文件末尾开始偏移，例如偏移-8字节
        fileSY.seekCur(-8);

        for (uint8_t ii = 0; ii < 8; ii++) {
          char c = fileSY.read();
          gbwz_str += c;
        }
        uint16_t zjjilushu = atol(gbwz_str.c_str());  //章节索引最后，记录的数量。

        uint16_t yswz_uint321 = zjjilushu + 1;
        String yswz_str = "";

        if (yswz_uint321 >= 10000000) yswz_str += String(yswz_uint321);
        else if (yswz_uint321 >= 1000000) yswz_str += String("0") + String(yswz_uint321);
        else if (yswz_uint321 >= 100000) yswz_str += String("00") + String(yswz_uint321);
        else if (yswz_uint321 >= 10000) yswz_str += String("000") + String(yswz_uint321);
        else if (yswz_uint321 >= 1000) yswz_str += String("0000") + String(yswz_uint321);
        else if (yswz_uint321 >= 100) yswz_str += String("00000") + String(yswz_uint321);
        else if (yswz_uint321 >= 10) yswz_str += String("000000") + String(yswz_uint321);
        else yswz_str += String("0000000") + String(yswz_uint321);

        fileSY.seekEnd();
        // 从文件末尾开始偏移，例如偏移-8字节
        fileSY.seekCur(-8);
        fileSY.print(yswz_str);  //写入数据
        fileSY.close();
        //这里要开始下一章节的信息计算，。。。。。。。。###########################################################
        zjymsc(1);
        display.clearDisplay();
        xiayiye(0);
      }else{
          //Serial.println("读取当前页面");
          //Serial.println(dqymsuoyin);
          String filePath2 = "/字体/" + binzitiname;
          //Serial.print(filePath2);
          //Serial.println(binzitiname.length());
          if(!txtFile1.open(filePath2.c_str(), O_RDWR)){
            Serial.println("打不开");
          }

              if (!txtFile.open(txtnamea.c_str(), O_RDONLY)) {
                Serial.println("所选txt 文件无法打开!");
              }

              

                  txtFile.seekSet(dqymsuoyin);
                  while (txtFile.available()) {              //txt文件有字节存在的情况。
                    c = txtFile.read();                      // 读取一个字节
                    if (txtFile.position() == zuihouyiye) {  //  每章节最后一页最后一个字符。
                      break;
                    }
                    decodedValue = decodeUTF8(c);
                      if(binziti){  //是否外部字体。
                          jisuangao = binzitigao +hangjianju;
                      
                      }else{
                          jisuangao = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent()+ 2 +hangjianju;
                      }
                      if (c == '\n' && line <= (display.height() / jisuangao)) {  // 检查换行符,  // 要考虑翻页问题，如果刚好翻页遇到换行符，要做处理。
                        line++;                                                              //遇到换行符，就换行增加一行。
                        StringLength = 0;                                                    //清空每行的长度计数                                
                      }

                      if (decodedValue != 0) {
                            if(binziti){  //是否是外部bin字库字体。

                                if(display.width()-StringLength < binzitikuan){
                                      line++;
                                      StringLength = 0;              
                                }
                                if (line > (display.height() / jisuangao)) {   //行间距应用。
                                  break;
                                } else {
                            
                                  uint32_t seekPosition = decodedValue * zijiejisuan;// 计算需要偏移的字节数。
                                  //decodedValue是字的unicode码，zijiejisuan是单个字所占用字节大小。
                                  uint8_t buffer1[zijiejisuan]; // 创建一个缓冲区来存储读取的数据
                                  txtFile1.seekSet(seekPosition);//偏移到字，在字库文件中所在开始位置。
                                  txtFile1.read(buffer1, zijiejisuan); // 读取 zijiejisuan 个字节到 buffer 中
                                  display.drawBitmap(StringLength,jisuangao*(line-1),buffer1,binzitikuan,binzitigao,1);   //显示出来
                                  StringLength = StringLength + binzitikuan + zijianju;  //字间距应用。
                                }

                      }else{
                              
                                const char s[] = "哪";
                                    if(c == '\n'){uniCode = 0 ;}
                                    else {uniCode = u8g2Fonts.getUTF8Width(s)+1; } //计算出字的宽。                    
                                if(display.width()-StringLength < uniCode){
                                      line++;
                                      StringLength = 0;              
                                }
                                if (line > (display.height() / jisuangao)) {   //行间距应用。
                                  break;
                                } else {

                                // u8g2Fonts.setCursor(StringLength, (((u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent())* line )+(hangjianju)* (line-1)));
                                  u8g2Fonts.drawGlyph(StringLength, (((u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent()+2)* line)+(hangjianju)* (line-1)), decodedValue);
                                  //u8g2Fonts.print(decodedValue);
                                  StringLength = StringLength + uniCode + zijianju;  //字间距应用。
                                }
                              }
                }
              }
                txtFile.close();
                txtFile1.close();

      }
  }
}
//END############################################################


//##############################上一页的页面信息################################################
void shangyiye() {

  char c;
  uint16_t ch_count = 0;  // 统计中文等 3个字节的字符
  uint16_t decodedValue;
  uint8_t uniCode = 0;
  uint16_t StringLength = 0;
  uint8_t line = 1;
  String shou_str = "";
  String gbwz_str = "";
  uint32_t  dqymsuoyin;
  uint32_t zuihouyiye;
  uint8_t jisuangao = 0;  //  计算字体的高

  String txtnameSY = txtnamea + ".i";
  String zjSY = txtnamea + ".ii";

    //##################################减一
      fileSY1.open(zjSY.c_str(), O_RDWR);
      fileSY1.seekEnd();

      fileSY1.seekCur(-8);
      shou_str = "";
      for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
        char c = fileSY1.read();
        shou_str += c;
      }
      uint16_t dqym = atol(shou_str.c_str());  //当前页面
      fileSY1.close();

    uint16_t yswz_uint321 = dqym - 1;


    //##############################判断，小于第一页的情况。

        if (yswz_uint321 == 0){
              Serial.println(yswz_uint321);
              //Serial.println("0000000000000000啦");

              fileSY.open(txtnameSY.c_str(), O_RDWR);

              fileSY.seekEnd();

              // 从文件末尾开始偏移，例如偏移-8字节
              fileSY.seekCur(-8);

              for (uint8_t ii = 0; ii < 8; ii++) {
                char c = fileSY.read();
                gbwz_str += c;
              }
              uint16_t zjjilushu = atol(gbwz_str.c_str());  //章节索引最后，记录的数量。

            //  Serial.println(zjjilushu);
              fileSY.close(); 

                    if(zjjilushu == 0){   //  第一章节了、
                      

                        fileSY1.open(zjSY.c_str(), O_RDWR);
                            fileSY1.seekSet(0);//###############################################
                            shou_str = "";
                            for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
                              char c = fileSY1.read();
                              shou_str += c;
                            }
                           dqymsuoyin = atol(shou_str.c_str());  //当前页面

                        //Serial.println(dqymsuoyin);
                        fileSY1.close();
                      

                    }else{
                          uint16_t yswz_uint321 = zjjilushu - 1; // 章节记录数-1；
                          String yswz_str = "";

                          if (yswz_uint321 >= 10000000) yswz_str += String(yswz_uint321);
                          else if (yswz_uint321 >= 1000000) yswz_str += String("0") + String(yswz_uint321);
                          else if (yswz_uint321 >= 100000) yswz_str += String("00") + String(yswz_uint321);
                          else if (yswz_uint321 >= 10000) yswz_str += String("000") + String(yswz_uint321);
                          else if (yswz_uint321 >= 1000) yswz_str += String("0000") + String(yswz_uint321);
                          else if (yswz_uint321 >= 100) yswz_str += String("00000") + String(yswz_uint321);
                          else if (yswz_uint321 >= 10) yswz_str += String("000000") + String(yswz_uint321);
                          else yswz_str += String("0000000") + String(yswz_uint321);

                          fileSY.open(txtnameSY.c_str(), O_RDWR);
                          fileSY.seekEnd();
                          // 从文件末尾开始偏移，例如偏移-8字节
                          fileSY.seekCur(-8);
                          fileSY.print(yswz_str);  //写入数据
                          fileSY.close();

                          //这里要开始上一章节的信息计算，。。。。。。。。###########################################################

                          zjymsc(1);     //################################################################################################有问题，需要处理。

                          display.clearDisplay();
                       


                          fileSY1.open(zjSY.c_str(), O_RDWR);
                          fileSY1.seekEnd();
                          // 从文件末尾开始偏移，例如偏移-16字节
                          fileSY1.seekCur(-16);
                          shou_str = "";
                          for (uint8_t tt = 0; tt < 8; tt++) {
                            char c = fileSY1.read();
                            shou_str += c;
                          }
                          uint16_t zongys = atol(shou_str.c_str());  //章节 总页面

                          yswz_uint321 = zongys - 1; // 章节记录数-1；
                          yswz_str = "";

                          if (yswz_uint321 >= 10000000) yswz_str += String(yswz_uint321);
                          else if (yswz_uint321 >= 1000000) yswz_str += String("0") + String(yswz_uint321);
                          else if (yswz_uint321 >= 100000) yswz_str += String("00") + String(yswz_uint321);
                          else if (yswz_uint321 >= 10000) yswz_str += String("000") + String(yswz_uint321);
                          else if (yswz_uint321 >= 1000) yswz_str += String("0000") + String(yswz_uint321);
                          else if (yswz_uint321 >= 100) yswz_str += String("00000") + String(yswz_uint321);
                          else if (yswz_uint321 >= 10) yswz_str += String("000000") + String(yswz_uint321);
                          else yswz_str += String("0000000") + String(yswz_uint321);

                          fileSY1.seekEnd();
                          // 从文件末尾开始偏移，例如偏移-8字节
                          fileSY1.seekCur(-8);
                          fileSY1.print(yswz_str);  //写入数据
                        
                          fileSY1.seekEnd();
                          // 从文件末尾开始偏移，例如偏移-8字节
                          fileSY1.seekCur(-24);
                          shou_str = "";
                          for (uint8_t ii = 0; ii < 8; ii++) {
                            char c = fileSY1.read();
                            shou_str += c;
                          }
                          zuihouyiye = atol(shou_str.c_str());  //章节最后页面，也就是下一章的开始字符。

                            fileSY1.seekSet((yswz_uint321-1) * 8);//###############################################
                            shou_str = "";
                            for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
                              char c = fileSY1.read();
                              shou_str += c;
                            }
                          dqymsuoyin = atol(shou_str.c_str());  //当前页面

                     // Serial.println("当前页面的索引");
                     // Serial.println(dqymsuoyin);

                      fileSY1.close();
                    }

        }else{

              //将当前页存入索引文件的末尾七位    //       、、、、、、、、、、、、、、、、、、、、
                
                String yswz_str = "";
                //Serial.println("减一页写入文件");
                if (yswz_uint321 >= 10000000) yswz_str += String(yswz_uint321);
                else if (yswz_uint321 >= 1000000) yswz_str += String("0") + String(yswz_uint321);
                else if (yswz_uint321 >= 100000) yswz_str += String("00") + String(yswz_uint321);
                else if (yswz_uint321 >= 10000) yswz_str += String("000") + String(yswz_uint321);
                else if (yswz_uint321 >= 1000) yswz_str += String("0000") + String(yswz_uint321);
                else if (yswz_uint321 >= 100) yswz_str += String("00000") + String(yswz_uint321);
                else if (yswz_uint321 >= 10) yswz_str += String("000000") + String(yswz_uint321);
                else yswz_str += String("0000000") + String(yswz_uint321);

                fileSY1.open(zjSY.c_str(), O_RDWR);
                fileSY1.seekEnd();
                // 从文件末尾开始偏移，例如偏移-8字节

                fileSY1.seekCur(-8);      //相对前面位移
                fileSY1.print(yswz_str);  //写入数据
               // fileSY1.close();

                        //  fileSY1.open(zjSY.c_str(), O_RDWR);
                          fileSY1.seekEnd();

                          // 从文件末尾开始偏移，例如偏移-24字节

                          fileSY1.seekCur(-24);
                            shou_str = "";
                          for (uint8_t ii = 0; ii < 8; ii++) {
                            char c = fileSY1.read();
                            shou_str += c;
                          }
                           zuihouyiye = atol(shou_str.c_str());  //章节最后页面，也就是下一章的开始字符。

                          shou_str = "";
                          for (uint8_t tt = 0; tt < 8; tt++) {
                            char c = fileSY1.read();
                            shou_str += c;
                          }
                          uint16_t zongys = atol(shou_str.c_str());  //章节 总页面

                          shou_str = "";
                          for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
                            char c = fileSY1.read();
                            shou_str += c;
                          }
                          dqym = atol(shou_str.c_str());  //当前页面


                          fileSY1.seekSet((dqym - 1) * 8);
                          shou_str = "";
                          for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
                            char c = fileSY1.read();
                            shou_str += c;
                          }
                          dqymsuoyin = atol(shou_str.c_str());  //当前页面

                          fileSY1.close();
          
        }


 

  if (!txtFile.open(txtnamea.c_str(), O_RDONLY)) {
    Serial.println("所选txt 文件!");
  }


  String filePath2 = "/字体/" + binzitiname;
  //Serial.print(filePath2);
  //Serial.println(binzitiname.length());

  if(!txtFile1.open(filePath2.c_str(), O_RDWR)){
    Serial.println("打不开");
  }


  
      txtFile.seekSet(dqymsuoyin);
      while (txtFile.available()) {              //txt文件有字节存在的情况。
        c = txtFile.read();                      // 读取一个字节
        if (txtFile.position() == zuihouyiye) {  //  每章节最后一页最后一个字符。
          break;
        }

        decodedValue = decodeUTF8(c);


    if(binziti){  //是否外部字体。
        jisuangao = binzitigao +hangjianju;
    
    }else{
        jisuangao = u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent()+ 2 +hangjianju;
    }
    if (c == '\n' && line <= (display.height() / jisuangao)) {  // 检查换行符,  // 要考虑翻页问题，如果刚好翻页遇到换行符，要做处理。
      line++;                                                              //遇到换行符，就换行增加一行。
      StringLength = 0;                                                    //清空每行的长度计数
    }

    if (decodedValue != 0) {
          if(binziti){  //是否外部字体。
                    if(display.width()-StringLength < binzitikuan){
                          line++;
                          StringLength = 0;              
                    }
                    if (line > (display.height() / jisuangao)) {   //行间距应用。
                      break;
                    } else {
                      //String filePath2 = "/字体/" + binzitiname;
                      //txtFile1.open(filePath2.c_str(), O_RDWR); 
                      uint32_t seekPosition = decodedValue * zijiejisuan;// 计算需要移动的字节数
                      uint8_t buffer1[zijiejisuan]; // 创建一个缓冲区来存储读取的数据
                      txtFile1.seekSet(seekPosition);
                      txtFile1.read(buffer1, zijiejisuan); // 读取 zijiejisuan 个字节到 buffer 中
                      display.drawBitmap(StringLength,jisuangao*(line-1),buffer1,binzitikuan,binzitigao,1);
                      //txtFile1.close();
                      StringLength = StringLength + binzitikuan + zijianju;  //字间距应用。
                      //memset(buffer1, 0, zijiejisuan);
                    }

          }else{
                    const char s[] = "哪";
                        if(c == '\n'){uniCode = 0 ;}
                        else {uniCode = u8g2Fonts.getUTF8Width(s)+1; } //计算出字的宽。 
                    if(display.width()-StringLength < uniCode){
                          line++;
                          StringLength = 0;              
                    }
                    if (line > (display.height() / jisuangao)) {   //行间距应用。
                      break;
                    } else {

                     // u8g2Fonts.setCursor(StringLength, (((u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent())* line )+(hangjianju)* (line-1)));
                      u8g2Fonts.drawGlyph(StringLength, (((u8g2Fonts.getFontAscent() - u8g2Fonts.getFontDescent()+2)* line)+(hangjianju)* (line-1)), decodedValue);
                      //u8g2Fonts.print(decodedValue);
                      StringLength = StringLength + uniCode + zijianju;  //字间距应用。
                    }
          }
      
    }
  }
        txtFile1.close();
        txtFile.close();
   display.display();

}
//END############################################################



//############################章节生成#############################################
void zhangjieshengcheng() {
  // 初始化自定义SPI通信，传入SCK、MISO、MOSI引脚及CS控制信号
  //CustomSPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS);
  int v=0;
  uint16_t yswz_count = 0;    // 待写入文件统计
  uint16_t zzjys_count1 = 0;  // 总章节数
                              // uint32_t yswz_count1 = 0;
  String yswz_str = "";       // 待写入的文件

      //hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
     // if(!sd.begin(SdSpiConfig(SDCS_PIN, DEDICATED_SPI, 18000000,&hspi)))
   // Serial.println("error");
    //return;
 // }
  Serial.println("进入章节页面生成");
  // 编译正则表达式
  //const char *pattern = "[第卷][0123456789一二三四五六七八九十零〇百千两]*[章回部节集卷].*";

  String txtnameSY = txtnamea + ".i";
  Serial.println(txtnamea);

  if (!fileSY2.open(txtnameSY.c_str(), O_RDONLY)) {

    fileSY2.close();

    Serial.println("没有章节信息文件，开始生成");

        uint16_t w = 230;
        uint16_t h = 90;
      
        uint16_t start_x = (display.width() - w) / 2;
        uint16_t start_y = (display.height() - h) / 2;
      
       

        display.fillRect(start_x, 16, w, h, 0);             //背景清白
    display.drawRoundRect(start_x, 16, w, h, 5, 1);     // 230*90窗口背景，圆角5

    display_partialLine(2, "开始创建章节目录索引");
    display_partialLine(4, "请稍等...");

    // 打开TXT文件
    if (!txtFile.open(txtnamea.c_str(), O_RDONLY)) {
      Serial.println("所选txt 文件!,打开错误");
    }

    uint32_t txtTotalSize = txtFile.size();  //记录该TXT文件的大小，插入到索引的倒数14-8位

    while (txtFile.available()) {
      //vTaskDelay(500);

      uint32_t yswz_uint32 = txtFile.position();  //当前章节第一个字符的位置
      String line = txtFile.readStringUntil('\n');
      if (line.length() > 10 && line.length() < 80) {  // 字符数量大于10且小于100的处理逻辑

        char buf[line.length() + 1];
        line.toCharArray(buf, sizeof(buf));
        ms.Target(buf);

        if (ms.Match("第[0123456789一二三四五六七八九十零〇百千两]+章") | ms.Match("第[0123456789一二三四五六七八九十零〇百千两]+卷")) {


          // Serial.println(yswz_uint32);
          // Serial.println(line);

          //页数位置编码处理
          if (yswz_uint32 >= 10000000) yswz_str += String(yswz_uint32);
          else if (yswz_uint32 >= 1000000) yswz_str += "0" + String(yswz_uint32);
          else if (yswz_uint32 >= 100000) yswz_str += "00" + String(yswz_uint32);
          else if (yswz_uint32 >= 10000) yswz_str += "000" + String(yswz_uint32);
          else if (yswz_uint32 >= 1000) yswz_str += "0000" + String(yswz_uint32);
          else if (yswz_uint32 >= 100) yswz_str += "00000" + String(yswz_uint32);
          else if (yswz_uint32 >= 10) yswz_str += "000000" + String(yswz_uint32);
          else yswz_str += "0000000" + String(yswz_uint32);
          yswz_count++;
          zzjys_count1++;
          if (yswz_count == 10)  //每10章控制屏幕显示一下当前进度
          {

            float value = (float(yswz_uint32) / float(txtTotalSize)) * 100;
            if (v != int(value)) {
              v = int(value);
              Serial.print("处理进度：");
              String progressStr = "处理进度: ";
              progressStr += value;
              progressStr += "%";
       
              display_partialLine(2, txtnamea);
              
              display_partialLine(3, progressStr);
              display_partialLine(4, "请勿进行按键操作...");

              Serial.print(v);
              Serial.println(" %");
          


           
              if (!fileSY.open(txtnameSY.c_str(), O_RDWR | O_CREAT)) {
                Serial.println("所选索引!,打开错误");
              }
              fileSY.seekEnd();
              fileSY.print(yswz_str);  //将待写入的缓存 写入索引文件中
              fileSY.close();

              yswz_str = "";   // 待写入文件清空
              //yswz_count = 0;  // 待写入计数清空

              //Serial.println("写入索引文件");
            }
            yswz_count = 0;  // 待写入计数清空
          }
        }
      }
    }


    //剩余的章节写入索引文件，并在末尾加章节总数量14-8 页数记录位7-1
    uint16_t size_uint32 = zzjys_count1;  //获取当前章节数量文件的大小
    String size_str = "";
    //TXT文件大小编码处理
    if (size_uint32 >= 10000000) size_str += String(size_uint32);
    else if (size_uint32 >= 1000000) size_str += String("0") + String(size_uint32);
    else if (size_uint32 >= 100000) size_str += String("00") + String(size_uint32);
    else if (size_uint32 >= 10000) size_str += String("000") + String(size_uint32);
    else if (size_uint32 >= 1000) size_str += String("0000") + String(size_uint32);
    else if (size_uint32 >= 100) size_str += String("00000") + String(size_uint32);
    else if (size_uint32 >= 10) size_str += String("000000") + String(size_uint32);
    else size_str += String("0000000") + String(size_uint32);

    if (yswz_count != 0)  //还有剩余页数就在末尾加入 剩余的页数+总章节数+当前位置位（初始0）
    {
      fileSY.open(txtnameSY.c_str(), O_CREAT | O_APPEND | O_WRITE);
      fileSY.seekEnd();
      fileSY.print(yswz_str + size_str + "00000000");
      fileSY.close();
    } else  //没有剩余页数了就在末尾加入文件大小位+当前位置位
    {
              if (!fileSY.open(txtnameSY.c_str(), O_CREAT | O_APPEND | O_WRITE)) {
                Serial.println("所选追加!,打开错误");
              }
      
      fileSY.seekEnd();
      fileSY.print(size_str + "00000000");
      fileSY.close();
    }

    yswz_str = "";     // 待写入文件清空
    yswz_count = 0;    // 待写入计数清空
    zzjys_count1 = 0;  // 待写入计数清空

    txtFile.close();
    display_partialLine(5, "章节目录创建完成");
  } 
  else{
    fileSY2.close();
  }
  
   // Serial.println("有目录索引文件");
}
//END############################################################


//####################章节里面的页面生成########################################
void zjymsc(bool chongxin) {  //重新建立章节信息。
  char c;
  uint16_t ch_count = 0;  // 统计中文等 3个字节的字符
  uint16_t decodedValue;
  uint8_t uniCode = 0;
  uint16_t StringLength = 0;
  uint16_t yanzheng = 0;
  uint8_t line = 1;
  uint8_t jisuangao = 0;  //  计算字体的高
  uint8_t jisuankuan = 0;  //  计算字体的宽
  String yswz_str;
  //###################################################
  // 初始化自定义SPI通信，传入SCK、MISO、MOSI引脚及CS控制信号
  //CustomSPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS);
      //hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
     // if(!sd.begin(SdSpiConfig(SDCS_PIN, DEDICATED_SPI, 18000000,&hspi)))
 //   Serial.println("error");
 //   return;
 // }
  Serial.println("进入zjymsc");

  //########################################################
  String txtnameSY = txtnamea + ".i";

  String zjSY3 = txtnamea + ".ii";

  //###################################删除章节信息索引###################
  if (chongxin) {
    if (!fileSY1.open(zjSY3.c_str(), O_RDWR)) {
      Serial.println("打开文件失败，或文件不存在。");
      return;
    }
    
    // 检查文件是否成功打开
    if (fileSY1.isOpen()) {
      if (fileSY1.remove()) {  // 删除文件
        Serial.println("文件删除成功。");
      } else {
        Serial.println("删除文件失败。");
      }
    } else {
      Serial.println("文件未打开，无法删除。");
    }
    fileSY1.close();  // 关闭文件
  }

    vTaskDelay(10); 

  if (!fileSY1.open(zjSY3.c_str(), O_RDWR)) {
    fileSY1.close();
    Serial.println("每章节信息生成中");
   // display_partialLine(4, "章节信息生成中...");

    String gbwz_str = "";
    String shou_str ="";
    String wei_str ="";

    //Serial.println("进入目录章节生成环节");
    //##################读取出当前的所在章节的位置
    if (!fileSY.open(txtnameSY.c_str(), O_RDWR | O_CREAT)){
        Serial.println("文件未打开，666。");
    }
    
    fileSY.seekEnd();

    // 从文件末尾开始偏移，例如偏移-8字节
    int offset = -8;
    fileSY.seekCur(offset);

    for (uint8_t ii = 0; ii < 8; ii++) {
      char c = fileSY.read();
      gbwz_str += c;
    }
    uint16_t gbwz_uint32 = atol(gbwz_str.c_str());  //装换成int格式
    fileSY.seekSet(gbwz_uint32 * 8);  //  章节信息的起点。
    for (uint8_t ii = 0; ii < 8; ii++) {
      char c = fileSY.read();
      shou_str += c;
    }
    uint32_t shou_uint16 = atol(shou_str.c_str());  //章节起点坐标

    for (uint8_t ii = 0; ii < 8; ii++) {
      char c = fileSY.read();
      wei_str += c;
    }
    uint32_t wei_uint16 = atol(wei_str.c_str());  //下一章节起点坐标
    fileSY.close();

    if (!txtFile.open(txtnamea.c_str(), O_RDONLY)) {
      Serial.println("所选txt 文件!,打开错误");
    }
    txtFile.seekSet(shou_uint16);  //打开txt文本，该章节所在的开始字节位置。
  
    yswz_str += shou_str;

    const char s[] = "哪";
    uniCode = u8g2Fonts.getUTF8Width(s)+1; //计算出字的宽。

    String filePath2 = "/字体/" + binzitiname;
    Serial.print(filePath2);
    if (!txtFile1.open(filePath2.c_str(), O_RDONLY)){
          Serial.println("字体文件不存在");
          binzitiname="空";
          binziti =0; 
          strcpy(eepUserSet.binzitiname, String("空").c_str()); 
          eepUserSet.binziti = 0;
          EEPROM.put(eeprom_address0, eepUserSet);
          EEPROM.commit(); //保存                                                                                       
    }
    
    else{
          Serial.println("字体文件存在");
          binziti=1; //启动外部字体。
          //binzitiname = binzitiname.c_str();
          uint8_t length = binzitiname.length(); // 获取字符串的长度
          String num1 = binzitiname.substring(length - 10, length - -8); // 从倒数第10个字符到倒数
          String num2 = binzitiname.substring(length - 6, length - 4); // 从倒数第10个字符到倒数
              binzitikuan = num1.toInt();
              binzitigao = num2.toInt();
              uint8_t zijie = binzitikuan/8;
              if(binzitikuan%8!=0)  zijie=zijie+1;
              zijiejisuan = zijie*binzitigao;
              //Serial.print("First number: ");
              //Serial.println(num1.toInt()); // 将字符串转换为整数输出
              //Serial.print("Second number: ");
            // Serial.println(num2.toInt()); // 将字符串转换为整数输出
    }
    txtFile1.close();



    if(binziti){  //是否外部字体。
        jisuangao = binzitigao +hangjianju;
        jisuankuan = binzitikuan + zijianju;
    
    }else{
        int16_t ta = u8g2Fonts.getFontAscent(); // positive
        int16_t td = u8g2Fonts.getFontDescent(); // negative; in mathematicians view
        int16_t th = ta - td; // text box height
        jisuangao = th+2+hangjianju;
        jisuankuan = uniCode + zijianju;
    }


    while (txtFile.position() < wei_uint16) {  //字节所在位置，要小于 下一章节的位置。
   
      c = txtFile.read();  // 读取一个字节
      if (c == '\n' && line <= (display.height() / jisuangao)) {  // 检查换行符,  // 要考虑翻页问题，如果刚好翻页遇到换行符，要做处理。
        line++;                                                              //遇到换行符，就换行增加一行。
        StringLength = 0;   //清空每行的长度计数  


                          if (line > (display.height() / jisuangao)) {   //行间距应用。
                                  //翻页的时候，有一个文字会掉，用下面代码，可以解决。
                                  if (decodedValue > 0 && decodedValue < 128) {  // 对于单字节UTF-8，解码值应该小于128
                                    ch_count = 1;
                                  } else if (decodedValue <= 2047) {
                                    ch_count = 2;
                                  } else if (decodedValue <= 65535) {
                                    ch_count = 3;
                                  }  // 如果需要处理四字节，可以添加额外的条件
                                        uint32_t yswz_uint32;

                                        yswz_uint32 = txtFile.position();
                                  
                                  //页数位置编码处理
                                  if (yswz_uint32 >= 10000000) yswz_str += String(yswz_uint32);
                                  else if (yswz_uint32 >= 1000000) yswz_str += "0" + String(yswz_uint32);
                                  else if (yswz_uint32 >= 100000) yswz_str += "00" + String(yswz_uint32);
                                  else if (yswz_uint32 >= 10000) yswz_str += "000" + String(yswz_uint32);
                                  else if (yswz_uint32 >= 1000) yswz_str += "0000" + String(yswz_uint32);
                                  else if (yswz_uint32 >= 100) yswz_str += "00000" + String(yswz_uint32);
                                  else if (yswz_uint32 >= 10) yswz_str += "000000" + String(yswz_uint32);
                                  else yswz_str += "0000000" + String(yswz_uint32);
                                  // yswz_count++;
                                  line = 1;
                                  StringLength = 0;
                                  // yswz_count = 0;  // 待写入计数清空
                          }                                   
      }


      decodedValue = decodeUTF8(c);
      if (decodedValue != 0 ) {
            if(binziti){  //是否外部字体。
                    if(display.width()-StringLength < binzitikuan){
                                line++;
                                StringLength = 0;              
                          }
                          if (line > (display.height() / jisuangao)) {   //行间距应用。
                                  //翻页的时候，有一个文字会掉，用下面代码，可以解决。
                                  if (decodedValue > 0 && decodedValue < 128) {  // 对于单字节UTF-8，解码值应该小于128
                                    ch_count = 1;
                                  } else if (decodedValue <= 2047) {
                                    ch_count = 2;
                                  } else if (decodedValue <= 65535) {
                                    ch_count = 3;
                                  }  // 如果需要处理四字节，可以添加额外的条件
                                        uint32_t yswz_uint32;

                                        yswz_uint32 = txtFile.position() - ch_count;
                                  
                                  //页数位置编码处理
                                  if (yswz_uint32 >= 10000000) yswz_str += String(yswz_uint32);
                                  else if (yswz_uint32 >= 1000000) yswz_str += "0" + String(yswz_uint32);
                                  else if (yswz_uint32 >= 100000) yswz_str += "00" + String(yswz_uint32);
                                  else if (yswz_uint32 >= 10000) yswz_str += "000" + String(yswz_uint32);
                                  else if (yswz_uint32 >= 1000) yswz_str += "0000" + String(yswz_uint32);
                                  else if (yswz_uint32 >= 100) yswz_str += "00000" + String(yswz_uint32);
                                  else if (yswz_uint32 >= 10) yswz_str += "000000" + String(yswz_uint32);
                                  else yswz_str += "0000000" + String(yswz_uint32);
                                  // yswz_count++;
                                  line = 1;
                                  StringLength = jisuankuan;
                                  // yswz_count = 0;  // 待写入计数清空
                          } else {
                            StringLength = StringLength + binzitikuan + zijianju;  //字间距应用。
                          }

            }else{
                        const char s[] = "哪";
                        if(c == '\n'){uniCode = 0 ;}
                        else {uniCode = u8g2Fonts.getUTF8Width(s)+1; } //计算出字的宽。
                        
                                //###############
                          if(display.width()-StringLength < uniCode){
                                  line++;
                                  StringLength = 0;              
                                }
                                //################
                              //   yanzheng = uniCode +zijianju + StringLength;  // #########################################################################
                              // if (yanzheng > guy.width()) {  //注意，每行最后一个字符，可能会超过屏的宽度，这个地方，做好验证。
                                
                              //   line++;
                              //   StringLength = 0;
                              // }
                              if (line > (display.height() / jisuangao)) {

                                //翻页的时候，有一个文字会掉，用下面代码，可以解决。
                                if (decodedValue > 0 && decodedValue < 128) {  // 对于单字节UTF-8，解码值应该小于128
                                  ch_count = 1;
                                } else if (decodedValue <= 2047) {
                                  ch_count = 2;
                                } else if (decodedValue <= 65535) {
                                  ch_count = 3;
                                }  // 如果需要处理四字节，可以添加额外的条件
                                      uint32_t yswz_uint32;

                                      yswz_uint32 = txtFile.position() - ch_count;
                                
                                //页数位置编码处理
                                if (yswz_uint32 >= 10000000) yswz_str += String(yswz_uint32);
                                else if (yswz_uint32 >= 1000000) yswz_str += "0" + String(yswz_uint32);
                                else if (yswz_uint32 >= 100000) yswz_str += "00" + String(yswz_uint32);
                                else if (yswz_uint32 >= 10000) yswz_str += "000" + String(yswz_uint32);
                                else if (yswz_uint32 >= 1000) yswz_str += "0000" + String(yswz_uint32);
                                else if (yswz_uint32 >= 100) yswz_str += "00000" + String(yswz_uint32);
                                else if (yswz_uint32 >= 10) yswz_str += "000000" + String(yswz_uint32);
                                else yswz_str += "0000000" + String(yswz_uint32);
                                // yswz_count++;
                                line = 1;
                                StringLength = jisuankuan;
                                // yswz_count = 0;  // 待写入计数清空
                              } else {
                                StringLength = StringLength + uniCode + zijianju;
                              }
                            
                    }
                

      }
    }
    //Serial.println("打印出yswz");
    //Serial.println(yswz_str);
    txtFile.close();
    yswz_str += wei_str;
    uint16_t length = strlen(yswz_str.c_str());
    uint16_t shull = length / 8;

    //将当前页存入索引文件的末尾七位
    uint16_t yswz_uint16 = shull;
    if (yswz_uint16 >= 10000000) yswz_str += String(yswz_uint16);
    else if (yswz_uint16 >= 1000000) yswz_str += String("0") + String(yswz_uint16);
    else if (yswz_uint16 >= 100000) yswz_str += String("00") + String(yswz_uint16);
    else if (yswz_uint16 >= 10000) yswz_str += String("000") + String(yswz_uint16);
    else if (yswz_uint16 >= 1000) yswz_str += String("0000") + String(yswz_uint16);
    else if (yswz_uint16 >= 100) yswz_str += String("00000") + String(yswz_uint16);
    else if (yswz_uint16 >= 10) yswz_str += String("000000") + String(yswz_uint16);
    else yswz_str += String("0000000") + String(yswz_uint16);

    fileSY1.close();
    fileSY4.open(zjSY3.c_str(), O_RDWR | O_CREAT);

    fileSY4.seekSet(0);
    fileSY4.print(yswz_str + "00000001");  //将待写入的缓存 写入索引文件中
    fileSY4.close();
    //Serial.println(yswz_str + "00000001");
    yswz_str = "";  // 待写入文件清空
    


  } else {  // 有章节页面信息。

    Serial.println("有章节信息");
    fileSY1.close();
    zjymcj();  //调整过字体大小和行间距，需要重建。
  }
}
//END############################################################



void zjymcj(){  //调过字间距和行间距的，都要重新生成一下章节信息。
                      String zjSY = txtnamea + ".ii";
                      String zhangjieye_str ="";
                      uint16_t yswz_uint321;
                      String yswz_str = "";
                      uint32_t dqymsuoyin;
                      uint32_t dqym;
                      uint16_t dqymbj;  //比较页面大小用。
                      Serial.println("进入zjymcj");

                     if(!fileSY1.open(zjSY.c_str(), O_RDWR)){
                       Serial.println("打开失败22");
                     }
                      fileSY1.seekEnd();
                      fileSY1.seekCur(-8);

                      zhangjieye_str = "";
                      for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
                        char c = fileSY1.read();
                        zhangjieye_str += c;
                      }
                      dqym = atol(zhangjieye_str.c_str());  //当前页面 小于65535 

                      fileSY1.seekSet((dqym - 1) * 8);
                      zhangjieye_str = "";
                      for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
                        char c = fileSY1.read();
                        zhangjieye_str += c;
                      }
                      dqymsuoyin = atol(zhangjieye_str.c_str());  //当前页面索引。

                      fileSY1.close();

                      //Serial.println(dqymsuoyin);
                      
                      zjymsc(1);  //从新生成新的章节索引。


                      //#########再读出。。。。。
                      fileSY1.open(zjSY.c_str(), O_RDWR);


                      fileSY1.seekSet(0);
                      dqymbj = 0;
                      while(fileSY1.available()){
                            zhangjieye_str = ""; //每次循环都要清空
                            dqymbj++;//第1页。这里值为1。
                            for (uint8_t tt1 = 0; tt1 < 8; tt1++) {
                              char c = fileSY1.read();
                              zhangjieye_str += c;
                            }
                           // Serial.println(zhangjieye_str);
                            dqym = atol(zhangjieye_str.c_str());  //当前页面索引。
                            // Serial.println(dqym);
                            if (dqym >dqymsuoyin){     //  比较，原来的当前页索引数据，和新生成的章节信息的数据，对比，再生成新的当前页。
                                
                                    //将当前页存入索引文件的末尾七位
                                      yswz_uint321 = dqymbj-1;
                                      yswz_str = "";
                                    // Serial.println("加一页写入文件");
                                      if (yswz_uint321 >= 10000000) yswz_str += String(yswz_uint321);
                                      else if (yswz_uint321 >= 1000000) yswz_str += String("0") + String(yswz_uint321);
                                      else if (yswz_uint321 >= 100000) yswz_str += String("00") + String(yswz_uint321);
                                      else if (yswz_uint321 >= 10000) yswz_str += String("000") + String(yswz_uint321);
                                      else if (yswz_uint321 >= 1000) yswz_str += String("0000") + String(yswz_uint321);
                                      else if (yswz_uint321 >= 100) yswz_str += String("00000") + String(yswz_uint321);
                                      else if (yswz_uint321 >= 10) yswz_str += String("000000") + String(yswz_uint321);
                                      else yswz_str += String("0000000") + String(yswz_uint321);

                                      fileSY1.seekEnd();
                                      // 从文件末尾开始偏移，例如偏移-8字节

                                      fileSY1.seekCur(-8);      //相对前面位移
                                      fileSY1.print(yswz_str);  //写入数据
                                      fileSY1.close();

                            break;
                            }
                      }

}



//字体

std::vector<std::string> listziti(std::string path) {
  // 列出目录下的文件和文件夹
      //hspi.begin(SDSCK_PIN,SDMISO_PIN,SDMOSI_PIN,SDCS_PIN);
      //if(!sd.begin(SdSpiConfig(SDCS_PIN, DEDICATED_SPI, 18000000,&hspi)))
    //    {
    //    Serial.println("error");
    // //     //return;
    //    }

  std::vector<std::string> entries; 
  if (!dir.open(path.c_str())) {
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

    char buf[64];
  while (file4.openNext(&dir, O_RDONLY)) {
    

      // 检查文件扩展名是否为.bin
      file4.getName(buf, sizeof(buf));
      if (String(buf).endsWith(".bin")) {
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




// 检测文件是否为UTF-8编码的函数

bool isUTF8(const char* filename, FsFile& file3) {
  uint8_t c;
  bool isUTF8 = true;

  while (file3.available()) {
    c = file3.read();
    if (c < 0x80) {
      // 1字节，ASCII字符
    } else if ((c & 0xE0) == 0xC0) {
      // 2字节，UTF-8字符
      if (!file3.available()) {
        isUTF8 = false;
        break;
      }
      uint8_t c2 = file3.read();
      if ((c2 & 0xC0) != 0x80) {
        isUTF8 = false;
        break;
      } else {
        break;
      }
    } else if ((c & 0xF0) == 0xE0) {
      // 3字节，UTF-8字符
      if (!file3.available()) {
        isUTF8 = false;
        break;
      }
      uint8_t c2 = file3.read();
      if ((c2 & 0xC0) != 0x80) {
        isUTF8 = false;
        break;
      }
      if (!file3.available()) {
        isUTF8 = false;
        break;
      }
      uint8_t c3 = file3.read();
      if ((c3 & 0xC0) != 0x80) {
        isUTF8 = false;
        break;
      } else {
        break;
      }
    } else if ((c & 0xF8) == 0xF0) {
      // 4字节，UTF-8字符
      if (!file3.available()) {
        isUTF8 = false;
        break;
      }
      uint8_t c2 = file3.read();
      if ((c2 & 0xC0) != 0x80) {
        isUTF8 = false;
        break;
      }
      if (!file3.available()) {
        isUTF8 = false;
        break;
      }
      uint8_t c3 = file3.read();
      if ((c3 & 0xC0) != 0x80) {
        isUTF8 = false;
        break;
      }
      if (!file3.available()) {
        isUTF8 = false;
        break;
      }
      uint8_t c4 = file3.read();
      if ((c4 & 0xC0) != 0x80) {
        isUTF8 = false;
        break;
      } else {
        break;
      }
    } else {
      isUTF8 = false;
      break;
    }
  }

  return isUTF8;
}


//############################################################################

// 定义UTF-8解码状态的枚举
enum utf8_decode_state_t : uint8_t {
  utf8_state0 = 0,
  utf8_state1 = 1,
  utf8_state2 = 2
  };

  // 定义解码器状态和Unicode缓冲区变量
  //byte _decoderState = utf8_state0;   // UTF-8解码器状态
  utf8_decode_state_t _decoderState = utf8_state0;
  uint16_t _unicode_buffer = 0;  // Unicode码点缓冲区

  // 定义UTF-8解码函数
  uint16_t decodeUTF8(uint8_t c) {
  // 7位Unicode码点
  if (!(c & 0x80)) {
    _decoderState = utf8_state0;
    return c;
  }

  if (_decoderState == utf8_state0) {
    // 11位Unicode码点
    if ((c & 0xE0) == 0xC0) {
      _unicode_buffer = ((c & 0x1F) << 6);
      _decoderState = utf8_state1;
      return 0;
    }

    // 16位Unicode码点
    if ((c & 0xF0) == 0xE0) {
      _unicode_buffer = ((c & 0x0F) << 12);
      _decoderState = utf8_state2;
      return 0;
    }
    // 21位Unicode码点不支持，因此回退到扩展ASCII
    // if ((c & 0xF8) == 0xF0) return (unsigned int)c;
  } else {
    if (_decoderState == utf8_state2) {
      _unicode_buffer |= ((c & 0x3F) << 6);
      _decoderState = utf8_state1;
      return 0;
    }
    _unicode_buffer |= (c & 0x3F);
    _decoderState = utf8_state0;
    return _unicode_buffer;
  }

  _decoderState = utf8_state0;

  return c;  // 回退到扩展ASCII
}

//############################################################################


//##############################################

void display_partialLine(uint8_t line, String zf) ////发送局部刷新的显示信息到屏幕,带居中
  {
  /*
    display_partialLine()
    发送局部刷新的显示信息到屏幕,带居中

    line        行数（0-7）
    zf          字符内容
    lineRefresh 整行刷新 1-是 0-仅刷新字符长度的区域
  */
  u8g2Fonts.setFont(gancao);
  const char *character = zf.c_str();                            //String转换char
  uint16_t zf_width = u8g2Fonts.getUTF8Width(character);         //获取字符的像素长度
  uint16_t x = (display.width() / 2) - (zf_width / 2);           //计算字符居中的X坐标（屏幕宽度/2-字符宽度/2）

    display.fillRect(x, line*16+13, zf_width, 14, 0);   //背景清白
    u8g2Fonts.setCursor(x, line * 16 + 13);
    u8g2Fonts.print(character);
    display.display();
  //display.powerOff(); //关闭屏幕电源
}




void display_drawCenter(uint16_t y, String zf) ////发送局部刷新的显示信息到屏幕,带居中
  {
  /*
    display_partialLine()
    发送局部刷新的显示信息到屏幕,带居中

    line        行数（0-7）
    zf          字符内容
    lineRefresh 整行刷新 1-是 0-仅刷新字符长度的区域
  */
  //u8g2Fonts.setFont(gancao);
  const char *character = zf.c_str();                            //String转换char
  uint16_t zf_width = u8g2Fonts.getUTF8Width(character);         //获取字符的像素长度
  uint16_t x = (display.width() / 2) - (zf_width / 2);           //计算字符居中的X坐标（屏幕宽度/2-字符宽度/2）

    display.drawRoundRect(x - 3, y, zf_width+6, 20, 5, 1);

  }



void wenshidu(){
        sht.read(); 
        wenduo = String(sht.getTemperature(), 1);
        shiduo = String(sht.getHumidity(), 1);
}
