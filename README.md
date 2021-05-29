# LightSeeker
LightSeeker è una macchinina che una volta accesa cerca di muoversi nel posto più luminoso possibile.
Quando decide che secondo lei è il posto più luminoso di tutti si ferma.
In questo modo è possibile 'telecomandare' LightSeeker utilizzando per esempio una torcia oppure fargli percorrere delle piste di luce.
Per orientarsi utilizza due braccia con installate sopra due fotoresistenze e per evitare gli ostacoli utilizza un sensore ad ultrasuoni.
Per trovare la luce va anche a tentativi con la possibilità di aver trovato la strada giusta, oppure dover tornare indietro.
E' in programma anche quello di comunicare con LightSeeker via MQTT e impostargli un 'goal', ovvero una percentuale di crescita di luce richiesta,  e un timer.
Fino a che non verifica il goal continua a cercare fino a che scatta il timer impostato.



Componenti:
ESP8266
4 motori DC (3-6 V)
2 fotoresistenze
1 sensore a ultra suoni (HC-SR04)
1 batteria a 12V

