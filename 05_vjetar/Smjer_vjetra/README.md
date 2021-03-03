# Poznati problemi i moguća rješenja

* 4 smjera svijeta - 4 žičice - potrebno smanjiti na 1 pin

  * Svaki izlaz svakog senzora za magnet spojiti preko otpornika (promjena napona - svaki senzor ima svoj izlazni napon) na izlazni pin
  * funkcijom analogRead() očitamo vrijednost sa senzora i time odredimo položaj magneta
  * softverski odredimo precizniji položaj (sjevero-istok, jugo-zapad,...) - po potrebi
  
  <hr>
  
* Senzor uvijek na svojem izlazu ima logičku 1, što znači da kada magnet dođe u blizinu, stanje na izlaznom pinu je **0**
* Time prvo rješenje pada u vodu. - Ako su u isto vrijeme barem tri izlaza u stanju "1", onda taj napon poništava onu "0"

  * ~~Invertirati napon?~~
  * Tranzistor?
  * [Transistor NOT Gate](https://www.youtube.com/watch?v=sTu3LwpF6XI&t=481s)
