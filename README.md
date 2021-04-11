# Metereološke postaje s Arduinom
<hr>

### Senzori
* [x] Temperatura
* [x] Vlaga
* [x] Tlak
* [x] PM2.5 
* [x] CO₂
* [ ] Svjetlina
* [ ] Padaline
* [ ] Brzina vjetra
* [ ] Smjer vjetra
* [ ] Buka

### Obrada podataka i prikaz
* [ ] Uno
* [x] Uno -> NodeMCUv3
* [x] NodeMCUv3 -> Server
* [ ] Server

### 3D
* [ ] kutija
* [ ] vjetar
* [ ] padaline

### Pisani dio
* [ ] Naslovna stranica
* [ ] *Sadržaj*
* [ ] Predgovor 
* [ ] Uvod
* [ ] Teorijski dio 
* [ ] Tehničko-tehnološki dio
* [ ] Zaključak

<hr>

<br>

# Mikroupravljači

### Arduino Uno

### NodeMCU v3 ([Wireless module CH340 NodeMcu V3 Lua WIFI Internet of Things development board](https://www.aliexpress.com/item/32665100123.html?spm=a2g0o.productlist.0.0.5cc463757vLbA1&algo_pvid=0f02beb2-816e-447d-b91c-873c435693a7&algo_expid=0f02beb2-816e-447d-b91c-873c435693a7-0&btsid=2100bdf016060538203684009e286e&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_))

   * File -> Preferences -> Additional Boards Manager URLs: -> http://arduino.esp8266.com/stable/package_esp8266com_index.json
   * Tools -> Board -> Boards Manager -> esp8266 -> Install

<hr>

<br>

## 1 Zrak - kvaliteta

* CO2 sensor module
    * [MH-Z19B NDIR](https://www.aliexpress.com/item/4000212024923.html?spm=a2g0o.productlist.0.0.1dce1cd82xflAd&algo_pvid=dbb7f88f-37f6-4a7a-8fe4-205ec7e7523e&algo_expid=dbb7f88f-37f6-4a7a-8fe4-205ec7e7523e-1&btsid=2100bb4a16053956526828498ee251&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_) 
    * [MH-Z19B NDIR](https://www.aliexpress.com/item/32946106807.html?spm=a2g0o.productlist.0.0.1dce1cd82xflAd&algo_pvid=dbb7f88f-37f6-4a7a-8fe4-205ec7e7523e&algo_expid=dbb7f88f-37f6-4a7a-8fe4-205ec7e7523e-0&btsid=2100bb4a16053956526828498ee251&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
* PM2.5 Air particle dust laser sensor
    * [PMS5003](https://www.aliexpress.com/item/4000036196650.html?spm=a2g0o.productlist.0.0.303d1399gzLv8L&algo_pvid=96b9cd68-2ddd-4dd2-ad1d-1446bea35961&algo_expid=96b9cd68-2ddd-4dd2-ad1d-1446bea35961-0&btsid=2100bde716053960750548351e3e4c&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
    * [PMS7003](https://www.aliexpress.com/item/4000036196650.html?spm=a2g0o.productlist.0.0.303d1399gzLv8L&algo_pvid=96b9cd68-2ddd-4dd2-ad1d-1446bea35961&algo_expid=96b9cd68-2ddd-4dd2-ad1d-1446bea35961-0&btsid=2100bde716053960750548351e3e4c&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
* Air Quality Sensor - Hazardous Gas Detection Module
    * [MQ-135](https://www.aliexpress.com/item/32547745710.html?spm=a2g0o.productlist.0.0.7e01139b8deBkY&algo_pvid=9d999360-cde6-4dab-8593-17b1290986d6&algo_expid=9d999360-cde6-4dab-8593-17b1290986d6-0&btsid=2100bddb16053962904015920ee386&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)


## 2 Atmosfera - temperatura, vlaga, tlak

* [BME280 3.3V](https://www.aliexpress.com/item/32849462236.html?spm=a2g0o.productlist.0.0.1d341b5b04CpQk&algo_pvid=fb1dc50a-e533-4327-a8c5-cff872b68bde&algo_expid=fb1dc50a-e533-4327-a8c5-cff872b68bde-0&btsid=2100bdcf16053967085595679ea4c6&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)  
    * [link 2](https://www.aliexpress.com/item/32853751253.html?spm=a2g0o.detail.1000023.1.5f5a11072frzgn)
* [BME680](https://www.aliexpress.com/item/4000049700826.html?spm=a2g0o.productlist.0.0.4c8f1d4bJWVAhb&algo_pvid=1edde55a-9656-4287-a64b-94db0f53ee37&algo_expid=1edde55a-9656-4287-a64b-94db0f53ee37-0&btsid=0b0a050116053968699261587ededa&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)


## 3 Senzor buke

* MAX4466
    * [link 1](https://www.aliexpress.com/item/32752086620.html?spm=a2g0o.productlist.0.0.6a41fef0zKo15A&algo_pvid=4dff4ff1-f5b9-4a2c-8d89-ac28afbf10c5&algo_expid=4dff4ff1-f5b9-4a2c-8d89-ac28afbf10c5-2&btsid=0b0a050116060529817701068e3002&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)
    * [link 2](https://www.aliexpress.com/item/32607495483.html?spm=a2g0o.productlist.0.0.6a41fef0zKo15A&algo_pvid=4dff4ff1-f5b9-4a2c-8d89-ac28afbf10c5&algo_expid=4dff4ff1-f5b9-4a2c-8d89-ac28afbf10c5-1&btsid=0b0a050116060529817701068e3002&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)


## 4 Padaline

*  [link](https://www.instructables.com/Arduino-Rain-Gauge-Calibration/)

## 5 Vjetar

* Brzina vjetra
* Smjer vjetra


## Vrijeme

* [DS3231](https://www.aliexpress.com/item/4000004876793.html?spm=a2g0o.productlist.0.0.5c7e47dd24vbVt&algo_pvid=22c23d4e-6681-44ab-86a7-5395f934ed4a&algo_expid=22c23d4e-6681-44ab-86a7-5395f934ed4a-0&btsid=0b0a555516181379326868154e82b9&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_) 
* [Link 1](https://create.arduino.cc/projecthub/MisterBotBreak/how-to-use-a-real-time-clock-module-ds3231-bc90fe)
* [Link 2](http://gilles.thebault.free.fr/spip.php?article53)
* [Link 3](https://lastminuteengineers.com/ds3231-rtc-arduino-tutorial/)


## Ostalo

* Žice
* Konektori
