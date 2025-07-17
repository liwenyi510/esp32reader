#define youb 5//32 33 25
#define zhongb 6//32 33 25
#define zuob 7//32 33 25

OneButton button1(youb, true);    //key1
OneButton button2(zuob, true);    //key2
OneButton button3(zhongb, true);    //key3

void onebutton(void * pvParams) {

  button1.attachClick(click1);
  //button1.attachDoubleClick(doubleclick1);
  button1.attachLongPressStart(longclick1);

  button2.attachClick(click2);
  //button2.attachDoubleClick(doubleclick2);
  button2.attachLongPressStart(longclick2);

  button3.attachClick(click3);
  button3.attachDoubleClick(doubleclick3);
  button3.attachLongPressStart(longclick3);


  while(1){
    button1.tick();   //右键
    button2.tick();  //左键
    button3.tick();  //中键
    vTaskDelay(10);
    }

}



void click1()  //右键
  {
            switch (gongneng){
              case 0:
                  xTaskNotify( xzhu, 0b00000001, eSetValueWithOverwrite ); //传输值1到该任务
              break;
              case 1:
                  xResult=xTaskNotify( xtxtlist, 0b00000001, eSetValueWithOverwrite ); //传输值1到该任务
              break;
              case 2:
                  xResult=xTaskNotify( xyuedu, 0b00000001, eSetValueWithOverwrite ); //传输值1到该任务
              break;
              case 3:
                  xResult=xTaskNotify( xtianqi, 0b00000001, eSetValueWithOverwrite ); //传输值1到该任务
              break;
              case 4:
                  xResult=xTaskNotify( xmusiclist, 0b00000001, eSetValueWithOverwrite ); //覆盖原有的值3
              break;   
            }    
  }

void click2()
  {
            //左键单击
              switch (gongneng){
                case 0:
                    xResult=xTaskNotify( xzhu, 0b00000010, eSetValueWithOverwrite ); //传输值2到该任务
                break;
                case 1:
                    xResult=xTaskNotify( xtxtlist, 0b00000010, eSetValueWithOverwrite ); //传输值2到该任务
                break;
                case 2:
                    xResult=xTaskNotify( xyuedu, 0b00000010, eSetValueWithOverwrite ); //传输值2到该任务
                break;
                case 3:
                    xResult=xTaskNotify( xtianqi, 0b00000010, eSetValueWithOverwrite ); //传输值2到该任务
                break;
                case 4:
                  xResult=xTaskNotify( xmusiclist, 0b00000010, eSetValueWithOverwrite ); //覆盖原有的值3
                break; 
              }    
  }

void click3()
  {
                //中间确认单击
              switch (gongneng){
                case 0:
                  xResult=xTaskNotify( xzhu, 0b00000011, eSetValueWithOverwrite ); //传输值3到该任务
                break;
                case 1:
                  xResult=xTaskNotify( xtxtlist, 0b00000011, eSetValueWithOverwrite ); //传输值3到该任务
                break;
                case 2:
                  xResult=xTaskNotify( xyuedu, 0b00000011, eSetValueWithOverwrite ); //传输值3到该任务
                break;
                case 3:
                  xResult=xTaskNotify( xtianqi, 0b00000011, eSetValueWithOverwrite ); //传输值3到该任务
                break;
                case 4:
                  xResult=xTaskNotify( xmusiclist, 0b00000011, eSetValueWithOverwrite ); //覆盖原有的值3
                break;     
              }
  }

void longclick3()     //中间长按
  {
              switch (gongneng){
                case 0:
          //        xResult=xTaskNotify( xzhu, 0b00000011, eSetValueWithOverwrite ); //传输值4到该任务
                break;
                case 1:
                  xTaskNotify( xtxtlist, 0b00000100, eSetValueWithOverwrite ); //传输值4到该任务
                break;
                case 2:
                  xResult=xTaskNotify( xyuedu, 0b00000100, eSetValueWithOverwrite ); //传输值4到该任务
                break;
                case 3:
                  xResult=xTaskNotify( xtianqi, 0b00000011, eSetValueWithOverwrite ); //传输值4到该任务
                break;
                case 4:
                  xResult=xTaskNotify( xmusiclist, 0b00000100, eSetValueWithOverwrite ); //覆盖原有的值4
                break;
              }   


  }




 void longclick1()   //右键长按
{

            //左键单击
              switch (gongneng){
                case 0:
                   // xResult=xTaskNotify( xzhu, 0b00000010, eSetValueWithOverwrite ); //覆盖原有的值5
                break;
                case 1:
                    //xResult=xTaskNotify( xtxtlist, 0b00000010, eSetValueWithOverwrite ); //覆盖原有的值5
                break;
                case 2:
                    //xResult=xTaskNotify( xyuedu, 0b00000010, eSetValueWithOverwrite ); //覆盖原有的值5
                break;
                case 3:
                   // xResult=xTaskNotify( xtianqi, 0b00000010, eSetValueWithOverwrite ); //覆盖原有的值5
                break;
                case 4:
                   xResult=xTaskNotify( xmusiclist, 0b00000101, eSetValueWithOverwrite ); //覆盖原有的值5
                break;                
              }    

} 

 void longclick2()   //左键长按
{

            //左键单击
              switch (gongneng){
                case 0:
                   // xResult=xTaskNotify( xzhu, 0b00000010, eSetValueWithOverwrite ); //覆盖原有的值6
                break;
                case 1:
                    //xResult=xTaskNotify( xtxtlist, 0b00000010, eSetValueWithOverwrite ); //覆盖原有的值6
                break;
                case 2:
                    //xResult=xTaskNotify( xyuedu, 0b00000010, eSetValueWithOverwrite ); //覆盖原有的值6
                break;
                case 3:
                   // xResult=xTaskNotify( xtianqi, 0b00000010, eSetValueWithOverwrite ); //覆盖原有的值6
                break;
                case 4:
                   xResult=xTaskNotify( xmusiclist, 0b00000110, eSetValueWithOverwrite ); //覆盖原有的值6
                break;                
              }    

} 






void doubleclick3()
  {
  Serial.println("doubleclick3");
  }
