/*
Ce programme consiste simplement à mesurer l'entrée analogique 0, et à transmettre le résultat via une communication série.
Il ressemble beaucoup au programme décrit dans la première étape du projet "la petite bête qui a peur".
*/
//déclaration des variables
//stockage du numéro de Pin où sera branché l'entrée de la mesure 
int PinDeMesure=0;
//conteneur pour stocker la mesure
int tension=0;
// délai entre deux mesures
int periodeDeMesure=20;

void setup(){ // Initialisation du programme
    // établir la connection série à 19200 baud
    Serial.begin(19200);
    // définir le PinDeMesure comme une entrée
    pinMode(PinDeMesure, INPUT);
}

void loop() { //boucle principale
    tension = analogRead(PinDeMesure); // Lit la tension du pin DeMesure et stocke le résultat dans la variable tension
    Serial.println(tension); // communique la valeur de la tension au port série
    delay(periodeDeMesure); //attendre jusqu'à la prochaine mesure
}
