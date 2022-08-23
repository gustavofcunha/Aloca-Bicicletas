#ifndef BICICLETA_H
#define BICICLETA_H

class Bicicleta{
    public:
        Bicicleta();
        int id;
        std::pair<int,int> localizacao;
        int idVis;
};

/**
* -Descricao: construtor padrao
* -Entrada: N/A
* -Saida: N/A
*/
Bicicleta::Bicicleta(){
    idVis = -1;
    localizacao.first = -1;
    localizacao.second = -1;
}

#endif