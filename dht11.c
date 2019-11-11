#include<wiringPi.h>
#include<stdio.h>

#define PORT 0


short readData(void)
{
    short data;
    char data_buf[40];
    char i;

    pinMOde(PORT,OUTPUT); // set OUTPUT
    digitalWrite(PORT,0);  // start signal
    delay(18);  // delay18ms
    digitalWrite(PORT,1); // set high level
    delayMicoseconds(40);// delay 40 us
    pinMode(PORT,INPUT); //set input MOde
    while(digitalRead(PORT));// wait slave respond
    delayMicoseconds(80); // delay 80us
    while(digitalRead(PORT));
    // receive 40 bit datas
    for(i=0;i<40;i++)
    {
        delayMicoseconds(50);// delay 50 us
        while(!digitalRead(PORT));
        delayMicoseconds(30); // delay 30 us
        if(digitalRead(PORT) == 1)  // data is '1'
        {
            data[i] = 1;
            while(digitalRead(PORT)); // wait signel converts low
        }
        else   // data is 0
        {
            data[i] = 0;
        }
    }
    delayMicoseconds(50);  // delay 50 us;  stop
    pinMode(PORT,OUTPUT);
    digitalWrite(PORT,1);

    //  Verify that the data is transmitted correctly
    char temp,humidity,checksum;
    for(i=0;i<8;i++)
    {
        temp = data[i];
        temp = temp<<1;

        humidity = data[i+16];
        humidity = humidity <<1;

        checksum = data[i+32];
        checksum = checksum <<1;
    }
    if((temp+humidity) != checksum)
    {
        return -1;
    }

    data = temp;
    data = (data<<8) + humidity;  // high 8 bits is temperture, low 8 bits is humidity

    return data;
}

/**
* main function
*/
int main(void)
{
    short data;
    char temp,humidity;
    // wiringPi gpio set
    wiringPiSetup();
    delay(1000);  //delay 1 s after power-on
    pinMode(PORT,OUTPUT);
    delay(1);
    while(1)
    {
        data = readData();
        temp = data>>8;
        humidity = data&0x0ff;
        printf("temperture=: %d C humidity=: %d \n",temp,humidity);
        delay(1000);
    }

    return 0;
}
