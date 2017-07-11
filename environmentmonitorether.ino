#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//MAC address
IPAddress ip(192, 168, 137, 20);         // ip in lan (that's what you need to use in your browser. ("192.168.137.20")
//byte gateway[] = { 192, 168, 1, 1 };
//byte subnet[] = { 255, 255, 255, 0 };
EthernetServer server(80);               //server port

#define pHsensorPin 2         
unsigned long int avgValue;  
float phValue=0.0;

 int buf[10],temp;
 int inp1 = A2;
 int inp2 = A3;

 int inf1 = 3;
 int inf2 = 4;
 int ine1 = 5;
 int ine2 = 6;
 int inp3 = 7;
 int inp4 = 8;
 int led =  2;
#define DHTPIN     9
 int sensorPin = A0;
 int sensorValue = 0;
 int ldrPin = A1; 
 int ldrValue = 0;
 
#define DHTTYPE           DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

sensors_event_t event;
int sensorValue1;
int Light;

void setup() 
{
  Serial.begin(9600);
  dht.begin();
  pinMode(inf1,OUTPUT);
  pinMode(inf2,OUTPUT);
  pinMode(ine1,OUTPUT);
  pinMode(ine2,OUTPUT);
  pinMode(inp3,OUTPUT);
  pinMode(inp4,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(sensorPin,INPUT);
  pinMode(pHsensorPin,INPUT);
  pinMode(inp1,OUTPUT); 
  pinMode(inp2,OUTPUT);
  Ethernet.begin(mac, ip);
  server.begin();
}
void loop()
{
  ether();
}

void ether()
{
  String readString;
  EthernetClient client;
  // Create a client connection
  client = server.available();
  if (client)
  {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        //read char by char HTTP request
        if (readString.length() < 100)
        {
          //store characters to string
          readString += c;
        }

        //if HTTP request has ended
        if (c == '\n')
        {
          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();
          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Arduino Project</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");
          client.println("<br />");
          client.println("<H2>ENVIRONMENTAL MONITORING AND CONTROLLING</H2>");

          process(client);
                 
          client.println("<br />");
          client.println("<br />");
          client.println("<b>FAN &nbsp &nbsp :</b>");
          client.println("<a href=\"/?FON\"\">ON</a>");
          client.println("<a href=\"/?FOFF\"\">OFF</a><br />");

          client.println("<br />");
          client.println("<b>EFAN &nbsp :</b>");
          client.println("<a href=\"/?EON\"\">ON</a>");
          client.println("<a href=\"/?EOFF\"\">OFF</a><br />");

          client.println("<br />");
          client.println("<b>MOTOR :</b>");
          client.println("<a href=\"/?MON\"\">ON</a>");
          client.println("<a href=\"/?MOFF\"\">OFF</a><br />");

          client.println("<br />");
          client.println("<b>PUMP &nbsp :</b>");
          client.println("<a href=\"/?PON\"\">ON</a>");
          client.println("<a href=\"/?POFF\"\">OFF</a><br />");

          client.println("<br />");
          client.println("<b>LAMP &nbsp :</b>");
          client.println("<a href=\"/?LON\"\">ON</a>");
          client.println("<a href=\"/?LOFF\"\">OFF</a><br />");
          
          client.println("</BODY>");
          client.println("<head>");
          client.println("<meta http-equiv='refresh' content='1'>");
          client.println("</head>");
          client.println("</HTML>");

          //delay(1);
          //stopping client
          client.stop();
          //controls the Arduino if you press the buttons
          Serial.println(readString);
          if(readString.indexOf("?FON")>0)
          {
            Serial.println("fan");
            digitalWrite(inf1,HIGH);
            digitalWrite(inf2,LOW);
          }
          if(readString.indexOf("?FOFF")>0)
          {
            digitalWrite(inf1,LOW);
            digitalWrite(inf2,LOW);
          }
          if(readString.indexOf("?EON")>0)
          {
            digitalWrite(ine1,HIGH);
            digitalWrite(ine2,LOW);
          }
          if(readString.indexOf("?EOFF")>0)
          {
            digitalWrite(ine1,LOW);
            digitalWrite(ine2,LOW);
          }
          if(readString.indexOf("?MON")>0)
          {
            digitalWrite(inp3,HIGH);
            digitalWrite(inp4,LOW);
          }
          if(readString.indexOf("?MOFF")>0)
          {
            digitalWrite(inp3,LOW);
            digitalWrite(inp4,LOW);
          } 
          if(readString.indexOf("?PON")>0)
          {
            digitalWrite(inp1,LOW);
            digitalWrite(inp2,LOW);
          } 
          if(readString.indexOf("?POFF")>0)
          {
            digitalWrite(inp1,LOW);
            digitalWrite(inp2,LOW);
          }
          if(readString.indexOf("?LON")>0)
          {
            digitalWrite(led,HIGH);
          }
          if(readString.indexOf("?LOFF")>0)
          {
            digitalWrite(led,LOW);
          }
        }
      }
      readString="";
    }
  }
}

void process(EthernetClient client)
{
  // TEMPERATURE
  dht.temperature().getEvent(&event);
  client.println("</br>");
  client.println("TEMPERATURE:");
  if (isnan(event.temperature))
  {
    Serial.println("Error reading temperature!");
  }
  else 
  {
    Serial.print("TEMPERATURE: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
    client.println(event.temperature);
    client.println(" *C");
   }  
   //  HUMIDITY

  dht.humidity().getEvent(&event);
  client.println("</br>");
  client.print("RELATIVE HUMIDITY :");
  if (isnan(event.relative_humidity)) 
  {
    Serial.println("Error reading humidity!");
  }
  else 
  {
    Serial.print("RELATIVE HUMIDITY: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    client.print(event.relative_humidity);
    client.println("%");
  }
   //SOIL MOISTURE SENSOR

  sensorValue = analogRead(sensorPin);
  
  sensorValue1 = map(sensorValue,200,1023,100,0);
   
   Serial.print("SOIL MOISTURE:");
   Serial.print(sensorValue1);
   Serial.println("%");

   client.println("</br>");
   client.print("SOIL MOISTURE:");
   client.print(sensorValue1);
   client.println("%");
   
   //LDR SENSOR
  
  ldrValue = analogRead(ldrPin);
  double Vout = ldrValue*0.0048875;
  Light=(500*(5-Vout))/(10*Vout);
  Serial.print("LIGHT INTENSITY:");
  Serial.print(Light);
  Serial.println(" lux");

  client.println("</br>");
  client.print("LIGHT INTENSITY:");
  client.print(Light);
  client.println(" lux");

  phsense(client);
  Serial.println();
}

void phsense(EthernetClient client)
{
  for(int i=0;i<10;i++)       
  { 
     buf[i]=analogRead(pHsensorPin);
     delay(10);
  }
  for(int i=0;i<9;i++)        
  {
     for(int j=i+1;j<10;j++)
     {
        if(buf[i]>buf[j])
        {
          temp=buf[i];
          buf[i]=buf[j];
          buf[j]=temp;
        }
     }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      
     avgValue+=buf[i];
  float phVoltage=((float)avgValue*4.52/1024)/6; 
        
  phValue=3*phVoltage;                     
  Serial.print("SOIL pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  client.println("</br>");
  client.print("SOIL pH:");  
  client.print(phValue,2);
  client.println(" ");
}
