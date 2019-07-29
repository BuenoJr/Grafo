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
using std::sort;

// Class de modelagem

class Grafo{
public:
    long vertice;
    unsigned long long int arestasLeitura;
    int** matriz;
    bool** marcado;
    int* grau;
    vector<vector<int > > clique;

    Grafo(){}

    virtual ~Grafo() {}

    void iniciarMatriz(){
        // Alocando os valores
        matriz = new int*[vertice];
        marcado = new bool*[vertice];
        grau = new int[vertice];

        // Alocando cada posicao da matriz
        for(auto i = 0; i < vertice; i++){
            matriz[i] = new int[vertice];
            marcado[i] = new bool[vertice];
        }

        // Inicializando tudo com valores 0
        for(auto i = 0; i < vertice; i++){
            grau[i] = 0;
            for(auto j = 0; j < vertice; j++){
                matriz[i][j] = 0;
                marcado[i][j] = false;
            }
        }
    }
    // Desalocando variaveis
    void desalocarMatriz(){
        for(auto i = 0; i < vertice; i++){
            delete[] matriz[i];
            delete[] marcado[i];
        }
        delete []marcado;
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

    // verificar se com o vertice examinando tem adjacência com todos os outros posiveis vertices
    // vizinhos e ele tambem apresenta adjacência com todos formando um clique
    // Processo demorado - Tentar melhorar depois
    void verificarClique(list<int>& vizinho, list<int>& atual){
        bool adjacencia;

        for(auto x : vizinho){
            // Inicia como se como se ele fosse adjancencia
            adjacencia = true;
            for(auto y : atual)
                if(matriz[x][y] == 0){  // Verifica se ele tem adjacência com todos os vertices ja inserido
                    adjacencia = false;
                    break;  // Caso nao tenha com um nao forma clique e vai para outros
                }
            if(adjacencia){ // Se mantiver adjacênte e inserido em uma list
                            // e na proxima interacao tambem vai ser comparado
                atual.push_back(x);
            }
        }
    }

    // Condicao de parada
    bool existeMarcado(){
        for(auto i = 0; i < vertice; i++){
            for(auto j = 0; j < vertice; j++){
                if(!marcado[i][j]){ // Caso houver apenas uma aresta que nao estaja marcada deve continuar
                                    // rodando o algoritmo
                    return true;
                }
            }
        }
        // Caso verificou tudo e nao encontrou nenhum marcado encerrou o programa
        return false;
        //  Esse metodo pode ser melhorando salvando os valores e subtraindo sempre quando marcar alguma aresta
        //  Tentar implementar depois para melhorar complexidade
    }

    //  Metodo antigo que quando o codigo era por remocao de aresta
    //  Ele percorre toda matriz procurando aresta que ainda nao foi retirada
    bool existeAresta(){
        for(auto i = 0; i < vertice; i++){
            for(auto j = 0; j < vertice; j++){
                if(matriz[i][j] == 1)
                    return true;
            }
        }
        return false;
    }

    //  Dado todos os graus dos vertices ele retorna o indice referente ao valor de maior grau
    //  Caso o valor do maior grau seja zero retorna -1
    //  Poderia ser utilizando como criterio de parada
    long pegarMaiorGrau(){
        int retorno = -1,maior = 0;
        for(long i = 0; i < vertice; i++){
            if(grau[i] > maior && grau[i] != 0){
                maior = grau[i];
                retorno = i;
            }
        }
        return retorno;
    }

    // Funcao que somente imprime a matriz com todos os valores booleano somente na ideia de verificacao do codigo
    void imprimirMarcado(){
        for(long y = 0; y < vertice; y++){
            for(long w = 0; w < vertice; w++){
                cout << marcado[y][w] << " ";
            }
            cout << endl;
        }
    }

    //  Funcao que percorre um novo clique verificando todas as suas posicoes se ja marcadas ou nao
    //  Caso seja marcada somente estaria armanzenando um clique ja existente
    bool verificarCliqueMarcado(list<int> clique ){
        for(auto it : clique){
            for(auto it2 : clique){
                if(it != it2){
                    if(!marcado[it][it2]){
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void marcarNaoSelecionados(){
        for(long y = 0; y < vertice; y++){
            for(long w = 0; w < vertice; w++){
                if(matriz[y][w] == 0)
                    marcado[y][w] = true;
            }
        }
    }

    void execute(){
        // Declaracao de variaveis
        list<int> vizinhos; //  lista aonde vai ficar todos os vertice adjacêntes ao que vai ser verificado
        list<int> cliqueAtual;  //  Possivel clique que esta sendo formado a cada interacao
        vector<int> cliqueAuxiliar; // Um vector auxiliar para armazenamento
        long maiorGrau = 0; // Variavel que recebe o indice do maior grau

        unsigned long long int somatorio = 0;   //  Somatorio das arestas dos clique que foram confirmados, lembrando
                                                //  que esse valor podera ser maior que a quantidade inicial de arestas
                                                //  pois talvez arestas se repente em cliques diferentes para encontrar
                                                //  uma possivel melhor solucao

        // Marca todos aquelas arestas aonde nao existe adjacência
        marcarNaoSelecionados();

        do {    // Inicio do algoritmo
            maiorGrau = pegarMaiorGrau();   //  Recebendo a possivel com maior grau
             if(maiorGrau == -1)    // Possivel caso de encerramento onde todas as arestas ja foram "retiradas"
                break;
            cliqueAtual.push_back(maiorGrau);   // O vertice de maior naquele momento e escolhido para fazer
                                                // para ser analisado entao ele e armazenado aonde formara a possivel
                                                // clique

            // Aqui e adicionado todos os seus vizinhos a possivel clique.
            for (unsigned int j = 0; j < vertice; j++)
                if (matriz[maiorGrau][j] > 0)
                    vizinhos.push_back(j);

            // Se o vertice nao tiver nenhuma adjacência ele salta as proximas execucao retornando a escolha de vertice
            if (vizinhos.empty()) {
                cliqueAtual.clear();    // A possivel clique e limpa
                continue;
            }

            list<int> auxiliar;
            auxiliar.clear();
            // Percorrendo os vizinhos para escolher somentes os nao marcados para analise
            for (list<int>::iterator it = vizinhos.begin(); it != vizinhos.end(); ++it)
                if (!marcado[maiorGrau][*it] && !marcado[*it][maiorGrau])   // Caso nao seja marcado
                    auxiliar.push_back(*it);

            for (list<int>::iterator it = vizinhos.begin(); it != vizinhos.end(); ++it)
                if (marcado[maiorGrau][*it] && marcado[*it][maiorGrau])
                    auxiliar.push_back(*it);

            verificarClique(auxiliar, cliqueAtual); // Verifica se formou clique como aqueles vertices

            if (verificarCliqueMarcado(cliqueAtual)) {  //  Verfica se o clique formado ja nao esta inserido nos
                                                        // selecionado

                // Limpa o vector para armazenar o clique novo
                cliqueAuxiliar.clear();

                //  Percorre o clique que foi pego passando para um vector de armazenamento
                for (list<int>::iterator it = cliqueAtual.begin(); it != cliqueAtual.end(); it++) {
                    cliqueAuxiliar.push_back(*it);
                }

                // Caso o clique auxiliar nao esteja vazio vai ser armazenado
                if (!cliqueAuxiliar.empty()) {
                    // Percorre o clique que vai ser adicionado marcado aquelas arestas que ja foram adicionadas
                    for (unsigned int a = 0; a < cliqueAuxiliar.size(); a++) {
                        for (unsigned int b = 0; b < cliqueAuxiliar.size(); b++)
                            if (a != b) {
                                if(!(marcado[cliqueAuxiliar[a]][cliqueAuxiliar[b]])
                                && !(marcado[cliqueAuxiliar[b]][cliqueAuxiliar[a]])){
                                    //  Caso nao seja ja marcado e retirado um grau de vertice para o proximo pegar o
                                    //  maior grau
                                    grau[cliqueAuxiliar[a]]--;
                                    grau[cliqueAuxiliar[b]]--;
                                }
                                marcado[cliqueAuxiliar[a]][cliqueAuxiliar[b]] = true;
                                marcado[cliqueAuxiliar[b]][cliqueAuxiliar[a]] = true;
                            }
                    }
                    // Insere o clique no armazenamento de todos os cliques
                    clique.push_back(cliqueAuxiliar);
                    //  Usando a formula de quantidade de arestas de grafos completos e adicionado ao somatorio
                    somatorio += cliqueAuxiliar.size() * (cliqueAuxiliar.size() - 1) / 2;
                }
            }

            //  E limpo os conteiner para a proxima execucao
            vizinhos.clear();
            cliqueAtual.clear();


        }while (existeMarcado());
        //  Ordenando os cliques atraves do seu tamanho de k
        sort(clique.begin(),clique.end(),[](auto a, auto b){ return a.size() > b.size();});
    }

    // Funcao que imprime no terminal todos os cliques
    void imprimirCliques(){
        cout << "Todos os cliques encontrados: " << endl;
        cout << "Quantidade de cliques: " << clique.size() << endl;
        for(const auto & it : clique){
            for(auto valor : it) {
                cout << " " << valor;
            }
            cout << ". " << endl;
        }
        cout << "Quantidade de cliques: " << clique.size() << endl;
        //for(int i = 0 ; i < vertice; i++){
        //    cout << "Vertice " << i+ 1 << ": " << grau[i]<< endl;
        //}
    }

    //  Operator << da classe
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
int escreverArquivo(string nome,Grafo* grafo,duration<double>);

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


    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    //gerar a solução aqui
    graph->execute();

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
    cout << "Tempo de execucao: " << time_span.count() << " ms" << endl;

    do{
        cout << "Digite o nome do arquivo e sua extensao ou digite sair:";
        cin >> nome;
        string aux = converterMinusculo(nome);
        if(aux.compare("sair") == 0)
            exit(1);
    }while(escreverArquivo(nome,graph,time_span));

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
    if((file = fopen(nome.c_str(),"r")) == nullptr)
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

int escreverArquivo(string nome,Grafo* grafo,duration<double> tempo){
    // Variavel da funcao
    FILE* file; // Arquivo de leitura

    // Lendo o arquivo e tentando abrir
    if((file = fopen(nome.c_str(),"w")) == nullptr)
        return 1;

    fprintf(file,"Quantidade de cliques encontrada: %ld\n", grafo->clique.size());
    fprintf(file,"Tempo de execucao: %f ms\n", tempo.count());

    unsigned long i = 1,j;
    for(auto it : grafo->clique){
        fprintf(file,"Clique %lu: K%lu\n{", i,it.size());
        j = 1;
        for(auto valor : it) {
            if(j != it.size()){
                fprintf(file,"%d, ", valor + 1);
            }else{
                fprintf(file,"%d", valor + 1);
            }
            j++;
        }
        fprintf(file,"}\n");
        i++;
    }

    fprintf(file,"\nConferindo a matriz\n");
    for(unsigned long w = 0; w < grafo->vertice; w++){
        fprintf(file,"V %lu {", w + 1);
        for(unsigned long y = 0; y < grafo->vertice; y++){
            if(y == grafo->vertice -1){
                fprintf(file,"%d",grafo->marcado[w][y]);
            }else{
                fprintf(file,"%d, ",grafo->marcado[w][y]);
            }
        }
        fprintf(file,"}\n");
    }

    fclose(file);
    return 0;
}
