void musicplaying(void * pvParams) {

       //SD.begin();
     // SD.begin(SDCS_PIN, hspi);
    if (!SD.begin(SDCS_PIN,hspi,40000000)) { // 这里的4是自定义的CS引脚号
      Serial.println("initialization failed!");
     return;
    }   

    CreateQueues();
    if(!audioSetQueue || !audioGetQueue){
        log_e("queues are not initialized");
        while(true){;}  // endless loop
    }

    struct audioMessage audioRxTaskMessage;
    struct audioMessage audioTxTaskMessage;



      source = new AudioFileSourceSD();  
      out = new AudioOutputI2S();
      mp3 = new AudioGeneratorMP3();
      id3 = new AudioFileSourceID3(source);
     // id3->setCallback(myCallback); // 设置回调函数

               // bufferedSource = new AudioFileSourceBuffer(id3, 1024*2);
               // bufferedOutput = new AudioOutputBuffer(1024*2, out); //将缓冲输出连接到 I2S 输出    

        // 设置 I2S 引脚（根据你的硬件连接）
        out->SetGain(currentVolume); // 初始音量
        out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT); // BCK, LRC, DIN
        //  bufferedOutput->SetOutput(out); // 
      //bufferedSource = new AudioFileSourceBuffer(source, 1024);
        pinMode(I2S_POW, OUTPUT);
        digitalWrite(I2S_POW, HIGH);   
        //source->open("/music/三国恋.mp3");

        

      while(1){


          //vTaskDelay(1); 
        if(xQueueReceive(audioSetQueue, &audioRxTaskMessage, 1) == pdPASS) {
            if(audioRxTaskMessage.cmd == SET_VOLUME){
                audioTxTaskMessage.cmd = SET_VOLUME;
                //audio.setVolume(audioRxTaskMessage.value);
                //out->SetGain(audioRxTaskMessage.value);
                out->SetGain(currentVolume); // 设置音量
                audioTxTaskMessage.ret = 1;
                xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
            }
            else if(audioRxTaskMessage.cmd == CONNECTTOHOST){
                audioTxTaskMessage.cmd = CONNECTTOHOST;
                //audioTxTaskMessage.ret = audio.connecttohost(audioRxTaskMessage.txt);
                xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
            }
            else if(audioRxTaskMessage.cmd == CONNECTTOFS){
                audioTxTaskMessage.cmd = CONNECTTOFS;
                        if (mp3->isRunning()) {
                           // audio.pauseResume();
                            mp3->stop();
                            vTaskDelay(10); 
                          }
                         // bufferedSource->close();
                          //delete bufferedOutput; 
               // audioTxTaskMessage.ret = audio.connecttoFS(SD,audioRxTaskMessage.txt);
                audioTxTaskMessage.ret =  source->open(audioRxTaskMessage.txt);
                Serial.printf(audioRxTaskMessage.txt);
                id3 = new AudioFileSourceID3(source);
     // id3->setCallback(myCallback); // 设置回调函数                
               // bufferedSource = new AudioFileSourceBuffer(id3, 1024*3);
               // bufferedOutput = new AudioOutputBuffer(1024*3, out); //将缓冲输出连接到 I2S 输出                
               // mp3->begin(bufferedSource, bufferedOutput);
                mp3->begin(id3, out);
                xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
            }
            else if(audioRxTaskMessage.cmd == GET_VOLUME){
                audioTxTaskMessage.cmd = GET_VOLUME;
                //audioTxTaskMessage.ret = audio.getVolume();
                xQueueSend(audioGetQueue, &audioTxTaskMessage, portMAX_DELAY);
            }
            else{
                log_i("error");
            }
        }


             if (mp3->isRunning()) {
                
               if (!mp3->loop()) mp3->stop();
             } else {
               //Serial.printf("MP3 done\n");
               vTaskDelay(100);
             }
         // vTaskDelay(1);

      }

}



audioMessage transmitReceive(audioMessage msg){
    xQueueSend(audioSetQueue, &msg, portMAX_DELAY);
    if(xQueueReceive(audioGetQueue, &audioRxMessage, portMAX_DELAY) == pdPASS){
        if(msg.cmd != audioRxMessage.cmd){
            log_e("wrong reply from message queue");
        }
    }
    return audioRxMessage;
}

void audioSetVolume(float vol){
    audioTxMessage.cmd = SET_VOLUME;
    audioTxMessage.value = vol;
    audioMessage RX = transmitReceive(audioTxMessage);
}

uint8_t audioGetVolume(){
    audioTxMessage.cmd = GET_VOLUME;
    audioMessage RX = transmitReceive(audioTxMessage);
    return RX.ret;
}

bool audioConnecttohost(const char* host){
    audioTxMessage.cmd = CONNECTTOHOST;
    audioTxMessage.txt = host;
    audioMessage RX = transmitReceive(audioTxMessage);
    return RX.ret;
}

bool audioConnecttoFS(const char* filename){
    audioTxMessage.cmd = CONNECTTOFS;
    audioTxMessage.txt = filename;
    audioMessage RX = transmitReceive(audioTxMessage);
    return RX.ret;
}






