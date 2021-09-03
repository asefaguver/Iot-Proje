#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define in D7     // giris ve cikis pinleri tanimlandi
#define out D2

#define FIREBASE_HOST "iot-proje-asg-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "EGWtHdE2doD189vIL9yMOzsJE6ZeBs1I6i64Gn2f"

char thingSpeakAddress[] = "api.thingspeak.com";
String thingtweetAPIKey = "LJCQDXRSFG06FK07";
String tsData;


/*  Kablosuz  Bağlantı  Bilgileri */
#define WLAN_SSID " wifi adresinizin adı "       
#define WLAN_PASSWORD " wifi şifreniz " 
        
/*  ThingSpeak  Kurulumu */
unsigned long channelID = 1255636;
unsigned  int field_no=1;
const char* writeAPIKey = "GMYVRRNGKKQ70N8F";     //  Thingspeak  write API Key
const char* readAPIKey  = "GHXNDNCZZKGQP8LJ";     //  Thingspeak  read  API Key
const char* server = "api.thingspeak.com";
WiFiClient  client;

int count=1;

void wifiSetup();
void setup()
{
  Serial.begin(9600);
  Serial.print("Visitor Counter");
  delay(2000);
  pinMode(in, INPUT);
  pinMode(out, INPUT);
  wifiSetup();                    //  Kablosuz  ağ  kurulum fonksiyonu
  ThingSpeak.begin(client);                 //  ThingSpeak client sınıfı  başlatılıyor
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Serial.println("Person In Room:");
  Serial.print(count);
}

/*  ESP8266 WiFi  Kurulum Fonksiyonu  */
void  wifiSetup(){
    delay (10);
    Serial.println(); Serial.println(WLAN_SSID);
        Serial.println(F("Kablosuz  Agina Baglaniyor"));
    WiFi.begin(WLAN_SSID, WLAN_PASSWORD);
    //  WiFi  durum kontrolü
    while (WiFi.status()  !=  WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
Serial.println();
Serial.print(WLAN_SSID);  Serial.println("Kablosuz  Aga Baglandi");
Serial.println("IP  adresi: "); Serial.println(WiFi.localIP());
}

void loop()
{  
  int in_value = digitalRead(in);
  int out_value = digitalRead(out);
  if(in_value == LOW)   // eğer giris kapisindan girerse +1 yazdir
  {
    count++;
    Serial.print("Person In Room:");
    Serial.println(count);
    delay(1000);
  }
  
  if(out_value == LOW)  // eger cikis kapisindan cikarsa -1 yazdir
  {
    count--;
    Serial.print("Person In Room:");
    Serial.println(count);
    delay(1000);
  }
        
      /*  ThingSpeak  Field Yazma İşlemi */
    ThingSpeak.writeField (channelID, field_no, count, writeAPIKey);             //  kisi sayisini thingspeak'e  gönder
      /*  FireBase */
    Firebase.setInt("Value", count);                                             // kişi sayisini firebase'e gönder
      /*  ThingTweet  */
       if (client.connect("api.thingspeak.com",80)) {                           // thingtweet ile tweet at
      tsData = "api_key="+thingtweetAPIKey+"&status="+count;
      client.print("POST /apps/thingtweet/1/statuses/update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(tsData.length());
      client.print("\n\n");
      client.print(tsData);
      Serial.println("Tweet Send");

    }
    delay(200);
  
}
