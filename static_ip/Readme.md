**PLEASE NOTE**:
i have made this project in **Station Mode** of NodeMcu.
this is because of the underlying reasons.
- we can not generate the ip address without any constrain in AP mode
- if we use the AP mode for IP generation Gateway of your *Local Host Router* constarins the Gateway of your ESP8266
- if we have a constraind Gateway then we can not generate a free random IP
- rules would only be constraind to IP & Gateway generation listed in *Set your Gateway and IP address*

<br>
<br>
<br>

**Set your Gateway and IP address**
    NOTE: 
        if the ip address is of type :  192.a.b.c 
        gateway must be of type      :  192.1.1.1  
        also, mask must be of form   :  255.0.0.0  
please reffer this link : <> 
for more information on ip and gateway addressing    

**NOTE**: this ip is coustom alocated with WiFi.config() settings of the nodeMcu 8266 board

**syntax** : WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)

<br>
<br>
<br>

**CONNECTIONS**
  -------------------------------
 |    esp8266    led1      led2  |
  -------------------------------
 |   D5   -->     +         x    |
  -------------------------------
 |   D6   -->     x         +    |
  -------------------------------
