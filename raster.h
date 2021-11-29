#ifndef RASTER
#define RASTER

#include <sdsl/suffix_arrays.hpp>
#include <bits/stdc++.h>
#include <fstream>
#include <filesystem>
#include <string>
#include <set>


using namespace std;
using namespace sdsl;

class raster
{
private:
    int dim;
    double H;
    string path;
    int_vector<> v;
    bit_vector b;
    int_vector<> vPrima;
    sd_vector<> sdv;
    sd_vector<>::rank_1_type sdv_rank;
    vector<vector<vector<int>>> mat_list;
    int calcularDiferencia(vector<vector<int>> mActual, vector<vector<int>> mAnterior, vector<int> &dif, bit_vector &rep, int ind);
public:
    raster(int dim, string path);
    ~raster();
    void set_int_vector();
    double get_Entropy();
    void vector_to_bitset();
    int at(int valor);
    void creark2Tree();
    void matrizDiferencias();
    int acceder();
    void informe();
};
#endif