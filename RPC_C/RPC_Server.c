// Sammaritani Gloria 000072439
// RPC_Server.c

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <rpc/rpc.h>
#include "RPC_xFile.h"

#define NUM 6

// TODO DEFINIZIONE STRUTTURA DATI

static int inizializzato = 0;

// TODO STAMPA

void inizializza() {
    if (inizializzato == 1)
        return;

    // TODO INIZIALIZZAZIONE STRUTTURA DATI

    inizializzato = 1;
    printf("Terminata inizializzazione struttura dati!\n");
}

// TODO IMPLEMENTAZIONE FUNZIONI
