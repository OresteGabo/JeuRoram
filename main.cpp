#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int NMAXLIGS = 8;
const int NMAXCOLS = 8;
const int VMAX = 9;
const int MIRMAX = 4;
const int LIBRE = 0;
const int VMIROIR1 = -1;
const int VMIROIR2 = -2;
const int EVERTICAL = 1;
const int EHORIZ = 2;
const int NJOUEURS = 3;


typedef int Grille[NMAXLIGS][NMAXCOLS];
typedef int GrilleVisite[NMAXLIGS][NMAXCOLS];


/**
 * Contient les coordonnes d'une position sur la grille
 */
struct position {
    int x, y;
};

/**
 * Contient les scores des joueurs
 */
struct Scores {
    int nbJoueurs;
    int t[NJOUEURS];
};


/**
 * Contient la direction du robot
 */
struct direction {
    int diry, dirx;
};

/**
 * Contient le resultat d'un parcours du robot
 */
struct RParcours {
    int nbObjets;
    int score;
};

/**
 * Contient tous les parametres du jeu
 */
struct ParamsJeu {
    int nbLignes;
    int nbColonnes;
    int nbJoueurs;
    int nbObjets;
    int nbMiroirs;
    int bonus;
};

/**
 * Contient les donnees necessaires au jeu
 */
struct JeuRoRam {
    Grille GR;
    Scores score;
    int NbObjets;
};

/**
 * Pour la structure "choixIA"
 */
struct positionEtTypeMiroir {
    int x, y, typeMiroir;
};

/**
 * Contient le choix de l'IA du placement des miroirs
 */
struct choixIA {
    positionEtTypeMiroir tab[MIRMAX];
};







int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
