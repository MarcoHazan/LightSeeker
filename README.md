# LightSeeker
LightSeeker è una macchinina che una volta accesa cerca di muoversi nel posto più luminoso possibile. 
Prima di partire le viene assegnato un goal, ovvero di trovare un'intensità di luce che viene assegnata tra 0 e 1024 (unità di misura per la tensione di arduino).
In questo modo è possibile 'telecomandare' LightSeeker utilizzando per esempio una torcia e, nel momento che si trova al centro della luce proiettata da essa si ferma.
Il robottino in questione è costituito da una scatoletta a cui sono stati attaccati ai 4 bordi 4 motori DC, pilotati da un ponte H alimentato da una batteria a 12 V che sono posti all'interno della scatola.
Per orientarsi utilizza due braccia con installate sopra due fotoresistenze e una fotoresistenza centrale fissata al centro.

## Meccanismo

Per impostare il goal ho utilizzato un app chiamata iot On/off, che viene configurata per comunicare con il robot tramite protocollo MQTT. Con essa perciò si può impostare il limite di luce da raggiungere, oltre che impostare velocità e monitorare l'intensità di luce rilevata.
LightSeeker cambia direzione nel momento in cui una delle due fotoresistenze sulle braccia rileva un'intensità significantemente superiore della luce rilevata dalla fotoresistenza centrale. Continua quindi a girare nella direzione segnalata (destra o sinistra) fino a che la fotoresistenza centrale non rileva la stessa intensità, e a quel punto si stabilizza.




Componenti:
- ESP8266
- 4 motori DC (3-6 V)
- 2 fotoresistenze
- 1 batteria a 12V
- Ponte H l298N

## Foto

<img src="img/lightseeker.jpg" style="width:30%"> <img src="img/photo1.jpg" style="width:30%"> <img src="img/photo2.jpg" style="width:30%">
Le fotoresistenze sono state in qualche modo schermate, in modo da non captare la luce nelle direzioni non volute.

## Circuito

<img src = "lightseeker_circuit.jpg alt="circuito">
