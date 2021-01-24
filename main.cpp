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








int main()
{
    cout << "Hello world!" << endl;
    return 0;
}
