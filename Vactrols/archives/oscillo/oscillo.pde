/*
oscilloscope minimaliste avec un arduino
récupère les valeurs via une liaison serie
*/
import processing.serial.*; // importation de la librairie de communication serie
//variables
Serial maConnection; // Crée un objet de communication série
int tension=0; // variable ou sera stockée la valeur de la tension
//affichage
int fenetreTemps=12000; // temps pour parcourir la largeur de l'écran en millisecondes
int x=0;
int y=0;

void setup() {
    size(650, 400); // taille de l'ecran
    String NomDuPort = Serial.list()[0]; // récupère la première interface serie trouvée
    println("connection a "+NomDuPort);
    maConnection = new Serial(this, NomDuPort, 19200); // création de la connexion série
    background(255); // fond d'écran blanc
    smooth(); // lisser les dessins
    strokeWeight(2); // largeur de trait
    stroke(40); // couleur du trait gris
}

void draw() { //boucle de dessin principale
    //rien ne s'y passe, tout est géré dans la fonction serialEvent
}

void serialEvent (Serial maConnection) { // si des données arrivent par la connexion série
    String retour=maConnection.readStringUntil('\n'); // lit la donnée jusqu'à la fin de ligne
    if (retour != null) { //si le retour n'est pas vide
        retour = trim(retour); // enlever les espaces
        tension = int(retour); // converti le texte en nombre entier
    }
    //détermination du X actuel
    int oldx=x;
    x=(millis()%fenetreTemps)*width/fenetreTemps;
    if (oldx>x) {
        //reprise au debut de l'écran
        oldx=0;
        background(255); //fond d'écran blanc
    }
    //determination de la valeur de Y
    int oldy=y;
    y=int(map(tension, 0, 1023, 380, 20)); // mise à l'échelle de la tension pour entrer dans l'écran
    line(oldx, oldy, x, y); // dessine le trait
}

