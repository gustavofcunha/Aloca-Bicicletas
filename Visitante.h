#ifndef VISITANTE_H
#define VISITANTE_H

#include <deque>
#include "Bicicleta.h"

typedef struct{
    int preferencia;
    Bicicleta *bicicleta;
} Preferencia;

class Visitante{
    public:
        Visitante();
        int id;
        int idBike;
        std::pair<int,int> localizacao;
        std::deque<Preferencia> listaPreferencias;
        char alfaId(int id){
            return 97 + id;
        }
};

/**
* -Descricao: construtor padrao
* -Entrada: N/A
* -Saida: N/A
*/
Visitante::Visitante(){
    id = 0;
    idBike = -1;
    localizacao.first = -1;
    localizacao.second = -1;
}

#endif