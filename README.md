# ProjetSurface3D
Projet Surface 3D | GUEGUEN Theodore x LERIA Erwan


PRESENTATION :
Durant ce projet nous avons réalisé une discrétisation d'un carreau de bezier.


COMPILATION : 
Pour pouvoir compiler ce projet il est nessecere d'avoir la librairie OpenGL d'installé
Pour installer OpenGL, via un terminal : 
sudo apt-get install freeglut3


STRUCTURE : 
Ce projet est divisé en differentes classes que nous allons vous présenter plus 
en détail :

Bernstein.cpp :
Cette classe contient la fonction qui permet de calculer le polynome de bernstein ainsi que la fonction fact(int i) qui calcul la factorielle de i

Discretisation.cpp :
Dans cette classe : 
- on créé les points de discrétisation grace a paramCompute2
- on ajoute les points de discrétisation au VBO grace a paramToVBO
- on gère egalement le mode d'affichage du carreau
- de plus sont présentes quelques fonctions utiles tel que setStep qui change la précision de discrétisation ou set/getMode qui instancie/récupère le mode d'affichage

Main.cpp :
Rien de spécial dans cette classe, elle créé simplement notre mainWindow et l'affiche

MainWindow.cpp :
Dans cette classe se trouve toutes les fonction permettant de gérer les actions de l'utilisateur
C'est cette classe qui fait le lien entre l'ui et les données du programmes

myOpenGLWidget.cpp : 
Cette classe est la classe centrale du projet. 
En plus de gérer l'affichage openGL cest egalement dans cette classe que l'on créé les objet qui seron affiché grace a la fonction makeGlObject
C'est egalement dans cette classe que l'on stock toutes les informations de notre carreau (taille, precision ...) ainsi que toutes les fonctions relatives au traitement de ces attributs 
Dans cette classe nous gérons egalement les inputs claviers de l'utilisateur

Point.cpp :
Cette classe est simplement la classe objet Point qui nous permet de générer des points et effectuer des opérations sur ces derniers grace aux foncitons operator=/*/+ entre autre

Surface.cpp : 
Cette classe nous sert simplement a génerer les points de controles du carreaux de bezier 

