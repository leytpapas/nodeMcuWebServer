# nodeMcu WebServer

A simple web server with a simple switch for home automation using Wifi. The server also accepts json requests for minimum communication and quicker response.

### Software

* Arduino IDE with the appropriate setting to connect with nodemcu board
* ESP8266Wi-Fi library


### Hardware

* NodeMCU ESP8266  Board

![NodeMCU](https://github.com/leytpapas/nodeMcuWebServer/blob/master/images/nodemcu.jpg)

* 3V relay

![3V relay](https://github.com/leytpapas/nodeMcuWebServer/blob/master/images/relay.jpg)


* 5V AC/DC Converter Switch Power Supply Module


* Old box to fit setup for easy use + some rubber to put among the boards for isolation (isn't shown in the pictures). It gives power to AC/DC converter which powers the nodemcu board and the relay controls whether it will also give power to the device plugged in it

![outlet pic](https://github.com/leytpapas/nodeMcuWebServer/blob/master/images/priza1.jpg)

![outlet pic](https://github.com/leytpapas/nodeMcuWebServer/blob/master/images/priza3.jpg)

![outlet pic](https://github.com/leytpapas/nodeMcuWebServer/blob/master/images/priza2.jpg)

* Result: a way to control every device you want just by plugging it to the power using this setup.

![outlet pic](https://github.com/leytpapas/nodeMcuWebServer/blob/master/images/priza4.jpg)


Now, go make an army of these but dont forget to note each device's hostname.

![outlet pic]({{site.url}}{{site.baseurl}}images/priza4.jpg)

### How to use

* Enter your Wifi's ssid and password so that the webserver can be reached

```
const char* ssid = "******";

const char* password = "******";
```

* Enter the pin you connected the relay to

```
int relay = D4;
```

* After it's connected you can find it's IP address either using your preffered mapper or by searching your routers allocated addresses.
(an alternative option would be to connect it first at the computer and get its assigned IP using the Serial or even attach a LED screen)

* Go to your brower and hit that IP (make sure you are connected to the same network or have a way of communication with that network). BOOM you are ready to go. 


* Suggestion: I use [this](https://play.google.com/store/apps/details?id=ch.rmy.android.http_shortcuts) app which sets up widgets on the phone/tablet for everyday use. An example of shorcuts is on the httpRequestShortcuts folder, you can import them and then edit them. Just changing the IP address should be ok, but depending on the way the relay is connected 'on' and 'off' could be reverted with each other.
A drawback of this method is that each time the devices get a new IP address you also have to edit the shortcuts at the app. Even though I tried to reach the device using the assigned hostname it failed, but I believe there is something wrong with my router.
Of course, you can set a static IP for each device so that you don't have to deal with the above problem in every reset. In my case, the devices were reachable but they were missing from the routers tables so I reverted to the dynamic IP.

### Future Work 

* Make static IP work or more prefferable reach devices using hostname.

* Set a logic where you can connect via Wifi to the device and change SSID and password(instead of having them 'hardcoded'). Probably I have find a way to trigger a reboot of the device using code.



### Credits

Thanks to [spacehuhn](https://github.com/spacehuhn/) for the [tools](https://github.com/spacehuhn/esp8266_deauther/blob/master/web_server/) to convert files to PROGMEM









#####Any suggestions are welcome