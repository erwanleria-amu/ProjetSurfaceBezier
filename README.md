# ProjetSurface3D
Projet Surface 3D | GUEGUEN Theodore x LERIA Erwan


PRESENTATION :
Durant ce projet, nous avons réalisé une discrétisation d'un carreau de Bézier.


COMPILATION : 
Pour pouvoir compiler ce projet, il est nécessaire d'avoir la librairie OpenGL d'installée.
Pour installer OpenGL via un terminal : 
sudo apt-get install freeglut3


STRUCTURE : 
Ce projet est divisé en différentes classes que nous allons vous présenter plus en détails :

Bernstein.cpp :
Cette classe contient la fonction qui permet de calculer le polynôme de Bernstein, ainsi que la fonction fact(int i) qui calcule la factorielle de i.

Discretisation.cpp :
Dans cette classe : 
- on créé les points de discrétisation grâce à paramCompute2
- on ajoute les points de discrétisation au VBO grâce à paramToVBO
- on gère également le mode d'affichage du carreau
- de plus, sont présentes quelques fonctions utiles telles que setStep qui change la précision de discrétisation ou set/getMode qui instancie/récupère le mode d'affichage

Main.cpp :
Rien de spécial dans cette classe, elle créé simplement notre mainWindow et l'affiche

MainWindow.cpp :
Dans cette, classe se trouvent toutes les fonctions permettant de gérer les actions de l'utilisateur.
C'est cette classe qui fait le lien entre l'UI et les données du programme.

myOpenGLWidget.cpp : 
Cette classe est la classe centrale du projet. 
En plus de gérer l'affichage openGL, c'est également dans cette classe que l'on créé les objets qui seront affichés grâce à la fonction makeGlObject.
C'est également dans cette classe que l'on stocke toutes les informations de notre carreau (taille, précision ...), ainsi que toutes les fonctions relatives au traitement de ces attributs. 
Dans cette classe, nous gérons également les entrées clavier de l'utilisateur.

Point.cpp :
Cette classe est simplement la classe objet "Point" qui nous permet de générer des points et d'effectuer des opérations sur ces derniers grâce aux fonctions operator=/*/+ entre autre.

Surface.cpp : 
Cette classe nous sert simplement à génerer les points de contrôle du carreaux de Bézier.

