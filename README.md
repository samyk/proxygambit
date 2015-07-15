# [ProxyGambit](http://samy.pl/proxygambit)

**ProxyGambit** is a simple anonymization device that allows you to access the Internet **from anywhere in the world** without revealing your true location or IP, fracturing your traffic from the Internet/IP through either a long distance radio link or a reverse tunneled GSM connection.

[![ProxyGambit Directional Antenna to Laptop](http://samy.pl/proxygambit/laptop.png)](http://samy.pl/proxygambit/laptopbig.png)

A high speed (150Mbps+) link is available with direct line of sight from 10km+ away, or if further away, a 2G GSM connection produces a **reverse TCP tunnel serializing a shell** into the device accessible from anywhere in the world via the Internet or GSM. Either method **proxies your connection through local wifi networks near the device**, shielding and making it more difficult to determine your true location, IP and identity.

ProxyGambit is an improvement and reincarnation of [ProxyHam](http://www.wired.com/2015/07/online-anonymity-project-proxyham-mysteriously-vanishes/), the promising device that was to debut at Defcon 2015, yet [vanished with no source, documentation or reason for its "destruction"](http://www.wired.com/2015/07/online-anonymity-project-proxyham-mysteriously-vanishes/). ProxyHam *"promised to mask your location online by putting you up to 2.5 miles away from your router"*

ProxyGambit improves upon ProxyHam not only by allowing a greater direct range, but also enables you to proxy from **anywhere in the world using just an Internet connection, which can then proxy through a reverse GSM serial link onto a public wifi network**.

Plug your SIM card into the device and from anywhere in the world you can drop a shell while being a layer removed, and access the Internet through a remote wifi network.

```
This is an insecure, bare bones proof of concept. The fragmentation of data through alternate mediums is a useful concept and those interested in privacy, anonymization, or deanonymization should explore this area further. Entropy is both gained and lost with these methods and many risks are involved when deploying any system of this nature.
```

by [@SamyKamkar](https://twitter.com/samykamkar) // <code@samy.pl> // <http://samy.pl> // Jul 14, 2015

Code available on [github](https://github.com/samyk/proxygambit)


------

# Hardware

[![ProxyGambit Hardware](http://samy.pl/proxygambit/all.png)](http://samy.pl/proxygambit/allbig.png)


### Raspberry Pi
**$35**: A [Raspberry Pi 2 Model B](http://amzn.to/1TCfkO1) (or cheaper [RPi Model B+](http://amzn.to/1f33DAL)) computer is a critical portion of the core and drives the Linux serial link while also bridging our public wifi to radio connection.

```
username: pi
password: donttracemebro
```

### Arduino
**$8**: Any ATmega328 device will suffice, such as my favorite [Arduino Nano](http://amzn.to/1f33WM2), which provides a software proxy layer between the serial UART of the Raspberry Pi and the FONA GSM board. A logic level converter or voltage divider is required to prevent imminent destruction of hardware which we perform with a few inexpensive resistors.

### FONA GSM Board
**$40**: The [Adafruit FONA GSM board](http://amzn.to/1SmM1N9) uses a SIM800 chip which allows us to connect an Arduino to GSM, providing us a 2G Internet link, and can additionally allow SMSs and voice calls to our friends to verify whether their refrigerators are running.

[![ProxyGambit Raspberry Pi with Arduino](http://samy.pl/proxygambit/IMG_3138sm.JPG)](http://samy.pl/proxygambit/IMG_3138.JPG)

### FONA Antenna
**$6**: An [antenna](http://amzn.to/1IZLDDt) is helfpul (read: necessary).

### Point-to-Point Wireless Links
**$98**: For low cost and low profile, I suggest either a **pair** of the [Ubiquiti NanoStation LocoM2s](http://amzn.to/1GlBsmn) (2.4GHz) which reach 5km+, or a **pair** of the [Ubiquiti NanoStation LocoM5s](http://amzn.to/1GlBqep) (5GHz) which reach 10km+, but are slightly more expensive at $62 or $124 for both. These devices are not interchangeable. Additionally, if you choose the LocoM5, you can actually use one of the devices to acquire Internet access at Burning Man.

### Wifi Card
**$10**: You can use anything such as an [Edimax](http://amzn.to/1f34zVV) adapter, or a more powerful [Alfa AWUS036NH](http://amzn.to/1SmMi2x) adapter. This all depends on how far you'd like your ProxyGambit device to reach. This is specifically to connect to the "exit node" of the proxy link where your Internet traffic will ultimately come out of and appear to be coming from.

### SIM Card
**$8**: You will need a [SIM card](http://amzn.to/1GlBz1w) with access to a 2G network. You can purchase prepaid SIM cards in cash with 2G through [T-Mobile](http://amzn.to/1GlBz1w).

### SD Card
**$6**: An [SD card](http://amzn.to/1f352XW) is required for the Raspberry Pi to host the operating system and data.

### Powered USB Hub
**$10**: I avoid two issues with Raspberry Pi -- the limited power output from USB, and the limited power input from most USB power supplies. I do this by using a [2.5A powered USB hub](http://amzn.to/1TCg0D3) in a virtual loop. The hub acts as the power supply for the Raspberry Pi, preventing any need for a power supply, while ironically I connect the source of the hub back into one of the Raspberry Pi USB slots. This may appear dangerous as a voltage or data loop, but the Pi's USB-voltage-in has data lines split off, and the USB hub prefers power from its voltage rectifier rather than the recirculated power lines.

[![Raspberry Pi USB Hub Power Loop](http://samy.pl/proxygambit/loopdesc.png)](http://samy.pl/proxygambit/loopdescbig.png)

### 3.7 Lithium-Ion (LiOn or LiPo) Battery
**$13**: The FONA GSM board annoyingly requires a 3.4-4.0V voltage source in addition to a 5V source, so I use a small [lipo battery](http://amzn.to/1TCg71t) for this. The FONA has a built-in charger so I tie the charger directly to the 5V from the Arduino, which actually bypasses the Arduino voltage regulator and instead grabs power from our high-current USB hub.

-----

# Software

All necessary software and Linux image is available from my github: <https://github.com/samyk/proxygambit>

### ProxyGambit (Arduino + GSM Link)
The microcontroller code produces a serial connection over a reverse TCP tunnel through a 2G GSM link and ties it to the UART serial lines of the Raspberry Pi. Compile this onto your Arduino, but be sure to change the TCP_PORT and TCP_HOST definitions in pg_gsm_avr.ino to connect to your own server. We abuse the fact that the SIM800 GSM chip allows only outbound TCP connections and produce a reverse tunnel. The hardware serial lines are hardwired (with a voltage divider) to the Raspberry Pi's serial, and we employ software serial to communicate to the FONA board.

### FONA
We use the Adafruit [FONA](https://github.com/adafruit/Adafruit_FONA_Library) library to produce an outbound TCP connection over the 2G GSM/GPRS link. This actually allows a reverse tunnel back in which we link to serial.

### Raspberry Pi Linux Image w/Network Bridging + UART
We use Raspberry Pi Wheezy (2015-05-05) image with network bridging via [bridge-utils](https://packages.debian.org/search?keywords=bridge-utils) and serial UART enabled to communicate with our microcontroller. Make sure to set up the wifi connection you want to use via `/etc/wpa_supplicant/wpa_supplicant.conf`, or you can serial in over GSM later on to connect to a network.

### netcat
From your own server (defined in pg_gsm_avr.ino), run:

`nc -vlp 1337 # listens verbosely on port 1337`


-----


[![ProxyGambit Arduino](http://samy.pl/proxygambit/IMG_3139sm.JPG)](http://samy.pl/proxygambit/IMG_3139.JPG)


# Schematic

Available from [github](https://github.com/samyk/proxygambit)


[![ProxyGambit Arduino](http://samy.pl/proxygambit/proxygambit_bb.png)](http://samy.pl/proxygambit/proxygambit_bbbig.png)



-----

# Contact

Contact on the twitters [@SamyKamkar](https://twitter.com/samykamkar)

More projects at <http://samy.pl> or sometimes reachable @ <code@samy.pl>

------