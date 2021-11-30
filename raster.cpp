#include "raster.h"
#include <sdsl/k2_tree.hpp>

using namespace k2_tree_ns;

raster::raster(int d, string patha)
{
    dim = d;
    path = patha;
}

raster::~raster(){}

// lee los data_sets y los guarda en un int_vector "v"
void raster::set_int_vector()
{
// leo los files donde se encuentra el data_set y los guardo en un vector auxiliar
    set<filesystem::path> pathOrdenados;
    for (auto & p : filesystem::directory_iterator(path))
        pathOrdenados.insert(p.path());

    vector<int> auxVec;
    for (auto & archivo : pathOrdenados)
    {
        ifstream file(archivo.string(), ios::in);
        string linea;
        vector<vector<int>> matriz;
        while(getline(file, linea))
        {
            stringstream sstream(linea);
            string s;
            vector<int> vecPrevio;
            while(sstream >> s) 
            { // aqui se guardan las lineas de numeros
                auxVec.push_back(stoi(s));
                vecPrevio.push_back(stoi(s));
            }
            // aqui se guardan las lineas dentro de una matriz
            matriz.push_back(vecPrevio);
        }
        // aqui se guarda la matriz en el arbol
        mat_list.push_back(matriz);
        file.close();
    }

// guardo los datos del vector auxiliar en el int_vector v
    v.resize(auxVec.size());
    for (int i = 0; i < auxVec.size(); i++)
    {
        v[i] = auxVec[i];
    }
    // para ver los datos en el int_vector v
    //cout<< v << endl;
}

// me entrega la entropia del data_set
double raster::get_Entropy()
{
// lleno mapa con los valores del int_vector v
    map<int, int> myMap;

    for (int i = 0; i < v.size(); i++)
    {
        int number = v[i];
        auto it = myMap.find(number);
        if(it == myMap.end())
            myMap.insert(make_pair(number,1));
        else
            it->second++;
    }

// calcula cantidad de datos N en el int_vector v
    int n = 0;
    for(auto it = myMap.begin(); it!=myMap.end(); it++)
        n += it->second;
    H = 0;
// calcula probabilidades y obtiene la entropia
    for(auto it = myMap.begin(); it!=myMap.end(); it++){
        double dato_map = (double)it->second/(double)n;
        H += dato_map*log2(dato_map);
    }
    H *= -1;

// retorna un double con la entropia del data_set
    return H;
}

// genera el vector vPrima y el bitmap del int_vector v
void raster::convert_to_succinct_version()
{
    b.resize(v.size()); // se debe pasar el tamaño de vec (8x8, 128x128 o 512x512 segun corresponda)
    vector<int> valores; // debe ser traspasado a un int_vector
    valores.push_back(v[0]);
    b[0] = 1;
    int j=1;
    int k=1;
    for(int i=1; i<v.size(); i++){
        if(v[i]==v[i-1]){
            b[j] = 0;
            j++;
        }else{
            b[j] = 1;
            j++;
            valores.push_back(v[i]);
            k++;
        }
    }
    
// se opuede usar este print para ver todos los valores en b
    // cout << b << endl;
    
    vPrima.resize(valores.size());
    for (int i = 0; i < valores.size(); i++)
        vPrima[i] = valores[i];
    
// se puede usar este print para ver todos los valores en vPrima
    // cout << vPrima << endl;

// s
 
    rrrv = rrr_vector<>(b);
    rrrv_rank.set_vector(&rrrv);
    rrr_vector<> rrrv(b);
    cout << "peso (en bytes) de rrr_vector: " << size_in_bytes(rrrv) << endl;

    // cout << "peso de sd_vector y rrr_vector con bit_compress: " << endl;

    // util::bit_compress(sdv.);
    // cout << size_in_bytes(sdv) << endl;

    // util::bit_compress(rrrv);
    // cout << size_in_bytes(rrrv) << endl;
}

// numero: el i-esimo elemento en el int_vector v
int raster::at(int indx)
{
    if(indx == 0) return v[indx];
    int j = rrrv_rank.rank(indx)-1;
    return vPrima[j];
}

void raster::creark2Tree()
{
    vector<vector<vector<int>>> matriz3d;
    int sumaBytes=0;
    for (int i = 1; i < mat_list.size(); i++)
    {
        vector<vector<int>> mat;
        auto mi_1 = mat_list[i - 1];
        auto mi = mat_list[i];
        for (int j = 0; j < mi_1.size(); j++)
        {
            vector<int> fila;
            for (int k = 0; k < mi_1.size(); k++)
            {
                int diff = (mi_1[j][k] != mi[j][k]); // resultado de la expresion y lo almacena
                fila.push_back(diff);
            }
            mat.push_back(fila);
        }

        matriz3d.push_back(mat);
        k2_tree<2> k2Aux(mat);
        //cout<<"BM_"<<i<<": "<<size_in_bytes(k2Aux)<<endl;
        sumaBytes += size_in_bytes(k2Aux);
    }
    cout<<"La suma total de todos los k2_tree es: "<<sumaBytes<<" bytes."<<endl;
}

void raster::matrizDiferencias(){
    
    vector<vector<int>> matrizBase = mat_list[0]; //quizas podriamos guardar la matriz base como atributo de la clase
    vector<int> diferencias;
    bit_vector repeticiones(v.size());
    int indicador=0; //indica cuantos elementos hay en el bitmap "repeticiones"

    for(int i=1; i<120; i++){
        indicador = calcularDiferencia(mat_list[i], mat_list[i-1], diferencias, repeticiones, indicador);
    }

    int_vector<> differences; //v'
    differences.resize(diferencias.size());
    for (int i = 0; i < diferencias.size(); i++)
        differences[i] = diferencias[i];

    cout
    << "Peso (en bytes) de differences  (int_vector): " << size_in_bytes(differences) << "\n"
    << "Peso (en bytes) de repeticiones (bit_vector): " << size_in_bytes(repeticiones)
    << endl;

    //RECORDAR QUE (int64_t)differences[i] ME DA EL VALOR NEGATIVO DE LAS DIFERENCIAS
/*
    for (int i = 0; i < differences.size(); i++)
    {
        cout<<(int64_t)differences[i] << " ";
    }
    cout << endl << endl;
    */
    //cout << repeticiones << endl;

    
}

int raster::calcularDiferencia(vector<vector<int>> mActual, vector<vector<int>> mAnterior, vector<int> &dif, bit_vector &rep, int ind){
    for(int i=0; i<mActual.size(); i++){
        for(int j=0; j<mActual[i].size(); j++){
            int resta = mActual[i][j] - mAnterior[i][j];
            if(dif.size() == 0){
                rep[0] = 1;
                dif.push_back(resta);
                ind++;
                continue;
            }
            if(resta == dif.back()){
                rep[ind] = 0;
            }else{
                rep[ind] = 1;
                dif.push_back(resta);
            }
            ind++;
        }
    }
    return ind;
}

void raster::informe()
{
    cout
    << "Datos para data_set " << dim << "x" << dim << " sin 'util::bit_compress()'\n"
    << "Entropia: " << H << "\n"
    << "Peso (en bytes) vector v: " << size_in_bytes(v) << "\n"
    << "Peso (en bytes) vector vPrima: " << size_in_bytes(vPrima) << "\n"
    << "Peso (en bytes) vector de bits: " << size_in_bytes(b) << "\n"
    << "Peso (en bytes) rrr_vector: " << size_in_bytes(rrrv) << "\n"
    << endl;

    util::bit_compress(v);
    util::bit_compress(vPrima);
    util::bit_compress(b);

    cout
    << "Datos para data_set " << dim << "x" << dim << " con 'util::bit_compress()'\n"
    << "Entropia: " << H << "\n"
    << "Peso (en bytes) vector v: " << size_in_bytes(v) << "\n"
    << "Peso (en bytes) vector vPrima: " << size_in_bytes(vPrima) << "\n"
    << "Peso (en bytes) vector de bits: " << size_in_bytes(b) << "\n"
    << endl;
}

int raster::v_at(int indx)
{
    return v[indx];
}