
/*
materials

ESP8266,
LED (Red)
LED (blue)
jumper wires
Bread boar


connections to leds
  -------------------------------
 |    esp8266    led1      led2  |
  -------------------------------
 |   D5   -->     +         x    |
  -------------------------------
 |   D6   -->     x         +    |
  -------------------------------
*/
// connect the board to your pc and select com port based on availability
// include all the librairies necessary to connect to esp8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti multifi;
// we are using ESP8266WiFiMulti which allows us to connect to multiple wifi and connect to strongest wifi available in the proximity
const char *ssid = "ESP_IS_FUN";
const char *pass = "somethingSneaky";
// write your ssid and password to what you want to connect to

#define red D5
#define blue D6

// all the functtion sthat we would be creating
void handleRoot();
void notFound();
void set_connect();
void set_server();
void set_ota();
void red_on();
void red_off();
void blue_on();
void blue_off();
bool log_in = false;

ESP8266WebServer server(80);

/*

home page :
this is a get / call which is run whenever a button on the webpage is pressed
it takes us to the home page of the server taht is hosted on your local host ip

*/
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
 <head>
   <meta charset='UTF-8'>
   <meta http-equiv='X-UA-Compatible' content='IE=edge'>
   <meta name='viewport' content='width=device-width, initial-scale=1.0'>
   <style>
     * {
       box-sizing: border-box;
       padding: 0;
       margin: 0;
     }
     body {
       background-color: rgb(243, 187, 119);
       align-content: center;
       align-self: center;
       align-items: center;
       border-style: none;
     }
     .container {
       display: flex;
       justify-content: center;
     }
     .btn {
       background-color: rgb(255, 98, 98);
       align-content: center;
       align-self: center;
       align-items: center;
       border-style: none;
       width: 45vw;
       margin: 10px 10px 10px;
       font-family: Georgia, ' Times New Roman ', Times, serif;
       border-radius: 25px;
       font-size: 50px;
       padding: 5px 5px;
       color: azure;
       height: 10.5rem;
     }
     a {
       text-decoration: none;
       font-family: Georgia, ' Times New Roman ', Times, serif;
       border-radius: 25px;
       font-size: 40px;
       color: azure;
     }
     .btn:hover {
       transition: 0.5S;
       background-color: rgb(239, 70, 70);
     }
     nav {
       background-color: rgb(255, 136, 0);
       height: 60px;
       width: 100vw;
       display: flex;
       border-radius: 0px;
     }
     .nav-list {
       display: flex;
       list-style: none;
       padding-top: 15px;
       color: aliceblue;
     }
     .nav-items {
         font-size: 20px;
         justify-content: center;
         height: fit-content;
         padding-left: 50vw;
       }
     .nav-items1 {
       font-size: 30px;
       justify-content: center;
       height: fit-content;
     }
     @media (max-width: 600px) {
       .btn {
         background-color: rgb(255, 98, 98);
         align-content: center;
         align-self: center;
         align-items: center;
         border-style: none;
         width: 80vw;
         margin: 10px 10px 10px;
         height: 10.5rem;
         font-family: Georgia, ' Times New Roman ', Times, serif;
         border-radius: 25px;
         font-size: 50px;
         padding: 5px 5px;
         color: azure;
       }
       .container {
       display: flex;
       justify-content: center;
       justify-self: center;
       align-content: center;
       align-self: center;
       align-items: center;
       flex-direction: column;
       }
       .nav-list {
         display: flex;
         list-style: none;
         padding-top: 10px;
         color: aliceblue;
       }
       .nav-items {
         font-size: 20px;
         height: fit-content;
         padding-left: 15vh;
       }
       .nav-items1 {
         font-size: 30px;
         height: fit-content;
       }
       .fas{
         justify-items: left;
         padding-left: 20vw;
       }}
     i{
       justify-items: left;
       padding-right: 30px;
     }
   </style>
   <title>automation</title>
 </head>

 <body>
   <nav class='nav'>
     <ul class='nav-list'>
       <li class='nav-items1'>AUTOMAX</li>
       <li class='nav-items'>product ID : http://ESP8266.localhost/</li>
     </ul>
   </nav>
   <div class='container'>
     <a href='/OnSeEyTq8QiEkRlHe1JsIsOwIm9AuHuHfEc/SpEsOyUy5OmOtNtLe8NjMmNtBh1UvNrAyCd/FvMeYeYk1AtOgIyVi/redon'><button class='btn'>RED ON</button></a>
     <a href='/ZwMmTuMv6AiHfEsOy4JaZeIsGr5KwRaZmEf/ZwIwOmKx2MtNmDaKt8TzJrEiYl6CqVcGeZp/UfUnKoYj8JcIbQiPe/redoff'><button class='btn'>RED OFF</button></a>
     <br>
    </div>
    <div class='container'>
      <a href='/QnSeKkCj5GpMlDcQx5JwRgQuZp4FrChEbMf/PaHqZgLd8VbJwPwKo3LpQqXuJz8AzXhLuLz/EsIaFoIh5CnMcXiTp/blueon'><button class='btn'>BLUE ON</button></a>
      <a href='/XtVfQvGm2VyPdVdRh9SwMrCbKl6VsXtJxDm/LjGcSiJr6ZgPvWjTa3FuJpAgGp4UtUoHyOw/ZqSxAwFq5YtEsKvIt/blueoff'><button class='btn'>BLUE OFF</button></a>
      <br>
    </div>
   <div class='container'>
     <a href='/QnSeKkCj5GpMlDcQx5JwRgQuZp4FrChEbMf/PaHqZgLd8VbJwPwKo3LpQqXuJz8AzXhLuLz/EsIaFoIh5CnMcXiTp/allon'><button class='btn'>ALL ON</button></a>
     <a href='/XtVfQvGm2VyPdVdRh9SwMrCbKl6VsXtJxDm/LjGcSiJr6ZgPvWjTa3FuJpAgGp4UtUoHyOw/ZqSxAwFq5YtEsKvIt/alloff'><button class='btn'>ALL OFF</button></a>
     <br>
   </div>
 </body>
 </html>
)rawliteral";

/*

not found page :
this is a GET/ call which is run whenever a any random web address is called
it takes us to the not_found page of the server that is hosted on your local host ip

*/

const char error_Window[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
  <html lang='en'>
  <head>
    <meta charset='UTF-8'>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>ERROR MESSAGE</title>
    <style>
        .container{
            height: 100vh;
            width: 100vw;
            display: flex;
            justify-content: center;
        }
        .inner_container{
            display:flex;
            width: 95vw;
            height: 15vw;
            border-radius:20px ;
            border: dashed rgba(232, 73, 73, 0.722);
            background-color: rgba(250, 56, 22, 0.296);
            justify-content: center;
            align-items: center;
        }
        .error{
            text-align: center;
            color: crimson;
        }
        a{
            text-align: center;
            color: crimson;
            padding-left: 10px;
            text-decoration: none;
        }
        a:hover{
            text-decoration: underline;
        }
                @media only screen and (min-height: 700px) {
            /* For tablets: */
            .col-s-1 {height: 8.33%;}
            .col-s-2 {height: 16.66%;}
            .col-s-3 {height: 25%;}
            .col-s-4 {height: 33.33%;}
            .col-s-5 {height: 41.66%;}
            .col-s-6 {height: 50%;}
            .col-s-7 {height: 58.33%;}
            .col-s-8 {width: 66.66%;}
            .col-s-9 {height: 75%;}
            .col-s-10 {height: 83.33%;}
            .col-s-11 {height: 91.66%;}
            .col-s-12 {height: 100%;}
        }
        @media only screen and (min-height: 900px) {
            /* For tablets: */
            .col-1 {height: 8.33%;}
            .col-2 {height: 16.66%;}
            .col-3 {height: 25%;}
            .col-4 {height: 33.33%;}
            .col-5 {height: 41.66%;}
            .col-6 {height: 50%;}
            .col-7 {height: 58.33%;}
            .col-8 {width: 66.66%;}
            .col-9 {height: 75%;}
            .col-10 {height: 83.33%;}
            .col-11 {height: 91.66%;}
            .col-12 {height: 100%;}
        }
    </style>
</head>
<body>
    <div class='container'>
        <div class='inner_container col-s-2 col-1'>
            <p class='error'>
                <strong>ERROR 404 : </strong>some error have occured , Please authorize yourself again to continue
                <br>
                <br>
                please click this link to login and try again!
                <a href='/'> click me </a>
            </p>
        </div>
    </div>
</body>
</html>
)rawliteral";

void setup()
{
    pinMode(red, OUTPUT);
    pinMode(blue, OUTPUT);

    Serial.begin(115200);
    WiFi.softAP(ssid, pass);
    Serial.println("\n\n\nserial monitor is now online !!\n\n\n");
    /*
    printing current ip and ssid
    NOTE: this ip is dynamically alocated with factory settings of the nodeMcu 8266 board
    */ 
    Serial.print("\n\nconnected to :");
    Serial.println(WiFi.SSID());
    Serial.print("IPadress :");
    Serial.println(WiFi.localIP());
    // setting custom ip address

    /*
    Set your Gateway and IP address
        NOTE: 
            if the ip address is of type :  192.a.b.c 
            gateway must be of type      :  192.1.1.1  
            also, mask must be of form   :  255.0.0.0  
        reffer this <> for more information on ip and gateway addressing
    */
    IPAddress local_IP(192, 168, 10, 184);  //set custom ipaddress
    IPAddress gateway(192, 168, 1, 1);      //set custom gateway
    IPAddress subnet(255, 255, 0, 0);       
    IPAddress primaryDNS(8, 8, 8, 8);       // optional
    IPAddress secondaryDNS(8, 8, 4, 4);     // optional

    /*
       NOTE: this ip is coustom alocated with WiFi.config() settings of the nodeMcu 8266 board
       syntax : WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)
    */

    delay(1000);
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("STA Failed to configure");
    }

    // printing new ip and ssid
    Serial.print("\n\nconnected to :");
    Serial.println(WiFi.SSID());
    Serial.print("IPadress :");
    Serial.println(WiFi.localIP());

    // Start the OTA
    set_ota();
    // Start the server
    set_server();
}

void loop()
{
    ArduinoOTA.handle();
    server.handleClient();
}

void set_ota()
{
    ArduinoOTA.setHostname("ESP8266_SERVER_1");
    ArduinoOTA.setPassword("admin");
    ArduinoOTA.onStart([]()
                       {
  String type;
  delay(5000);
  if(ArduinoOTA.getCommand()==U_FLASH){
    type = "sketch";
  } else { // U_FS
    type = "filesystem";
  }
  Serial.println("Starting to update" + type); });
    ArduinoOTA.onEnd([]()
                     { Serial.print("\n\n Upload Ended !!"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          {
  if ((progress / (total / 100))%10==0)Serial.printf("\nProgress %u%%\r", (progress / (total / 100))); });
    ArduinoOTA.onError([](ota_error_t error)
                       {
  Serial.printf("Error[%u]: ",error);
  if(error == OTA_AUTH_ERROR){
    Serial.println("Auth Failed");
  } 
  else if(error == OTA_BEGIN_ERROR){
    Serial.println("Begin Failed");
  } 
  else if(error == OTA_BEGIN_ERROR){
    Serial.println("Begin Failed");
  }
  else if(error == OTA_CONNECT_ERROR){
    Serial.println("Connect Failed");
  } 
  else if(error == OTA_RECEIVE_ERROR){
    Serial.println("Receive Failed");
  } 
  else if(error == OTA_END_ERROR){
    Serial.println("End Failed");
  } });
    ArduinoOTA.begin();
}

void set_server(){

  // creating all the instances for all possible server call and declaring corrosponding actions
  // syntax : server.on( url_request , function_call);
  
  server.on("/",handleRoot);
  
  server.on("/OnSeEyTq8QiEkRlHe1JsIsOwIm9AuHuHfEc/SpEsOyUy5OmOtNtLe8NjMmNtBh1UvNrAyCd/FvMeYeYk1AtOgIyVi/redon", red_on );

  server.on("/ZwMmTuMv6AiHfEsOy4JaZeIsGr5KwRaZmEf/ZwIwOmKx2MtNmDaKt8TzJrEiYl6CqVcGeZp/UfUnKoYj8JcIbQiPe/redoff", red_off );

  server.on("/QnSeKkCj5GpMlDcQx5JwRgQuZp4FrChEbMf/PaHqZgLd8VbJwPwKo3LpQqXuJz8AzXhLuLz/EsIaFoIh5CnMcXiTp/allon", all_on );

  server.on("/XtVfQvGm2VyPdVdRh9SwMrCbKl6VsXtJxDm/LjGcSiJr6ZgPvWjTa3FuJpAgGp4UtUoHyOw/ZqSxAwFq5YtEsKvIt/alloff", all_off );
  
  server.on("/MiEkUzHf1NiDbCzBj5DrFdFeEu2KjSnAxCj/DbMvWqYt4TfBeBeCy7RuUiLjJn5VlLwPyNg/FiPwIvWd2LjIgPlDl/blueon", blue_on);

  server.on("/HtUiPlWj3EsHlLkWs1NzGjNvUf6NpVnXnEb/NzYbQnSg2EdCuZzCv3CvWlNgNw4NeQaKsSv/HrFbPzPg2PrRmLsEb/blueoff", blue_off);

  server.onNotFound(notFound);
  
  server.begin();
  
  Serial.println("\n\n\nserver started!!\n\n\n");
}

void notFound(){
  // if we recive any url that is not mention as a server call in set server function we will call this not found page 
  server.send(200, "text/html",error_Window);
}

void all_on(){
  // this server call function is to connect turn on all lights
  Serial.println("all on called");
  digitalWrite(red, HIGH);
  digitalWrite(blue, HIGH);
  server.sendHeader("Location","/");
  server.send(303);

}

void all_off(){
  // this server call function is to connect turn on off all lights
  Serial.println("all off called");
  digitalWrite(red, LOW);
  digitalWrite(blue, LOW);
  server.sendHeader("Location","/");
  server.send(303); 

}

void red_on(){
  // this server call function is to connect turn on red light
  Serial.println("light on called");
  digitalWrite(red, HIGH);
  server.sendHeader("Location","/");
  server.send(303); 

}

void red_off(){
  // this server call function is to connect turn off red light
  Serial.println("light off called");
  digitalWrite(red, LOW); 
  server.sendHeader("Location","/");
  server.send(303);
}

void blue_on(){
  // this server call function is to connect turn on blue light
  Serial.println("lamp on called");
  digitalWrite(blue, HIGH);
  Serial.println("pin now high");
  server.sendHeader("Location","/");
  server.send(303); 

}

void blue_off(){
  // this server call function is to connect turn off blue light
  Serial.println("lamp off called");
  digitalWrite(blue, LOW);
  Serial.println("pin now low");
  server.sendHeader("Location","/");
  server.send(303); 

}

void handleRoot() { 
  // When URL / is requested, send a web page with a call root page request
  server.send(200, "text/html", index_html);
}
