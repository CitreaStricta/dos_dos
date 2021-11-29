#include "raster.h"

int main(int argc, char const *argv[]) // compilar con: g++ -std=c++17 -O3 -DNDEBUG -I ~/include -L ~/lib program.cpp -o program -lsdsl -ldivsufsort -ldivsufsort64
{ // ruta a los files en el vscode /home/vicen/p/edaa/proyectos/dos/data/
    if(argc < 2)
    {
        cout << "Es necesaria la ruta a los data sets\n" << endl;
        return 0;
    }

    string ruta = argv[1];

    raster ras(8, ruta + "8x8");
    ras.set_int_vector();
    ras.get_Entropy();
    cout << "Entropia de data_set " << "'8x8': " <<  ras.get_Entropy() << endl;
    ras.vector_to_bitset();
    ras.creark2Tree();

    ras.matrizDiferencias();

    ras.informe();


    raster ras1(128, ruta + "128x128");
    ras1.set_int_vector();
    ras1.get_Entropy();
    cout << "Entropia de data_set " << "'128x128': " <<  ras1.get_Entropy() << endl;
    ras1.vector_to_bitset();
    ras1.creark2Tree();

    ras1.matrizDiferencias();

    ras1.informe();

    raster ras2(512, ruta + "512x512");
    ras2.set_int_vector();
    ras2.get_Entropy();
    cout << "Entropia de data_set " << "'512x512': " <<  ras2.get_Entropy() << endl;
    ras2.vector_to_bitset();
    ras2.creark2Tree();

    ras2.matrizDiferencias();

    ras2.informe();
// ----------------- experimentos de tiempo -----------------
/*
    srand(time(NULL));
    int random, L, R;
    int rep = 200;
    double time, duracion;
    int nElementosXResultado = 50000;
    int cant_valores_Tabla = 20;
    clock_t start;
    for (int n = nElementosXResultado; n <= nElementosXResultado*cant_valores_Tabla; n += nElementosXResultado)
    {
        start = clock();
        for (int i = 0; i < rep; i++)
        {
            SparseTable Prueba_SpT(vec); // velocidad de busqueda usando vPrima, el bit map y usando rank en ellos
        }

        duracion = (double)(clock()-start)/CLOCKS_PER_SEC/rep;
        cout << setprecision(10) << fixed << n << "," << duracion/(double)rep << endl;
        SparseTable SpT(vec);
        start = clock();

        for (int i = 0; i < rep; i++)
        {
            SpT.RMQ(L, R); // Velocidad de busqueda de Spt
        }

        duracion = (double)(clock()-start)/CLOCKS_PER_SEC/rep;
        cout << setprecision(10) << fixed << n << "," << duracion/(double)rep << endl;
    }
*/



/* EJEMPLO UTILIZADO PARA ENTENDER EL FUNCIONAMIENTO DE RANK
    int_vector<> v = {3, 3, 3, 2, 1, 5, 5, 5, 7, 7, 7};  //este parte de 0 a n-1
    bit_vector b =   {1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0};  //este parte de 1 a n
    int_vector<> vPrima = {3, 2, 1, 5, 7};               //este parte de 0 a n-1

    sd_vector<> sdb(b);
    sd_vector<>::rank_1_type sdb_rank(&sdb);
    int i;
    cin >> i;
+
    cout << "Rank es: " << sdb_rank(i) << " en v hay: " << v[i - 1] << " que debe ser igual a vPrima que es:  " << vPrima[sdb_rank(i) - 1] << endl;
*/
    return 0;
}
