#include <iostream>
#include <string>
#include <algorithm>
#include "msgassert.h"
#include "Visitante.h"
#include "No.h"

#define INFINITO 99999999

/**
* -Descricao: funcao auxiliar do metodo sort
* -Entrada: a, b
* -Saida: true, se a deve preceder b
*         false, caso contrario
*/
bool ordenaBicicletas(Preferencia a, Preferencia b){
    if(a.preferencia > b.preferencia){
        return true;
    }
    else if(a.preferencia == b.preferencia){
        if(a.bicicleta->id < b.bicicleta->id){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

/**
* -Descricao: verifica se ha visitantes desalocados e que ainda tem pretendentes
* -Entrada: visitantes[], V
* -Saida: true, se sim
*         false, se nao
*/
Visitante* temDesalocadoePretendentes(Visitante *visitantes, int V){
    int i;

    Visitante *dft;
    dft = new Visitante();
    dft->id = -1;

    for(i=0; i<V; i++){
        if(visitantes[i].idBike == -1 && 
           !visitantes[i].listaPreferencias.empty()){
               return &visitantes[i];
        }
    }

    return dft;
}

/**
* -Descricao: calcula distancia entre uma bicicleta e um visitante, considerando
*  que se podem fazer movimentos apenas na verticar e horizontal, 
*  utilizando o algoritmo bfs
* -Entrada: vis, bike, mapa, N, M
* -Saida: distancia
*/
int calculaDistancia(Visitante vis, Bicicleta bike, char **mapa, int N, int M){
    int i,j;
    No aux = No(0,0,0);
    No aux2 = No(0,0,0);
    //converte o id da bike em char
    char idBikeChar = 48 + bike.id;

    //matriz de controle de visitacao das celulas do mapa
    bool explorado[N][M];
    for(i=0; i<N; i++){
        for(j=0; j<M; j++){
            if(mapa[i][j] == '-'){
                explorado[i][j] = true;
            }
            else{
                explorado[i][j] = false;
            }
            
        }
    }
    
    //inicializacao do no correspondente ao visitante, de onde começa a BFS
    No visitante;
    visitante.localizacao.first = vis.localizacao.first;
    visitante.localizacao.second = vis.localizacao.second;
    visitante.distanciaOrigem = 0;
    explorado[visitante.localizacao.first][visitante.localizacao.second] = true;

    //inicializacao de um deque para algoritmo bfs
    std::deque<No> camadas;
    camadas.clear();
    camadas.push_back(visitante);

    while(!camadas.empty()){
        aux = camadas.front();
        camadas.pop_front();

        if(mapa[aux.localizacao.first][aux.localizacao.second] == idBikeChar){
            /*std::cout << std::endl << "Distancia calculada entre visitante " << vis.alfaId(vis.id) <<
            " e bicicleta " << bike.id << ": " << aux.distanciaOrigem << std::endl;*/
            return aux.distanciaOrigem;
        }

        if(aux.localizacao.first - 1 >= 0 &&
           explorado[aux.localizacao.first - 1][aux.localizacao.second] == false){
            
            aux2.localizacao.first = aux.localizacao.first -1;
            aux2.localizacao.second = aux.localizacao.second;
            aux2.distanciaOrigem =  aux.distanciaOrigem + 1;

            camadas.push_back(aux2);
            explorado[aux.localizacao.first - 1][aux.localizacao.second] = true;
        }

        if(aux.localizacao.first + 1 < N && 
           explorado[aux.localizacao.first + 1][aux.localizacao.second] == false){

             aux2.localizacao.first = aux.localizacao.first + 1;
            aux2.localizacao.second = aux.localizacao.second;
            aux2.distanciaOrigem =  aux.distanciaOrigem + 1;

            camadas.push_back(aux2);
            explorado[aux.localizacao.first + 1][aux.localizacao.second] = true;
        }

        if(aux.localizacao.second - 1 >= 0 && 
           explorado[aux.localizacao.first][aux.localizacao.second - 1] == false){

            aux2.localizacao.first = aux.localizacao.first;
            aux2.localizacao.second = aux.localizacao.second - 1;
            aux2.distanciaOrigem =  aux.distanciaOrigem + 1;

            camadas.push_back(aux2);
            explorado[aux.localizacao.first][aux.localizacao.second - 1] = true;
        }

        if(aux.localizacao.second + 1 < M && 
           explorado[aux.localizacao.first][aux.localizacao.second + 1] == false){

            aux2.localizacao.first = aux.localizacao.first;
            aux2.localizacao.second = aux.localizacao.second + 1;
            aux2.distanciaOrigem =  aux.distanciaOrigem + 1;

            camadas.push_back(aux2);
            explorado[aux.localizacao.first][aux.localizacao.second + 1] = true;
        }
    }
    return INFINITO;
}

/**
* -Descricao: funcao que executa o algoritmo de Gale Shapley para fazer
*  os casamentos estaveis entre visitantes e bicicletas
* -Entrada: visitantes[], V, N, M, mapa
* -Saida: N/A
*/
void galeShapley(Visitante *visitantes, int V, int N, int M, char** mapa){   
    int a,b; 
    //visitante desalocado e ainda com bicicletas pretendentes               
    Visitante* vis = temDesalocadoePretendentes(visitantes, V);

    while(vis->id != -1){
        //primeira bicicleta na lista de preferencias de vis
        Bicicleta *bike = vis->listaPreferencias.front().bicicleta;

        //caso a bike ja esteja alocada, as distancias de interesses 
        //sao calculadas e armazenadas, para que nao sejam calculadas 
        //mais de uma vez
        if(bike->idVis != -1){
            a = calculaDistancia(*vis, *bike, mapa, N, M);
            b = calculaDistancia(visitantes[bike->idVis], *bike, mapa, N, M);
        }
        
        //se a bicicleta estiver livre
        if(bike->idVis == -1){
            //alocacao do par vis-bike
            bike->idVis = vis->id;
            vis->idBike = bike->id;

            //bike removida da lista de preferencia de vis
            vis->listaPreferencias.pop_front();
        }

        //se a bicicleta ja estiver alocada, mas a distancia de vis ate ela
        //eh menor que a distancia de seu visitante alocado
        else if(a < b){
            //desfazendo antigo par
            visitantes[bike->idVis].idBike = -1;

            //alocacao do novo par
            bike->idVis = vis->id;
            vis->idBike = bike->id;

            //bike removida da lista de preferencia de vis
            vis->listaPreferencias.pop_front();
        }

        //desempate por distancia, ganha o visitante com menor id
        else if(a == b && vis->id < bike->idVis){
            //desfazendo antigo par
            visitantes[bike->idVis].idBike = -1;

            //alocacao do novo par
            bike->idVis = vis->id;
            vis->idBike = bike->id;

            //bike removida da lista de preferencia de vis
            vis->listaPreferencias.pop_front();
        }

        //caso contrario, apenas desconsidera aquela bicicleta
        else{
            //bike removida da lista de preferencia de vis
            vis->listaPreferencias.pop_front();  
        }

        vis = temDesalocadoePretendentes(visitantes, V);
    }
}

/**
* -Descricao: retorna coordernadas de uma bicicleta no mapa
* -Entrada: id, N, M, mapa
* -Saida: coordenadas (par ordenado)
*/
std::pair<int,int> encontraBicicleta(int id, int N, int M, char **mapa){
    int i, j;
    std::string aux;
    std::pair<int, int> coordenadas;
    for(i=0; i<N; i++){
        for(j=0; j<M; j++){
            //se o caractere for um numero
            if(mapa[i][j] >= 48 && mapa[i][j] <= 57){
                aux = mapa[i][j];
                if(stoi(aux) == id){
                    coordenadas = std::make_pair(i, j);
                    return coordenadas;
                }
            }
        }
    }

    erroAssert(false, "Bicicleta nao encontrada.");
    return std::make_pair(-1,-1);
}

/**
* -Descricao: retorna coordernadas de um visitante no mapa
* -Entrada: id, N, M, mapa
* -Saida: coordenadas (par ordenado)
*/
std::pair<int,int> encontraVisitante(char id, int N, int M, char **mapa){
    int i, j;
    std::string aux;
    std::pair<int, int> coordenadas;
    for(i=0; i<N; i++){
        for(j=0; j<M; j++){
            //se o caractere for uma letra
            if(mapa[i][j] >= 97 && mapa[i][j] <= 122){
                if(mapa[i][j] == id){
                    coordenadas = std::make_pair(i, j);
                    return coordenadas;
                }
            }
        }
    }

    erroAssert(false, "Visitante nao encontrado.");
    return std::make_pair(-1,-1);
}

/**
* -Descricao: funcao principal: abertura, processamento e fechamento
*  do arquivos de entrada, instanciacao das entidades envolvidas
*  e chamada dos metodos para resolucao do problema
* -Entrada: argc, argv
* -Saida: 0
*/
int main(){
    int V, N, M, i,j;
    std::string buf, aux;

    //processando qtde de visitante/bicicletas
    getline(std::cin, buf);
    V = stoi(buf);

    Visitante visitantes[V];

    //processando dimensoes do mapa
    getline(std::cin, buf);
    std::size_t pos = buf.find_first_of(" ");
    aux = buf;
    N = stoi(buf.erase(pos));
    aux = aux.substr(pos+1);
    M = stoi(aux);

    //alocacao dinamica de matriz para o mapa
    char **mapa;
    mapa = new char* [N];
    for(i=0; i<M; i++){ 
        mapa[i] = new char[M];
    }

    //processando mapa
    for(i=0; i<N; i++){
        getline(std::cin, buf);
        for(j=0; j<M; j++){
            mapa[i][j] = buf[j];
        }
    }

    //vetor para bicicletas
    Bicicleta bicicletas[V];

    //processando listas de preferencias
    for(i=0; i<V; i++){
        visitantes[i].id = i;
        visitantes[i].localizacao = 
        encontraVisitante(visitantes[i].alfaId(visitantes[i].id), N, M, mapa);
        
        getline(std::cin, buf);
        for(j=0; j<V; j++){
            //processando as notas da lista de preferencia
            pos = buf.find_first_of(" ");

            //se nao for o ultimo valor de buf, 
            //buf eh cortado de maneira a pegar apenas o primeiro valor
            if(pos < buf.size()){
                aux = buf;
                buf.erase(pos);
                aux = aux.substr(pos+1);
            }

            Preferencia *pref;
            pref = new Preferencia;

            pref->preferencia = stoi(buf);
            pref->bicicleta = &bicicletas[j];
            pref->bicicleta->id = j;

            //se a localizacao da bicicleta ainda nao tiver sido calculada, calcule-a
            if(pref->bicicleta->localizacao.first == -1){
                pref->bicicleta->localizacao = encontraBicicleta(pref->bicicleta->id, N, M, mapa);
            }
            
            visitantes[i].listaPreferencias.push_back(*pref);
            delete(pref);

            buf = aux;
        }
    }

    //ordenacao das listas de preferencia
    for(i=0; i<V; i++){
        std::sort(visitantes[i].listaPreferencias.begin(), 
                  visitantes[i].listaPreferencias.end(), ordenaBicicletas);
    }

    galeShapley(visitantes, V, N, M, mapa);

    //impressao padrao de resultados
    for(i=0; i<V; i++){
        std::cout << std::endl << visitantes[i].alfaId(visitantes[i].id) 
        << " " << visitantes[i].idBike << std::endl;
    }

    //desalocacao da matriz dinamica mapa
    for(i=0; i<M; i++){ 
        delete(mapa[i]);
    }
    delete(mapa);

    return 0;
}