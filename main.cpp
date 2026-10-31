#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <random>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace chrono;

// MIN HEAP AVEC VECTOR

void inserer(vector<int>& tas, int valeur)
{
    tas.push_back(valeur);
    int i = tas.size() - 1;

    // Remonter l'element s'il est plus petit que son parent
    while (i > 0)
    {
        int parent = (i - 1) / 2;

        if (tas[i] < tas[parent])
        {
            swap(tas[i], tas[parent]);
            i = parent;
        }
        else
            break;
    }
}

// Extraire le minimum
int extraireMin(vector<int>& tas)
{
    int minimum = tas[0];

    tas[0] = tas.back();
    tas.pop_back();

    int i = 0;
    int n = tas.size();

    while (true)
    {
        int gauche = 2 * i + 1;
        int droite = 2 * i + 2;
        int plusPetit = i;

        if (gauche < n && tas[gauche] < tas[plusPetit])
            plusPetit = gauche;

        if (droite < n && tas[droite] < tas[plusPetit])
            plusPetit = droite;

        if (plusPetit == i)
            break;

        swap(tas[i], tas[plusPetit]);
        i = plusPetit;
    }

    return minimum;
}

int main()
{
    int tailles[] = {10000, 50000, 100000, 500000, 1000000};

    mt19937 gen(42);
    uniform_int_distribution<int> dist(1, 10000000);

    cout << fixed << setprecision(6);

    cout << "\n========== BENCHMARKING MIN HEAP ==========\n\n";

    cout << left
         << setw(12) << "Elements"
         << setw(22) << "Insertion Vector(ms)"
         << setw(22) << "Insertion PQ(ms)"
         << setw(22) << "Extraction Vector(ms)"
         << setw(22) << "Extraction PQ(ms)"
         << endl;

    cout << string(100, '-') << endl;

    for (int taille : tailles)
    {
        vector<int> donnees;

        // Generer les memes donnees pour les deux tas
        for (int i = 0; i < taille; i++)
            donnees.push_back(dist(gen));

        vector<int> tasVector;

        // priority_queue avec greater pour obtenir un Min Heap
        priority_queue<int, vector<int>, greater<int>> tasPQ;

        // Insertion dans le vector
        auto debut = steady_clock::now();

        for (int x : donnees)
            inserer(tasVector, x);

        auto fin = steady_clock::now();

        double tempsInsertionVector =
            duration<double, milli>(fin - debut).count();

        // Insertion dans priority_queue
        debut = steady_clock::now();

        for (int x : donnees)
            tasPQ.push(x);

        fin = steady_clock::now();

        double tempsInsertionPQ =
            duration<double, milli>(fin - debut).count();

        // Extraction du minimum dans le vector
        debut = steady_clock::now();

        int minVector = extraireMin(tasVector);

        fin = steady_clock::now();

        double tempsExtractionVector =
            duration<double, milli>(fin - debut).count();

        // Extraction du minimum dans priority_queue
        debut = steady_clock::now();

        int minPQ = tasPQ.top();
        tasPQ.pop();

        fin = steady_clock::now();

        double tempsExtractionPQ =
            duration<double, milli>(fin - debut).count();

        // Affichage des resultats
        cout << left
             << setw(12) << taille
             << setw(22) << tempsInsertionVector
             << setw(22) << tempsInsertionPQ
             << setw(22) << tempsExtractionVector
             << setw(22) << tempsExtractionPQ
             << endl;

        if (minVector != minPQ)
            cout << "Erreur : minimums differents !" << endl;
    }

    return 0;
}
