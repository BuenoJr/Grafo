#include <iostream>
#include <vector>
#include <set>
#include <list>
#include <chrono>
#include <iomanip>

using namespace std::chrono;
using std::vector;
using std::set;
using std::list;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::setw;

// Class de modelagem

class Grafo{
public:
    long vertice;
    unsigned long long int arestasLeitura;
    int** matriz;
    int* grau;
    set<vector<int>> clique;

    Grafo(){}

    virtual ~Grafo() {}

    void iniciarMatriz(){
        // Alocando os valores
        matriz = new int*[vertice];
        grau = new int[vertice];

        for(auto i = 0; i < vertice; i++){
            matriz[i] = new int[vertice];
        }

        // Inicializando tudo com valores 0
        for(auto i = 0; i < vertice; i++){
            grau[i] = 0;
            for(auto j = 0; j < vertice; j++){
                matriz[i][j] = 0;
            }
        }
    }
    // Desalocando variaveis
    void desalocarMatriz(){
        for(auto i = 0; i < vertice; i++){
            delete[] matriz[i];
        }
        delete []matriz;
        delete []grau;
    }

    // Inserindo as aresta na matriz e aumentado os graus
    void inserirAresta(int origem,int destino){
        matriz[origem][destino]++;
        matriz[destino][origem]++;
        aumentarGrau(origem);
        aumentarGrau(destino);
    }

    // aumenta o grau dos vertices
    void aumentarGrau(int vertice){
        grau[vertice]++;
    }

    void verificarClique(list<int>& vizinho, list<int>& atual){
        bool ehAdjacente;

        for(auto eixoX : vizinho){
            ehAdjacente = true;

            for(auto eixoY : atual)
                if(matriz[eixoX][eixoY] == 0){
                    ehAdjacente = false;
                    break;
                }

            if(ehAdjacente)
                atual.push_back(eixoX);
        }
    }

    bool existeAresta(){
        for(auto i = 0; i < vertice; i++){
            for(auto j = 0; j < vertice; j++){
                if(matriz[i][j] == 1)
                    return true;
            }
        }
        return false;
    }

    int pegarMaiorGrau(){
        int retorno = 0,maior = 0;
        for(long i = 0; i < vertice; i++){
            cout << "Posicao: " << i << " valor: " << grau[i] << endl;
            if(grau[i] > maior){
                maior = grau[i];
                retorno = i;
            }
        }
        return retorno;
    }

    void execute(){
        // Declaracao de variaveis
        list<int> vizinhos;
        list<int> cliqueAtual;
        vector<int> cliqueAuxiliar;
        int somatorio = 0;

        unsigned long long int qAresta = arestasLeitura * 2; // Valor que armazenar a quantidade de aresta ela que e a condicao para o laco



        do{
            for(unsigned int i=0; i< vertice; i++){
                int maiorGrau = pegarMaiorGrau();
                cliqueAtual.push_back(i);

                for(unsigned int j=0; j<vertice; j++)
                    if(matriz[i][j] > 0)
                        vizinhos.push_back(j);

                if(vizinhos.empty()){
                    cliqueAtual.clear();
                    continue;
                }

                verificarClique(vizinhos, cliqueAtual);

                cliqueAuxiliar.clear();
                for(list<int>::iterator it = cliqueAtual.begin(); it != cliqueAtual.end(); it++){
                    cliqueAuxiliar.push_back(*it);
                    for(list<int>::iterator itt = it; itt != cliqueAtual.end(); ++itt){
                        matriz[*it][*itt] = 0;
                        matriz[*itt][*it] = 0;
                        qAresta -= 2;
                    }
                }

                if(!cliqueAuxiliar.empty()){
                    clique.insert(cliqueAuxiliar);
                    for(auto it : cliqueAuxiliar){
                        grau[it] -= cliqueAuxiliar.size() - 1;
                    }
                }

                //cout << *this<< endl;
                vizinhos.clear();
                cliqueAtual.clear();
                if(qAresta == 0)
                    break;
            }  
        }while (existeAresta());
        cout << *this << endl;
        cout << "teste" << endl;
    }

    void imprimirCliques(){
        cout << " Todos os cliques encontrados : " << endl;
        cout << "Quantidade de cliques" << clique.size() << endl;
        int soma = 0;
        for(auto it = clique.begin(); it != clique.end(); ++it){
            for(auto vertice : *it) {
                cout << " " << vertice;
                soma++;
            }
            cout << ". " << endl;
        }
        cout << "Quantidade de arestas pegas: " << soma * 2 << endl;
    }

    friend std::ostream &operator<<(std::ostream &os, const Grafo &grafo) {

        os << "Vertices: " << grafo.vertice << " | Arestas: " << grafo.arestasLeitura << "\n";
        for(auto i = 0; i < grafo.vertice; i++){
            os << "V " <<setw(3)<< i + 1<< ": ";
            for(auto j = 0; j < grafo.vertice; j++){
                //if(i == j && grafo.matriz[i][j] != 0){
                    //os.clear(os.beg);
                  //  os << "Diagonal principal tem valor diferente de 0";
                //    return os;
                //}
                os << grafo.matriz[i][j] << " ";
            }
            os << "\n";
        }
        return os;
    }
};

// Funcoes
string converterMinusculo(string nome);
int lerArquivo(string nome,Grafo* grafo);

int main(int argc, char *argv[]) {
    // Variaveis da main
    duration<double> time_span2;
    string nome;
    Grafo* graph = new Grafo();


    if(argc > 1){
        if(lerArquivo(argv[1],graph)){
            do{
                cout << "Digite o nome do arquivo e sua extensao ou digite sair:";
                cin >> nome;
                string aux = converterMinusculo(nome);
                if(aux.compare("sair") == 0)
                    exit(1);
            }while(lerArquivo(argv[1],graph));
        }
    }else{
        do{
            cout << "Digite o nome do arquivo e sua extensao ou digite sair:";
            cin >> nome;
            string aux = converterMinusculo(nome);
            if(aux.compare("sair") == 0)
                exit(1);
        }while(lerArquivo(nome,graph));
    }

    cout << *graph << endl;

    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    //gerar a solução aqui
    graph->execute();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
    graph->imprimirCliques();
    cout << "Tempo de execucao: " << time_span.count() << " ms" << endl;
    return 0;
}

string converterMinusculo(string nome){
    char auxiliar[256];
    for(int i = 0; i < nome.length(); i++){
        auxiliar[i] = tolower(nome[i]);
    }
    return auxiliar;
}

int lerArquivo(string nome,Grafo* grafo){
    // Variavel da funcao
    FILE* file; // Arquivo de leitura
    string e;   // Auxiliar de leitura
    char edge[10];    // Auxiliar de leitura
    char p;

    // Variaveis de leitura de cabeçalho
    int vertice;
    unsigned long long int aresta;

    // Variaveis de auxilio para leitura
    unsigned long long int i = 0;
    int origem,destino;


    // Lendo o arquivo e tentando abrir
    if((file = fopen(nome.c_str(),"r")) == NULL)
        return 1;

    // Lendo a primeira linha que contem os valores da quantidade de aresta e vertice
    //fscanf(file,"%c %[A-z a-z] %d %d '\n'",&p,edge,&vertice,&aresta);
    fscanf(file, "%c %[A-z a-z] %d %llu '\n'",&p, edge, &vertice, &aresta);

    // Armazendo os valores no grafo
    grafo->arestasLeitura = aresta;
    grafo->vertice = vertice;

    // Inicializando Matriz
    grafo->iniciarMatriz();

    i = 0;
    // Lendo os valores da aresta do arquivo
    while(fscanf(file, "%c %d %d '\n'",&p, &origem, &destino) != EOF){
        grafo->inserirAresta(origem-1, destino-1);
        i++;
    }
    // Fechado o arquivo
    fclose(file);

    if (i != aresta)
        return 1;
    return 0;
}