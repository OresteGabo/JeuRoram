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
/**
 * Associe a' chaque entier un char
 * @param[in] a - entier
 * @return le caractere correspondant a' l'entier
 */
char symboleCase(int a) {

    if (a == LIBRE) return '.';
    if (a == VMIROIR1) return '/';
    if (a == VMIROIR2) return '\\';
    return 48 + a;
}

/**
 * Test si La dimension des Lignes est valide (entre 4 et 8)
 * @param[in] YMAX - Dimension lignes
 * @return Vrai si la dimension est valide, faux sinon
 */
bool estValideDimensionLignes(int YMAX) {

    if (YMAX > NMAXLIGS || YMAX < 4) return false;
    return true;
}

/**
 * Test si La dimension des Colonnes est valide (entre 4 et 8)
 * @param[in] XMAX - Dimension colonnes
 * @return Vrai si la dimension est valide, faux sinon
 */
bool estValideDimensionColonnes(int XMAX) {

    if (XMAX > NMAXCOLS || XMAX < 4) return false;
    return true;
}

/**
 * Test si le nombre d'objet a' placer est valide (entre 4 et 8)
 * @param[in] nbObjets - nombre d'objets a' placer
 * @param[in] MAXOBJETS - Nombre d'objets a placer au maximum
 * @return Vrai si nbObjets est entre 2 et MAXOBJETS  ,faux sinon
 */
bool nbObjetValide(int nbObjets, int MAXOBJETS) {

    if (nbObjets > MAXOBJETS || nbObjets < 2) return false;
    return true;
}

/**
 * Test si le nombre de joueurs est valide (entre 2 et NJOUEURS)
 * @param[in] nbJoueurs - nombre de joueurs
 * @return Vrai si le nombre de joueurs est compris entre 2 et NJOUEURS, faux sinon
 */
bool estValideNbJoueurs(int nbJoueurs) {

    if (nbJoueurs > NJOUEURS || nbJoueurs < 2) return false;
    return true;
}

/**
 * Test si le nombre de miroirs est valide (entre 1 et MIRMAX)
 * @param[in] nbMiroirs - nombre de miroirs
 * @return Vrai si le nombre de miroirs est compris entre 1 et MIRMAX, faux sinon
 */
bool estValideNbMiroirs(int nbMiroirs) {

    if (nbMiroirs > MIRMAX || nbMiroirs < 1) return false;
    return true;
}

/**
 * Test si une position est entre XMAX et YMAX
 * @param[in] p - position
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 * @return Vrai si la position est dans la grille, faux sinon
 */
bool estValide(position p, int XMAX, int YMAX) {

    if (p.x > XMAX || p.x < 0 || p.y > YMAX || p.y < 0) return false;
    return true;
}

/**
 * Test si une position est libre
 * @param[in] p - position
 * @param[in] gr - Grille
 * @return Vrai si la position est libre, faux sinon
 */
bool estLibre(position p, const Grille &gr) {

    if (gr[p.y][p.x] == 0) return true;
    return false;
}

/**
 * genere une chiffre aleatoire
 * @param[in] MIN - Valeur minimale
 * @param[in] MAX - Valeur maximale
 * @return une chiffre entre MIN et Max
 */
int aleatoire(int MIN, int MAX) {

    return MIN + rand() % (MAX - MIN + 1);
}

/**
 * Pose un objet dans la grille
 * @param[in] gr - Grille
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 */
void poserObjet(Grille &gr, int XMAX , int YMAX) {

    position p;

    do {
        p.x=aleatoire(0, XMAX);
        p.y=aleatoire(0, YMAX);

    } while (!estLibre(p, gr));

    gr[p.y][p.x] = aleatoire(1, VMAX);
}


/**
 * Pose nb objets dans la grille
 * @param[in] nb - Nombre d'objets
 * @param[in] G - Grille
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 */
void poserObjetsGrille(int nb, Grille &G, int XMAX, int YMAX) {

    for (int i = 0; i < nb; i++)
        poserObjet(G, XMAX, YMAX);
}

/**
 * Afficher une grille
 * @param[in] G - Grille
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 */
void afficherGrille(const Grille &G, int XMAX, int YMAX) {

    cout << ". ";
    for (int i = 1; i <= XMAX+1; i++)
        cout << i << " ";
    cout << endl;

    for (int i = 0; i <= YMAX; i++) {

        cout << i + 1 << "|";
        for (int j = 0; j <= XMAX; j++)
            cout << symboleCase(G[i][j]) << " ";
        cout << endl;
    }
}

/**
 * Initiale une grille avec toute ses positions libres
 * @param[in] gr - Grille
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 */
void initialiserGrille(Grille &gr, int XMAX, int YMAX) {

    for (int i = 0; i <= YMAX; i++)
        for (int j = 0; j <= XMAX; j++)
            gr[i][j]=0;
}

/**
 * Met a' jour une grille en remplacant la case libre par une nouvelle valeur
 * @param[in,out] gr - Grille
 * @param[in] p - position
 * @param[in] val - valeur
 */
void majGrille(Grille &gr, position p, int val) {

    gr[p.y][p.x] = val;
}

///////////////////////////// Partie Miroirs : ////////////////////////////

/**
 * Test si une position est jouable
 * @param[in] p - position
 * @param[in] gr - Grille
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 * @return Vrai si la position est libre et valide, faux sinon
 */
bool estJouable(position p, const Grille& gr, int XMAX, int YMAX) {

    if (estValide(p,XMAX,YMAX) && estLibre(p,gr)) return true;
    return false;
}

/**
 * saisie la position d'un miroir
 * @param[in] gr - Grille
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 * @return la position entree par l'utilisateur du miroir
 */
position saisirPosMiroir(Grille &gr, int XMAX, int YMAX) {

    position p;

    cin >> p.x >> p.y;
    p.x--;
    p.y--;

    while (!estJouable(p,gr,XMAX,YMAX)) {
        cout << "ERREUR: Position non jouable, saisir position miroir x y : ";
        cin >> p.x >> p.y;
        p.x--;
        p.y--;
    }
    return p;
}

/**
 * fait saisir a' l'utilisateur le type d'un miroir ( / ou \ )
 * @return un entier negatif correspondant au miroir ( -1 pour / et -2 pour \ )
 */
int saisieTypeMiroir() {

    char miroir;
    cout << "Saisir type de miroir : ";
    cin >> miroir;

    if (miroir == '/') return -1;
    return -2;
}

/**
 * pose nbMiroirs Miroirs dans la grille de jeu
 * @param[in] gr - Grille
 * @param[in] nbMiroirs - nombre de miroirs a poser
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 */
void poserMiroirs(Grille &gr, int nbMiroirs,int XMAX, int YMAX) {

    position p;
    int miroir;

    for (int i = 0; i < nbMiroirs; i++) {

        miroir = saisieTypeMiroir();

        cout << "Saisir position miroir x y : ";
        p=saisirPosMiroir(gr, XMAX, YMAX);

        gr[p.y][p.x] = miroir;
        afficherGrille(gr, XMAX, YMAX);
    }
}

/**
 * test si dans une position p il y a un miroir
 * @param[in] gr - Grille
 * @param[in] p - position
 * @return Vrai si dans la position p il y a un miroir, faux sinon
 */
bool estMiroir(const Grille &gr, position p) {

    if (gr[p.y][p.x] == -1 || gr[p.y][p.x] == -2) return true;
    return false;
}

/**
 * Efface tous le miroirs dans une grille de jeu
 * @param[in] gr - Grille
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 */
void effacerMiroirs(Grille &gr, int XMAX, int YMAX) {

    position p;

    for (int i = 0; i <= YMAX; i++) {
        p.y = i;
        for (int j = 0; j <= XMAX; j++) {
            p.x = j;
            if (estMiroir(gr,p)) gr[i][j] = 0;
        }
    }
}

// Partie Grille parcours

/**
 * Afficher une grille rappresentant le parcours du robot
 * @param[in] G - Grille de jeu
 * @param[in] V - Grille de parcours
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 */
void afficherGrilleParcours(const Grille &G, const GrilleVisite &V, int XMAX, int YMAX) {

    cout << endl << "GRILLE PARCOURS" << endl << ". ";

    for (int i = 1; i <= XMAX+1; i++)
        cout << i << " ";

    cout << endl;

    for (int i = 0; i <= YMAX; i++) {

        cout << i + 1 << "|";

        for (int j = 0; j <= XMAX; j++)
            switch (V[i][j])
        {
            case EVERTICAL:
                cout << "| ";
                break;
            case EHORIZ:
                cout << "- ";
                break;
            default:
                cout << symboleCase(G[i][j]) << " ";
                break;
        }

        cout << endl;
    }
    cout << endl;
}

/**
 * Met a' jour une grille de parcours
 * @param[in,out] V - Grille de parcours
 * @param[in] gr - Grille de jeu
 * @param[in] p - la position
 * @param[in] dirx - direction verticale
 * @param[in] diry - direction horizontale
 */
void majGrilleVisite(GrilleVisite &V, const Grille &gr, position p, int dirx, int diry) {

    if (estMiroir(gr, p)) V[p.y][p.x] = gr[p.y][p.x];
    else if (dirx != 0) V[p.y][p.x] = EHORIZ;
    else V[p.y][p.x] = EVERTICAL;
}

/**
 * Test si la positio a ete deja visite par le robot
 * @param[in] V - Grille parcours/visite
 * @param[in] p - position du robot
 * @return Vrai si la position p a' ete deja visitee', faux sinon
 */
bool estDejaVisite(const GrilleVisite &V, position p) {
    return V[p.y][p.x] != 0;
}

// Partie changement de direction

void permuter2i(int &a, int &b) {

    int aux = a;
    a = b;
    b = aux;
}

/**
 * Change de direction en fonction du type de miroir
 * @param[in] miroir - type de miroir
 * @param[in,out] direction - direction du robot
 */
void changeDir(int &miroir, direction &direction) {

    permuter2i(direction.diry, direction.dirx);

    if (miroir == -1) {
        direction.diry *= -1;
        direction.dirx *= -1;
    }
}

// Partie 3.10

/**
 * Initialise les scores des joueurs a 0
 * @param[in,out] s - scores
 * @param[in] nbJoueurs - Nombre de joueurs
 */
void initialiserScores(Scores &s, int nbJoueurs) {

    s.nbJoueurs = nbJoueurs;
    for (int i = 0; i < nbJoueurs; ++i)
        s.t[i] = 0;
}

/**
 * Actulise le score pour un joueur donnee
 * @param[in,out] s - scores
 * @param[in] joueur - joueur qui joue
 * @param[in] score - score realisee
 */
void actualiserScore(Scores &s, int joueur, int score) {

    s.t[joueur - 1] += score;
}

/**
 * Affiche le gagnants ou les gagnants en cas de match null
 * @param[in] s - scores
 */
void afficherGagnants(const Scores &s)
{
    int i;
    for (i = 1; i < s.nbJoueurs && s.t[i] == s.t[i-1]; ++i);

    if (i == s.nbJoueurs) cout << "Partie nulle." << endl;

    else {
        int tab[NJOUEURS];
        int max = s.t[0];
        for (int i = 0; i < s.nbJoueurs; ++i)
            if (s.t[i] > max) max = s.t[i];
        int pos = 0;
        for (int i = 0; i < s.nbJoueurs; ++i)
            if (s.t[i] == max) tab[pos++] = i + 1;
        if (pos == 1) cout << "Le joueur " << tab[0] << " a gagne. Score : " << max << endl;
        else {
            cout << "Les joueurs ";
            for (int i = 0; i < pos; ++i)
                cout << tab[i] << ", ";
            cout << "ont gagnes, avec un score de " << max << endl;
        }
    }
}

////Parcours robot

/**
 * Test si la case correspondante a' la position p contient un objet
 * @param[in] gr - Grille de jeu
 * @param[in] p - position du robot
 * @return Vrai si la case correspondante a' la position p contient un objet, faux sinon
 */
bool estObjet(const Grille &gr, position p) {

    if (gr[p.y][p.x] > 0 && gr[p.y][p.x] < 10) return true;
    return false;
}

/**
 * Test si la case correspondante a' la position p est bien la case finale
 * @param[in] p - position du robot
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 * @return Vrai si la case correspondant a' la position p est la case finale, faux sinon
 */
bool estPosFinValide(position p, int XMAX, int YMAX) {

    if ((p.y == YMAX+1 && p.x == XMAX) || (p.y == YMAX && p.x == XMAX+1)) return true;
    return false;
}

/**
 * Avance le robot d'un case selon la direction
 * @param[in,out] p - position du robot
 * @param[in] dir - Direction robot
 */
void avancerPos(position &p, direction dir) {

    p.x += dir.dirx;
    p.y += dir.diry;
}

/**
 * Increment le score du robot selon l'objet dans la position p, puis efface l'objet de la grille de jeu
 * @param[in,out] gr - grille de jeu
 * @param[in] p - position du robot
 * @param[in,out] rp - RParcours
 */
void incrementScore(Grille &gr,position p, RParcours &rp) {

    rp.nbObjets++;
    rp.score += gr[p.y][p.x];
    gr[p.y][p.x] = 0;
}

/**
 * Test si la case correspondante a' la position p est bien la case finale
 * @param[in,out] gr - grille de jeu
 * @param[in] nbMiroirs - nombre de miroirs pour chaque joueur
 * @param[in] bonus - le bonus a ajouter
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 * @return Un RParcours avec le score realise' et le nombre d'objets ramasses
 */
RParcours parcoursRobot(Grille &gr, int nbMiroirs, int bonus, int XMAX, int YMAX) {

    GrilleVisite grV;
    initialiserGrille(grV, XMAX, YMAX);

    RParcours rp;
    rp.nbObjets = 0;
    rp.score = 0;

    direction dir;
    dir.diry = 0;
    dir.dirx = 1;

    position p;
    p.y = 0;
    p.x = 0;

    while (estValide(p, XMAX, YMAX) && !estDejaVisite(grV, p)) {

        if (estMiroir(gr, p) && grV[p.y][p.x] == 0) nbMiroirs--;

        majGrilleVisite(grV, gr, p, dir.dirx, dir.diry);

        if (estLibre(p, gr)) avancerPos(p, dir);
        else if (estObjet(gr, p)) {
            incrementScore(gr, p, rp);
            avancerPos(p, dir);
        }
        else if (estMiroir(gr, p)) {
            changeDir(gr[p.y][p.x], dir);
            avancerPos(p, dir);
        }
    }

    if (estPosFinValide(p, XMAX, YMAX)) rp.score *= 2;
    if(nbMiroirs == 0) rp.score += bonus;

    afficherGrilleParcours(gr, grV, XMAX, YMAX);
    return rp;
}

/**
 * Test si la case correspondante a' la position p est bien la case finale
 * @param[in,out] gr - grille de jeu
 * @param[in] nbMiroirs - nombre de miroirs pour chaque joueur
 * @param[in] bonus - le bonus a ajouter
 * @param[in] XMAX - Dimension colonnes
 * @param[in] YMAX - Dimension lignes
 * @return Un RParcours avec le score realise' et le nombre d'objets ramasses
 */
RParcours parcoursRobotIA(Grille &gr, int nbMiroirs, int bonus, int XMAX, int YMAX) {

    GrilleVisite grV;
    initialiserGrille(grV, XMAX, YMAX);

    RParcours rp;
    rp.nbObjets = 0;
    rp.score = 0;

    direction dir;
    dir.diry = 0;
    dir.dirx = 1;

    position p;
    p.y = 0;
    p.x = 0;

    while (estValide(p, XMAX, YMAX) && !estDejaVisite(grV, p)) {

        if (estMiroir(gr, p) && grV[p.y][p.x] == 0) nbMiroirs--;

        majGrilleVisite(grV, gr, p, dir.dirx, dir.diry);

        if (estLibre(p, gr)) avancerPos(p, dir);
        else if (estObjet(gr, p)) {
            incrementScore(gr, p, rp);
            avancerPos(p, dir);
        }
        else if (estMiroir(gr, p)) {
            changeDir(gr[p.y][p.x], dir);
            avancerPos(p, dir);
        }
    }

    if (estPosFinValide(p, XMAX, YMAX)) rp.score *= 2;
    if(nbMiroirs == 0) rp.score += bonus;

    return rp;
}

/// Partie 3.11 et 3.12

/**
 * Initialise un JeuRoRam a' partir de ParamsJeu
 * @param[in,out] JR - JeuRoRam
 * @param[in] PJ - joueur qui joue
 */
void initialiserJeu(JeuRoRam &JR, const ParamsJeu &PJ) {

    initialiserGrille(JR.GR, PJ.nbColonnes, PJ.nbLignes);
    JR.NbObjets = PJ.nbObjets;
    initialiserScores(JR.score, PJ.nbJoueurs);
}

/**
 * Test s'il n'y a plus d'objets dans la grille
 * @param[in] JR - JeuRoRam
 * @return Vrai si il n'y a plus d'objet dans la grille, faux sinon
 */
bool finPartie(const JeuRoRam &JR) {

    if (JR.NbObjets == 0) return true;
    return false;
}

/**
 * Return le numero du joueur prochain selon le nombre de joueurs
 * @param[in] joueurActuel - Le joueur qui vient de joueur
 * @param[in] njoueurs - nombre de joueurs
 * @return Le numero du prochain joueur
 */
int adversaire(int joueurActuel, int njoueurs) {

    int joueur = (joueurActuel + 1) % njoueurs;
    if (joueur == 0) return njoueurs;
    return joueur;
}

/**
 * Demande a l'utilisateur de rentrer une dimension maximale de ligne valide
 * @param[in,out] PJ - ParamsJeu
 */
void saisirDimLigne(ParamsJeu &PJ) {

    int dim;

    cout << "Entrez le nombre de lignes - compris entre 4 et 8 : ";
    cin >> dim;

    while (!estValideDimensionLignes(dim)) {
        cout << "ERREUR : entrez un nombre de lignes - compris entre 4 et 8 : ";
        cin >> dim;
    }

    PJ.nbLignes = dim - 1;
}

/**
 * Demande a l'utilisateur de rentrer une dimension maximale de colonne valide
 * @param[in,out] PJ - ParamsJeu
 */
void saisirDimColonne(ParamsJeu &PJ) {

    int dim;

    cout << "Entrez le nombre de colonnes - compris entre 4 et 8 : ";
    cin >> dim;

    while (!estValideDimensionColonnes(dim)) {
        cout << "ERREUR : entrez le nombre de colonnes - compris entre 4 et 8 : ";
        cin >> dim;
    }

    PJ.nbColonnes = dim - 1;
}

/**
 * Demande a l'utilisateur de rentrer le nombre d'objets a placer dans la grille de jeu compris entre 4 et MAXOBJ
 * @param[in,out] PJ - ParamsJeu
 * @param[in,out] MAXOBJ - Nombre maximal d'objets qu'on peut placer
 */
void saisirNbObjets(ParamsJeu &PJ, int MAXOBJ) {

    int nbObjet;

    int nbObjetMax = (PJ.nbLignes + 1) * (PJ.nbColonnes + 1) / 2;
    cout << "Saisir le nombre d'objet dans [4..." << nbObjetMax << "] : ";
    cin >> nbObjet;

    while (!nbObjetValide(nbObjet,MAXOBJ)) {
        cout << "ERREUR : saisir le nombre d'objet dans [4..." << nbObjetMax << "] : ";
        cin >> nbObjet;
    }

    PJ.nbObjets = nbObjet;
}

/**
 * Demande a' l'utilisateur de rentrer le nombre de joueurs
 * @param[in,out] PJ - ParamsJeu
 */
void saisirNbJoueurs(ParamsJeu &PJ) {

    int nb;

    cout << "Saisir le nombre de joueurs (2 ou 3) : ";
    cin >> nb;

    while (!estValideNbJoueurs(nb)) {
        cout << "ERREUR : le nombre de joueurs doit etre 2 ou 3 : ";
        cin >> nb;
    }

    PJ.nbJoueurs = nb;
}

/**
 * Demande a' l'utilisateur de rentrer le nombre de joueurs
 * @param[in,out] PJ - ParamsJeu
 */
void saisirNbMiroirs(ParamsJeu &PJ) {

    int nb;

    cout << "Saisir le nombre de Miroirs [1..."<<MIRMAX<<"] : ";
    cin >> nb;

    while (!estValideNbMiroirs(nb)) {
        cout << "ERREUR : le nombre de joueurs doit etre dans [1..."<<MIRMAX<<"] : ";
        cin >> nb;
    }

    PJ.nbMiroirs = nb;
}

/**
 * Saisie dans un ParamJeu la dimension des lignes,colonnes, le nombre de joueurs,de miroirs et les points bonus
 * @param[in,out] PJ - ParamsJeu
 */
void saisirParamJeu(ParamsJeu &PJ) {

    saisirDimLigne(PJ);
    saisirDimColonne(PJ);
    saisirNbJoueurs(PJ);

    int MAXOBJETS = (PJ.nbColonnes + 1) * (PJ.nbLignes + 1) / 2;
    saisirNbObjets(PJ, MAXOBJETS);

    saisirNbMiroirs(PJ);

    cout << "Entrez la valeur du bonus : ";
    cin >> PJ.bonus;
}

/**
 * Actualise le nombre d'objets disponibles dans la grille de jeu
 * @param[in,out] JR - JeuRoRam
 * @param[in] rp - RParcours
 */
void actualiserNombreObjets(JeuRoRam &JR, RParcours rp) {

    JR.NbObjets -= rp.nbObjets;
}

/**
 * Lance une partie
 * @param[in,out] PJ - ParamsJeu
 */
void jouerUnePartie(ParamsJeu &PJ) {

    JeuRoRam JR;
    saisirParamJeu(PJ);
    initialiserJeu(JR,PJ);

    poserObjetsGrille(PJ.nbObjets, JR.GR, PJ.nbColonnes, PJ.nbLignes);

    int joueur = aleatoire(1, PJ.nbJoueurs);
    cout << "C'est le joueur numero " << joueur << " qui commence" << endl;

    RParcours rp;

    while (JR.NbObjets != 0) {

        cout << "C'est au tour du joueur numero " << joueur << endl;
        afficherGrille(JR.GR, PJ.nbColonnes, PJ.nbLignes);
        poserMiroirs(JR.GR, PJ.nbMiroirs, PJ.nbColonnes, PJ.nbLignes);
        rp = parcoursRobot(JR.GR, PJ.nbMiroirs, PJ.bonus, PJ.nbColonnes, PJ.nbLignes);

        actualiserScore(JR.score, joueur, rp.score);
        actualiserNombreObjets(JR, rp);

        cout << "Le joueur numero " << joueur << " a ramasse " << rp.nbObjets << " objets" << endl;
        cout << "Total points joueur " << joueur << " : " << JR.score.t[joueur - 1] << endl << endl;

        effacerMiroirs(JR.GR, PJ.nbColonnes, PJ.nbLignes);
        joueur = adversaire(joueur, PJ.nbJoueurs);
    }

    afficherGagnants(JR.score);
}

/**
 * Calcule pour de bonnes position et miroir pour le joueur "ordinateur"
 * @param[in] JR - un JeuRoRam
 * @param[in] PJ - les parametres du jeu
 * @return Un choixIA contenant de bon emplacement pour les miroir de "ordinateur"
 */
choixIA meilleurChoixPourAI(const JeuRoRam &JR, const ParamsJeu PJ) {

    JeuRoRam AUX = JR;
    choixIA cIA;
    position p, p2;
    RParcours rp;
    int scoreMax = 0;

    for (int j = 0; j < PJ.nbLignes; j++)
    {
        for (int i = 0; i < PJ.nbColonnes; i++) {
            p.x = i; p.y = j;
            if (estJouable(p, AUX.GR, PJ.nbColonnes, PJ.nbLignes))
            {
                for (int j2 = 0; j2 < PJ.nbLignes; j2++)
                    for (int i2 = 0; i2 < PJ.nbColonnes; i2++) {
                        p2.x = i2; p2.y = j2;
                        if (estJouable(p2, AUX.GR, PJ.nbColonnes, PJ.nbLignes))
                        {
                            AUX.GR[p.y][p.x] = VMIROIR1;
                            AUX.GR[p2.y][p2.x] = VMIROIR1;
                            rp = parcoursRobotIA(AUX.GR, PJ.nbMiroirs, PJ.bonus, PJ.nbColonnes, PJ.nbLignes);
                            if (rp.score > scoreMax) {
                                scoreMax = rp.score;
                                cIA.tab[0].x = p.x; cIA.tab[0].y = p.y; cIA.tab[0].typeMiroir = VMIROIR1;
                                cIA.tab[1].x = p2.x; cIA.tab[1].y = p2.y; cIA.tab[1].typeMiroir = VMIROIR1;
                            }
                            effacerMiroirs(AUX.GR, PJ.nbColonnes, PJ.nbLignes);
                        }
                    }

                for (int j2 = 0; j2 < PJ.nbLignes; j2++)
                    for (int i2 = 0; i2 < PJ.nbColonnes; i2++) {
                        p2.x = i2; p2.y = j2;
                        if (estJouable(p2, AUX.GR, PJ.nbColonnes, PJ.nbLignes))
                        {
                            AUX.GR[p.y][p.x] = VMIROIR1;
                            AUX.GR[p2.y][p2.x] = VMIROIR2;
                            rp = parcoursRobotIA(AUX.GR, PJ.nbMiroirs, PJ.bonus, PJ.nbColonnes, PJ.nbLignes);
                            if (rp.score > scoreMax) {
                                scoreMax = rp.score;
                                cIA.tab[0].x = p.x; cIA.tab[0].y = p.y; cIA.tab[0].typeMiroir = VMIROIR1;
                                cIA.tab[1].x = p2.x; cIA.tab[1].y = p2.y; cIA.tab[1].typeMiroir = VMIROIR2;
                            }
                            effacerMiroirs(AUX.GR, PJ.nbColonnes, PJ.nbLignes);
                        }
                    }

            }
        }

    }

    for (int j = 0; j < PJ.nbLignes; j++)
    {
        for (int i = 0; i < PJ.nbColonnes; i++) {
            p.x = i; p.y = j;
            if (estJouable(p, AUX.GR, PJ.nbColonnes, PJ.nbLignes))
            {
                for (int j2 = 0; j2 < PJ.nbLignes; j2++)
                    for (int i2 = 0; i2 < PJ.nbColonnes; i2++) {
                        p2.x = i2; p2.y = j2;
                        if (estJouable(p2, AUX.GR, PJ.nbColonnes, PJ.nbLignes))
                        {
                            AUX.GR[p.y][p.x] = VMIROIR1;
                            AUX.GR[p2.y][p2.x] = VMIROIR1;
                            rp = parcoursRobotIA(AUX.GR, PJ.nbMiroirs, PJ.bonus, PJ.nbColonnes, PJ.nbLignes);
                            if (rp.score > scoreMax) {
                                scoreMax = rp.score;
                                cIA.tab[0].x = p.x; cIA.tab[0].y = p.y; cIA.tab[0].typeMiroir = VMIROIR2;
                                cIA.tab[1].x = p2.x; cIA.tab[1].y = p2.y; cIA.tab[1].typeMiroir = VMIROIR1;
                            }
                            effacerMiroirs(AUX.GR, PJ.nbColonnes, PJ.nbLignes);
                        }
                    }

                for (int j2 = 0; j2 < PJ.nbLignes; j2++)
                    for (int i2 = 0; i2 < PJ.nbColonnes; i2++) {
                        p2.x = i2; p2.y = j2;
                        if (estJouable(p2, AUX.GR, PJ.nbColonnes, PJ.nbLignes))
                        {
                            AUX.GR[p.y][p.x] = VMIROIR1;
                            AUX.GR[p2.y][p2.x] = VMIROIR2;
                            rp = parcoursRobotIA(AUX.GR, PJ.nbMiroirs, PJ.bonus, PJ.nbColonnes, PJ.nbLignes);
                            if (rp.score > scoreMax) {
                                scoreMax = rp.score;
                                cIA.tab[0].x = p.x; cIA.tab[0].y = p.y; cIA.tab[0].typeMiroir = VMIROIR2;
                                cIA.tab[1].x = p2.x; cIA.tab[1].y = p2.y; cIA.tab[1].typeMiroir = VMIROIR2;
                            }
                            effacerMiroirs(AUX.GR, PJ.nbColonnes, PJ.nbLignes);
                        }
                    }

            }
        }

    }

    return cIA;
}

/**
 * Lance une partie avec IA
 * @param[in,out] PJ - ParamsJeu
 */
void jouerUnePartieAvecIA(ParamsJeu &PJ) {

    JeuRoRam JR;
    saisirParamJeu(PJ);
    initialiserJeu(JR,PJ);
    PJ.nbMiroirs = 2;

    poserObjetsGrille(PJ.nbObjets, JR.GR, PJ.nbColonnes, PJ.nbLignes);

    int joueur = 1;
    cout << "C'est le joueur numero " << joueur << " qui commence" << endl;

    RParcours rp;

    while (JR.NbObjets != 0) {

        cout << "C'est au tour du joueur numero " << joueur << endl;
        afficherGrille(JR.GR, PJ.nbColonnes, PJ.nbLignes);


        if (joueur == 2) // Determine si c'est a l'ordinateur de jouer
        {
            choixIA cIA = meilleurChoixPourAI(JR, PJ);
            for (int i = 0; i < PJ.nbMiroirs; ++i)
                JR.GR[cIA.tab[i].y][cIA.tab[i].x] = cIA.tab[i].typeMiroir; // Place le miroir du "meilleur choix"
        }
        else poserMiroirs(JR.GR, PJ.nbMiroirs, PJ.nbColonnes, PJ.nbLignes);

        rp = parcoursRobot(JR.GR, PJ.nbMiroirs, PJ.bonus, PJ.nbColonnes, PJ.nbLignes);

        actualiserScore(JR.score, joueur, rp.score);
        actualiserNombreObjets(JR, rp);

        cout << "Le joueur numero " << joueur << " a ramasse " << rp.nbObjets << " objets" << endl;
        cout << "Total points joueur " << joueur << " : " << JR.score.t[joueur - 1] << endl << endl;

        effacerMiroirs(JR.GR, PJ.nbColonnes, PJ.nbLignes);
        joueur = adversaire(joueur, PJ.nbJoueurs);
    }

    afficherGagnants(JR.score);
}





int main()
{
    srand(time(0));
    int choix;
    ParamsJeu PJ;
    cout << "Si vous (ou d'autres joueurs) voulez jouer avec une IA, entrez '1', sinon entrez '2' : ";
    cin >> choix;
    if (choix == 2) jouerUnePartie(PJ);
    else jouerUnePartieAvecIA(PJ);
    return 0;
}
