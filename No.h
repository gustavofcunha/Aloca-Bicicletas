#ifndef NO_H
#define NO_H

class No{
    public:
        No(){}
        No(int x, int y, int distanciaOrigem);
        std::pair<int, int> localizacao;
        int distanciaOrigem;
};

/**
* -Descricao: construtor padrao
* -Entrada: N/A
* -Saida: N/A
*/
No::No(int x, int y, int _distanciaOrigem){
    localizacao.first = x;
    localizacao.second = y;
    _distanciaOrigem = _distanciaOrigem;
}


#endif