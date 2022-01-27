# LightSeeker
LightSeeker è una macchinina che una volta accesa cerca di muoversi nel posto più luminoso possibile. 
Prima di partire le viene assegnato un goal, ovvero di trovare un'intensità di luce che viene assegnata tra 0 e 1024 (unità di misura per la tensione di arduino).
In questo modo è possibile 'telecomandare' LightSeeker utilizzando per esempio una torcia e, nel momento che si trova al centro della luce proiettata da essa si ferma.
Il robottino in questione è costituito da una scatoletta a cui sono stati attaccati ai 4 bordi 4 motori DC, pilotati da un ponte H alimentato da una batteria a 12 V che sono posti all'interno della scatola.
Per orientarsi utilizza due braccia con installate sopra due fotoresistenze e una fotoresistenza centrale fissata al centro.

## Meccanismo

Per impostare il goal ho utilizzato un app chiamata iot On/off, che viene configurata per comunicare con il robot tramite protocollo MQTT. Con essa perciò si può impostare il limite di luce da raggiungere, oltre che impostare velocità e monitorare l'intensità di luce rilevata.
LightSeeker cambia direzione nel momento in cui una delle due fotoresistenze sulle braccia rileva un'intensità significantemente superiore della luce rilevata dalla fotoresistenza centrale. Continua quindi a girare nella direzione segnalata (destra o sinistra) fino a che la fotoresistenza centrale non rileva la stessa intensità, e a quel punto si stabilizza.




##Componenti
- ESP8266
- 4 motori DC (3-6 V)
- 2 fotoresistenze
- 1 batteria a 12V
- Ponte H l298N

## Foto

<img src="img/lightseeker.jpg" style="width:30%"> <img src="img/photo1.jpg" style="width:30%"> <img src="img/photo2.jpg" style="width:30%"><br>
Le fotoresistenze sono state in qualche modo schermate, in modo da non captare la luce nelle direzioni non volute.

## Circuito

<img align="left" src = "lightseeker_circuit.jpg" alt="circuito" style="width:40%"> 
Poichè l'esp8266 ha solo un input analogico, per leggere una fotoresistenza bisogna prima mettere in modalità input i pin a cui sono attaccate le altre due fotoresistenze (oltre a mettere in modalità output e HIGH il pin associato alla fotoresistenza che si vuole leggere).
La batteria di 12 Volt viene adottata, sia perchè provvede più corrente a lungo termine, sia perchè alimentando con 12 volt il ponte h, esso provvede una carica di 5 volt adatta ad alimentare la board esp8266.
Inoltre la tensione sui motori, viene regolata tramite PWM e limitata in modo da non suvvoltare i motori.<br><br><br><br><br><br><br><br><br><br><br>


### Fotoresistenze 

<img align="left"  src = "img/foto_circuit.gif" alt="circuito fotoresistenza" style="width:40%"> 
Il componente elettronico che per semplicità viene chiamato fotoresistenza, è un circuito stampato che di fatto non è altro che un partitore di tensione tra la fotoresistenza stessa e un resistore di 10k Ohm. Se quindi vengono scambiati nel circuito polo positivo con il polo negativo, l'effetto è, che quando la fotoresistenza riceverà più luce, la tensione in uscita tra le due resistenze sarà maggiore, che torna utile per la logica del progetto.<br><br><br><br><br><br><br><br><br><br><br><br>

## Logica software

Nel momento in cui viene impostato un tetto di luce lightseeker svolge due task principali che lo consentono di seguire la luce:
- Choose Direction: il task che si occupa rilevare l'intensità di luce e in base a ciò decidere se girare a destra, girare a sinistra oppure fermarsi
- Stabilize: il task che si occupa di stabilizzare la macchina nel caso che stia girando.

Ognuno di questi task viene riprodotto ogni mille secondi.

Quindi nel momento in cui lightseeker decide di girare, viene registrato un valore True su una variabile booleana che indica che stia effettivamente girando e il valore numeruco indicante la tensione del circuito della fotoresistenza della direzione nella quale si è deciso di girare. Se quindi sta girando e la fotoresistenza centrale rileva un'intensità luminosa pari al valore registrato si stabilizzerà.

Un altro task (publisheffect) provvede a mandare al server mqtt (per esempio test.mosquitto.org) con argomento lightseeker/light, la luce rilevata dalla fotoresistenza centrale, in modo da poter osservare la distanza dal goal finale.

## Impostazione iniziale e taratura

La prima cosa che fa il lightseeker appena acceso è connettersi al server mqtt indicato, tramite wifi.
Il lightseeker in questo momento dovrà trovarsi con la fotoresistenza centrale di fronte alla fonte di luminosa più intensa. A questo punto inizia la taratura, ovvero il robot gira intorno a se stesso una volta registrando i valori massimi delle due fotoresistenze.  
A questo punto viene calcolata l'imprecisione, ovvero di quanto dista il valore massimo delle due fotoresistenze con quello rilevato inizialmente dalla fotoresistenza centrale.
A questo punto dall'app iot on/off si dovrà impostare il goal (intensità di luce da inseguire) e la velocità del robot. 

