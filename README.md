# Agent GSM

This agent is based on a GPRS connection in order to communicate with external services

## Currently Used Libraries

- PlatformIO: [https://platformio.org/]
- TinyGSM: [https://github.com/vshymanskyy/TinyGSM]
- PubSubClient: [https://github.com/knolleary/pubsubclient]

## Links

- [https://lastminuteengineers.com/sim800l-gsm-module-arduino-tutorial/]
- [https://github.com/vshymanskyy/TinyGSM]
- [https://www.instructables.com/SMS-Controller-by-Using-SIM800L-V2/]
- [https://andreasrohner.at/posts/Electronics/How-to-modify-an-Arduino-Pro-Mini-clone-for-low-power-consumption/]
- [https://github.com/rocketscream/Low-Power]

## Sensor Links

1. PH
   1. [https://wiki.dfrobot.com/Gravity__Analog_pH_Sensor_Meter_Kit_V2_SKU_SEN0161-V2]
   2. [https://cimpleo.com/blog/simple-arduino-ph-meter/]
   3. [https://scidle.com/how-to-use-a-ph-sensor-with-arduino/]
   4. [https://www.electroniclinic.com/ph-meter-arduino-ph-meter-calibration-diymore-ph-sensor-arduino-code/]
   5. [https://how2electronics.com/ph-meter-using-ph-sensor-arduino-oled/]
   6. [https://www.electroniclinic.com/ph-sensor-arduino-how-do-ph-sensors-work-application-of-ph-meter-ph-sensor-calibration/]
   7. [https://www.marinostore.com/sensores/sensores/sensor-de-ph-com-modulo-ph4502c]
2. Turbidity
   1. [https://www.usinainfo.com.br/blog/sensor-de-turbidez-projeto-de-leitura-da-qualidade-da-agua/]
   2. [https://forum.arduino.cc/index.php?topic=398900.0]

## PH Convertion Code

```js
var ph_7 = parseFloat(metadata.ss_ph_7); // 2500
var ph_4 = parseFloat(metadata.ss_ph_4); // 4000

var slope = (7.0 - 4.0) / (ph_7 - ph_4);
var ph = slope * ( msg.p - ph_7 ) + 7.0;

msg.PH = ph;

return {msg: msg, metadata: metadata, msgType: msgType};
```

## Turbidity Convertion COde

```js
var x = parseFloat(msg.b) / 1000.0;
var NTU = 0;

if (x < 2.5) // Min Voltage
{
    NTU = 3000;
}
else if (x > 4.2) // MAX Voltage
{
    NTU = 0;
}
else
{
    NTU = (-1120.4 * x * x) + (5742.3 * x) - 4352.9;
}

msg.NTU = NTU;

return {msg: msg, metadata: metadata, msgType: msgType};
```
## PH Sensor by Elecrow
`` `js
//Sensor pH com conversor da Elecrow que vem sem Trimpot

#define Vref 4.95 //Tensão Média de saída do conversor para a probe (sensor).Essa tensão Média foi informada pela vendedora do produto www.elecrow.com

unsigned long int avgValue;     //Armazena o valor médio dos valores lidps pelo sensor

int i=0;

void setup()
{
    Serial.begin(9600);
    
    pinMode(A0, INPUT);
    
    pinMode(A1, OUTPUT);      //Não utilizado
}
void loop()
{
    float sensorValue;
    
    int m;                    //Não utilizado
    
    long sensorSum;           //Não utilizado
    
    int buf[10];              //buffer para a leitura da entrada analógica
    
  for(int i = 0; i < 10; i++) //Obtem 10 valores da leitura analógica (Amostra) do sensor para suavizar o valor 
  { 
    buf[i] = analogRead(A0);//Conecte o sensor no pino analógico A0
    
    delay(10);
  }
  for(int i = 0; i < 9; i++)        //Classifica os valores lidos do menor para o maior
  {
    for(int j = i + 1; j < 10; j++)
    {
      if(buf[i] > buf[j])
      {
        int temp = buf[i];
        
        buf[i] = buf[j];
        
        buf[j] = temp;
      }
    }
  }
       avgValue=0;
 
      for(int i=2;i<8;i++)       //Colhe os valores de 6 amostras centrais. Começa da amostra na posição 2(Terceiro valor lido) até a amostra da posição 8(Sétimo valor lido)
      
      avgValue+=buf[i];          //Soma os valores das 6 amostras centrais colhidas e atribui essa soma à variável avgValue
    
     sensorValue =   avgValue/6; //Calcula a média dessas 6 amostras centrais e atribui à variável sensorValue. Aqui é fornecida a Tensão Média lida pelo sensor em bytes
     
    Serial.print("Leitura Média: ");
    
    Serial.println(sensorValue); //Imprime a Tensão Média lida pelo Sensor em bytes
    
    Serial.println(" ");
    
    Serial.print("pH: ");
    
    Serial.print(7-1000*(sensorValue-391)*Vref/59.16/1023,2);// 7 é o valor de referência baseando-se numa solução de pH = 7. SensorValue é a leitura média da Tensão lida em bytes.
    
    //391 é a Queda de Tensão quando colocamos a entrada do conversor em curto. 
    
    //Esse procedimento deve ser repetido para todo novo sensor a ser calibrado. O valor fornecido inicialmente pela fabricante é 365
   
    //Vref é a tensão de saída referência do conversor para o sensor. Foi fornecida pelo Fabricante a Tensão Referência de 4.95 volts.
    
    //59.16 é uma constante que ainda não entendi de onde foi tirada, mas que é essencial para o cálculo do pH##
    
    Serial.println(" ");
    
    delay(1000);
}
```
