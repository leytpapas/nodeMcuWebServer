# [nodeMcu WebServer](https://leytpapas.github.io/nodeMcuWebServer/)

A web server with a switch for home automation using Wifi.

### Software

* Arduino IDE with the appropriate setting to connect with nodemcu board
* ESP8266Wi-Fi library


### Hardware

* NodeMCU ESP8266  Board

![NodeMCU](images/nodemcu.jpg)

* 3V relay

![3V relay](images/relay.jpg)


* 5V AC/DC Converter Switch Power Supply Module


* Old box to fit setup for easy use + some rubber to put among the boards for isolation (isn't shown in the pictures). It gives power to AC/DC converter which powers the nodemcu board and the relay controls whether it will also give power to the device plugged in it

![outlet pic](images/priza1.jpg)

![outlet pic](images/priza3.jpg)

![outlet pic](images/priza2.jpg)

* Result: a way to control every device you want just by plugging it to the power using this setup.

![outlet pic](images/priza4.jpg)


Now, go make an army of these but dont forget to note each device's hostname.


### How to use 

* Note the IP Address of the webserver when it also serves as an 'access point'.

```
IPAddress  apIP(192,168,3,4);
```

* Enter the pin you connected the relay to.

```
int relay = D4;
```

* Plug it in and upload the code

* Connect to the access point(nodemcu) you configured and add the ssid and password of the network you want it to connect to. You may have to restart it by unplugging the power from the board.

* After it's connected you can find it's IP address either using your preffered network mapper or by searching your routers allocated addresses.
(an alternative option would be to connect it first at the computer and get its assigned IP using the Serial or even attach a LED screen)

* Go to your brower and hit that IP (make sure you are connected to the same network or have a way of communication with that network). BOOM you are ready to go. 


### Suggestions

  * You can bind the nodemcu's mac address to a specific IP from inside the router. With this setting router will allocate this IP for this specific this device so it gets the same IP everytime it's connected.

  * Portforwarding would let you have access to your setup from anywhere in the world. Just make sure you have a static IP address on your home network or have another way of knowing its IP address(etc. dydns or send it to yourself via mail or your favorite messenger everytime it changes)

  * I use [this](https://play.google.com/store/apps/details?id=ch.rmy.android.http_shortcuts) app which sets up widgets on the phone/tablet for everyday use. Check 'json_structure.json' to figure out the communication protocol

### Future Work 

- [x] Make static IP work or more prefferable reach devices using hostname.

	Uncomment the following line to make nodemcu board listen to this IP. Hostname is enabled by default.
```
	//#define STATICIP
```

- [x] Set a logic where you can connect via Wifi to the device and change SSID and password(instead of having them 'hardcoded'). Probably I have find a way to trigger a reboot of the device using code. (***needs update***)

![logic diagram.png](images/logic_diagram.png)

- [ ] Save more than one network.

##### Any suggestions are welcome #####
