/*
materials

ESP8266,
LED (Red)
LED (blue)
jumper wires
Bread boar


connections to leds

esp8266    led1      led2
D5   -->     +         x
D6   -->     x         +

*/
// connect the board to your pc and select com port based on availability
// include all the librairies necessary to connect to esp8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
ESP8266WiFiMulti multifi;

// we are using ESP8266WiFiMulti which allows us to connect to multiple wifi and connect to strongest wifi available in the proximity 
const char *ssid1="Ayush",*ssid2="ssid2",*ssid3="ssid3";
const char *pass1="ayush2004",*pass2="password2",*pass3="password3";
// write your ssid and password to what you want to connect to

#define red D5
#define blue D6

// all the functtion sthat we would be creating
void handleRoot();
void notFound();
void set_connect();
void set_server();
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
      <a href='/MiEkUzHf1NiDbCzBj5DrFdFeEu2KjSnAxCj/DbMvWqYt4TfBeBeCy7RuUiLjJn5VlLwPyNg/FiPwIvWd2LjIgPlDl/blueon'><button class='btn'>BLUE ON</button></a>
      <a href='/HtUiPlWj3EsHlLkWs1NzGjNvUf6NpVnXnEb/NzYbQnSg2EdCuZzCv3CvWlNgNw4NeQaKsSv/HrFbPzPg2PrRmLsEb/blueoff'><button class='btn'>BLUE OFF</button></a>
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
                <strong>ERROR 404 : </strong>some error might have occured or recived incorrect server request

                <br>
                <br>
                please click this link to enter server again!
                <a href='/'> click me </a>
            </p>
        </div>
    </div>
</body>
</html>
)rawliteral";


void setup(){
  // setting up the led pins
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  // setting up the serial port 
  Serial.begin(115200);
  delay(1000);
  // keeping our module NODEMCU in Access Point mode
  WiFi.mode(WIFI_STA);
  Serial.println("\n\n\nserial monitor is now online !!\n\n\n");

  // Connect to WiFi network
  set_connect();
  // Start the server
  set_server();

}

void loop(){
  // handeling all the calls that are made to the device
  server.handleClient();
}

void set_connect(){
  // adding all the wifi ssid and passwords that we want to add to list of connectable wifi
  // syntax : multifi.addAP( ssid , password );
  multifi.addAP(ssid1,pass1);
  multifi.addAP(ssid2,pass2);
  multifi.addAP(ssid3,pass3);
  Serial.print("Connecting to strongest wifi...");
  // keep on searching untill we find a strong and stable wifi to connect to 
  // i.e untill the status of the wifi module is  "WL_CONNECTED"
  while(multifi.run() != WL_CONNECTED){
    Serial.print(".");
    delay(1000);
    // ESP.restart();
  }
  // please connect to this ip address. note this ip is dynamically generated . reffer automation 2 for a static generated ip
  Serial.print("\n\nconnected to :");
  // prints ssid currently connected ammong given ssid
  Serial.println(WiFi.SSID());
  Serial.print("\nIPadress :");
  // prints IP currently connected ammong given IP
  Serial.println(WiFi.localIP());
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
