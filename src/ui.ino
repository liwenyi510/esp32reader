
// 168 384 分辨率


void msgbox_yn(const char *title, const char *msg, uint8_t i)   //进入阅读ui
{
        
       
        uint16_t w = 230;
        uint16_t h = 90;
        uint16_t zw ;

        uint16_t start_x = (display.width() - w) / 2;
        uint16_t start_y = (display.height() - h) / 2;
      
       

        display.fillRect(start_x, start_y, w, h, 0);             //背景清白
        display.drawRoundRect(start_x, start_y, w, h, 5, 1);     // 230*90窗口背景，圆角5

      
        display.drawFastHLine(start_x, start_y + 18, w, 1);  //标题区域
        int16_t wchar = u8g2Fonts.getUTF8Width(title);
        u8g2Fonts.setCursor(start_x + (w - wchar) / 2, start_y + 16);
        u8g2Fonts.print(title);


        
        u8g2Fonts.setCursor(start_x + 5, start_y + 35);   // 内容区域
        autoIndentDraw(msg, start_x + w - 5, start_x + 5);


        // 按钮

        if(i==2){
          display.drawRoundRect(start_x + 5, start_y + 90 - 22, 70, 18, 3, 1);
        }
        else if(i==1){
          display.drawRoundRect(start_x + 155, start_y + 90 - 22, 70, 18, 3, 1);
        }
        
      

        zw = u8g2Fonts.getUTF8Width("取消");
        u8g2Fonts.setCursor(start_x + 5 + (70 - zw) / 2, start_y + 90 - 20 + 12);
        u8g2Fonts.print("取消");
        zw = u8g2Fonts.getUTF8Width("确定");
        u8g2Fonts.setCursor(start_x + 155 + (70 - zw) / 2, start_y + 90 - 20 + 12);
        u8g2Fonts.print("确定");
      
}

    // 自动换行
void autoIndentDraw(const char *str, int max_x, int start_x)
    {
        while (*str)
        {
            if (u8g2Fonts.getCursorX() >= max_x || *str == '\n')
            {
                u8g2Fonts.setCursor(start_x, u8g2Fonts.getCursorY() + 16);
            }
            if (*str != '\n')
            {
                u8g2Fonts.print(*str);
            }
            str++;
        }
    }

void erjicaidanfc(uint8_t er,String erjicaidan[],uint8_t erjiarraySize){    //二级菜单ui

       //字大概是16

        uint16_t w = 180;
        uint16_t h = 130;
        uint16_t zw ;
        uint16_t zuobian = 120;
        uint8_t  biaotigao = 18;
        uint16_t xiaogegao = (h-biaotigao)/3;


        uint16_t start_x = (display.width() - w) / 2;
        uint16_t start_y = (display.height() - h) / 2;


        display.fillRect(start_x, start_y, w, h, 0);             //背景清白
        display.drawRoundRect(start_x, start_y, w, h, 5, 1);   //外围圆角框
        display.drawFastHLine(start_x, start_y + biaotigao, w, 1);  //标题区域

        int16_t wchar = u8g2Fonts.getUTF8Width("阅读调整");
        u8g2Fonts.setCursor(start_x + (w - wchar) / 2, start_y + 15);
        u8g2Fonts.print("阅读调整");
        display.drawFastVLine(start_x +zuobian, start_y + biaotigao, h-biaotigao, 1);  //分隔线

          for (uint8_t p = 1; p < 3; p++) {
                  display.drawFastHLine(start_x+zuobian, start_y + biaotigao+xiaogegao*p, w-zuobian, 1);  //小格区域
          }

            for (uint8_t p = 0; p < erjiarraySize; p++) {    
              display_linecenter(start_y + 18 +biaotigao + 21 * p,erjicaidan[p],start_x,zuobian);                           
            }  

        display_drawquan(start_y + 20 + 21 * (er-1),erjicaidan[er-1],start_x,zuobian);
        display.display();  

}


void yijicaidanfc(uint8_t txtMenuCount,String yijicaidan[],uint8_t arraySize){   //一级菜单ui

      /*
        ------------------------------
        -                            -
        -                            -
        -                            -
        -                            -
        -                            -
        ------------------------------
      */


        //字大概是16

        uint16_t w = 180;
        uint16_t h = 130;
        uint16_t zw ;
        uint16_t zuobian = 120;
        uint8_t  biaotigao = 18;
        uint16_t xiaogegao = (h-biaotigao)/3;

        uint16_t start_x = (display.width() - w) / 2;
        uint16_t start_y = (display.height() - h) / 2;


        display.fillRect(start_x, start_y, w, h, 0);             //背景清白
        display.drawRoundRect(start_x, start_y, w, h, 5, 1);   //外围圆角框
        display.drawFastHLine(start_x, start_y + biaotigao, w, 1);  //标题区域

        int16_t wchar = u8g2Fonts.getUTF8Width("设置");
        u8g2Fonts.setCursor(start_x + (w - wchar) / 2, start_y + 15);
        u8g2Fonts.print("设置");

        display.drawFastVLine(start_x +zuobian, start_y + biaotigao, h-biaotigao, 1);  //分隔线

          for (uint8_t p = 1; p < 3; p++) {
                  display.drawFastHLine(start_x+zuobian, start_y + biaotigao+xiaogegao*p, w-zuobian, 1);  //小格区域
          }

          for (uint8_t p = 0; p < arraySize; p++) {
            display_linecenter(start_y + 18 +biaotigao + 21 * p,yijicaidan[p],start_x,zuobian);                              
          }  
        display_drawquan(start_y + 20 + 21 * (txtMenuCount-1),yijicaidan[txtMenuCount-1],start_x,zuobian);
        display.display();  
}


void wenjianliebiaoui(const char *title,uint8_t wenjian,const std::vector<std::string>& wenjians){    // 168 384 分辨率    //  文件列表ui

        uint16_t w = 380;
        uint16_t h = 164;
        uint16_t zw ;
        uint16_t zuobian = 120;
        uint8_t  biaotigao = 18;
        uint16_t xiaogegao = (h-biaotigao)/3;
        uint16_t start_x = (display.width() - w) / 2;
        uint16_t start_y = (display.height() - h) / 2;

        uint8_t xianshishuliang;   //显示数量
        uint8_t wenjianpage = 0;    //文件页面
        uint8_t wenjianstartLine  = 0;   //文件开始行
        uint8_t wenjianlinesPerPage = 7;  //  这是每页显示多少行文件
        uint8_t wenjiankuang=1;      //文件框框
        
       // display.clearDisplay();      //清空屏
        
        display.drawRoundRect(start_x, start_y, w, h, 3, 1);
        display.drawFastHLine(start_x, start_y + biaotigao, w, 1);  //标题区域
        int16_t wchar = u8g2Fonts.getUTF8Width(title);
        u8g2Fonts.setCursor(start_x + (w - wchar) / 2, start_y + 15);
        u8g2Fonts.print(title);


                                      if (wenjians.empty()) {   //空文件夹
                                          u8g2Fonts.setCursor(100, 50);
                                          u8g2Fonts.print("无文件");                                        

                                        }
                                        else{  //非空文件夹
                                              if(wenjians.size()<=wenjianlinesPerPage){  //数量小于 每页显示行数。
                                                        xianshishuliang = wenjians.size();
                                                        wenjiankuang = wenjian;
                                                }else{

                                                      wenjianpage=(wenjian-1)/wenjianlinesPerPage;   //从第0页开始。
                                                      wenjiankuang=wenjian%wenjianlinesPerPage;   ///取余
                                                      if(wenjiankuang==0)
                                                                  {wenjiankuang=wenjianlinesPerPage;}     // 余数为0的时候，是每页显示最大的那个数
                                                      if(wenjians.size()-wenjianpage*wenjianlinesPerPage<=wenjianlinesPerPage)
                                                                  {xianshishuliang = wenjians.size()-wenjianpage*wenjianlinesPerPage;}
                                                      else {xianshishuliang =wenjianlinesPerPage;}
                                                      wenjianstartLine = wenjianpage*wenjianlinesPerPage;
                                                    }
                                                  
                                            for (uint8_t p = 0; p < xianshishuliang; p++) {
                                              u8g2Fonts.setCursor(start_x+4, start_y + biaotigao + 18 + p* 20);   
                                              u8g2Fonts.print(wenjians[wenjianstartLine+p].c_str());
                                              }
                                            display.drawRoundRect(start_x+2, start_y + biaotigao + 3 + (wenjiankuang-1)*20, display.width()-7, 18, 5, 1); //圆角外框  

                                        }

}

void yulanui(uint8_t weizhi,uint8_t wu){  //预览ui界面

        uint16_t w = 200;  //宽
        uint16_t h = 25;  //高
        uint16_t start_x = (display.width() - w) / 2;
        uint16_t start_y = display.height() - h -9;
        uint16_t lianggejuli[2] = {start_x+10-2,start_x + w-10 -2-u8g2Fonts.getUTF8Width("确定")};  //   2个x距离 （确定       返回）
        uint16_t wugejuli[2] = {start_x+10-2,203};  //     5个距离  （确定 + 1 - 返回）
        display.fillRect(start_x, start_y, w, h, 0);             //背景清白
        display.drawRoundRect(start_x , start_y, w , h, 3, 1);

        if(wu == 1){




        }
        else{
            u8g2Fonts.setCursor(start_x+10, start_y + 18 );
            u8g2Fonts.print("返回"); 
            u8g2Fonts.setCursor(start_x + w-10-u8g2Fonts.getUTF8Width("确定"), start_y + 18);
            u8g2Fonts.print("确定");  
            display.drawRoundRect(lianggejuli[weizhi-1], start_y+4, u8g2Fonts.getUTF8Width("返回")+6, 18, 2, 1);          
        }

                                
}


void display_linecenter(uint16_t y, String zf,uint16_t x,uint16_t w) ////画字居中
    {
      /*
        display_partialLine()
        发送局部刷新的显示信息到屏幕,带居中
        zf          字符内容
        lineRefresh 整行刷新 1-是 0-仅刷新字符长度的区域
      */
      u8g2Fonts.setFont(gancao);
      const char *character = zf.c_str();                            //String转换char
      uint16_t zf_width = u8g2Fonts.getUTF8Width(character);         //获取字符的像素长度
      uint16_t xx = (x+w/2)-(zf_width / 2);           //计算字符居中的X坐标（屏幕宽度/2-字符宽度/2）

     // display.fillRect(xx, y, zf_width, 14, 0);   //背景清白
      u8g2Fonts.setCursor(xx, y);
      u8g2Fonts.print(character);
      //display.display();
    }
void display_drawquan(uint16_t y, String zf,uint16_t x,uint16_t w) ////画圈居中
{
  /*
    display_partialLine()
    发送局部刷新的显示信息到屏幕,带居中

    line        行数（0-7）
    zf          字符内容
    lineRefresh 整行刷新 1-是 0-仅刷新字符长度的区域
  */
  //u8g2Fonts.setFont(gancao);
      u8g2Fonts.setFont(gancao);
      const char *character = zf.c_str();                            //String转换char
      uint16_t zf_width = u8g2Fonts.getUTF8Width(character);         //获取字符的像素长度
      uint16_t xx = (x+w/2)-(zf_width / 2);           //计算字符居中的X坐标（屏幕宽度/2-字符宽度/2）

    display.drawRoundRect(xx - 3, y, zf_width+6, 20, 5, 1);

}



