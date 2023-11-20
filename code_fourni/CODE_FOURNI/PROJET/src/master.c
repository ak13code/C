#if defined HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utils.h"
#include "myassert.h"
#include "client_master.h"
#include "master_worker.h"



// inclure les .h manquants
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/************************************************************************
 * Données persistantes d'un master
 ************************************************************************/
typedef struct
{
    // communication avec le client
    int master_to_client;
    int client_to_master;
    // données internes
    // communication avec le premier worker (double tubes)
    int master_to_worker[2];
    int worker_to_master[2];
    // communication en provenance de tous les workers (un seul tube en lecture)
    //TODO
} Data;


/************************************************************************
 * Usage et analyse des arguments passés en ligne de commande
 ************************************************************************/
static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}


/************************************************************************
 * initialisation complète
 ************************************************************************/
void init(Data *data)
{
    myassert(data != NULL, "il faut l'environnement d'exécution");

    //TODO initialisation data
    pipe(data->master_to_worker);
    pipe(data->worker_to_master);
    
}


/************************************************************************
 * fin du master
 ************************************************************************/
void orderStop(Data *data)
{
    TRACE0("[master] ordre stop\n");
    myassert(data != NULL, "il faut l'environnement d'exécution");

    //TODO
    // - traiter le cas ensemble vide (pas de premier worker)
    // - envoyer au premier worker ordre de fin (cf. master_worker.h)
    // - attendre sa fin
    // - envoyer l'accusé de réception au client (cf. client_master.h)
    //END TODO
}


/************************************************************************
 * quel est la cardinalité de l'ensemble
 ************************************************************************/
void orderHowMany(Data *data)
{
    TRACE0("[master] ordre how many\n");
    myassert(data != NULL, "il faut l'environnement d'exécution");

    //TODO
    // - traiter le cas ensemble vide (pas de premier worker)
    // - envoyer au premier worker ordre howmany (cf. master_worker.h)
    // - recevoir accusé de réception venant du premier worker (cf. master_worker.h)
    // - recevoir résultats (deux quantités) venant du premier worker
    // - envoyer l'accusé de réception au client (cf. client_master.h)
    // - envoyer les résultats au client
    //END TODO
}


/************************************************************************
 * quel est la minimum de l'ensemble
 ************************************************************************/
void orderMinimum(Data *data)
{
    TRACE0("[master] ordre minimum\n");
    myassert(data != NULL, "il faut l'environnement d'exécution");

    //TODO
    // - si ensemble vide (pas de premier worker)
    //       . envoyer l'accusé de réception dédié au client (cf. client_master.h)
    // - sinon
    //       . envoyer au premier worker ordre minimum (cf. master_worker.h)
    //       . recevoir accusé de réception venant du worker concerné (cf. master_worker.h)
    //       . recevoir résultat (la valeur) venant du worker concerné
    //       . envoyer l'accusé de réception au client (cf. client_master.h)
    //       . envoyer le résultat au client
    //END TODO
    float a = 1.0;
    int ret = write(data->master_to_client,&a,sizeof(float));
    myassert( ret != -1 ,"mal ecrit");
}


/************************************************************************
 * quel est la maximum de l'ensemble
 ************************************************************************/
void orderMaximum(Data *data)
{
    TRACE0("[master] ordre maximum\n");
    myassert(data != NULL, "il faut l'environnement d'exécution");

    //TODO
    // cf. explications pour le minimum
    //END TODO
}


/************************************************************************
 * test d'existence
 ************************************************************************/
void orderExist(Data *data)
{
    TRACE0("[master] ordre existence\n");
    myassert(data != NULL, "il faut l'environnement d'exécution");

    //TODO
    // - recevoir l'élément à tester en provenance du client
    // - si ensemble vide (pas de premier worker)
    //       . envoyer l'accusé de réception dédié au client (cf. client_master.h)
    // - sinon
    //       . envoyer au premier worker ordre existence (cf. master_worker.h)
    //       . envoyer au premier worker l'élément à tester
    //       . recevoir accusé de réception venant du worker concerné (cf. master_worker.h)
    //       . si élément non présent
    //             . envoyer l'accusé de réception dédié au client (cf. client_master.h)
    //       . sinon
    //             . recevoir résultat (une quantité) venant du worker concerné
    //             . envoyer l'accusé de réception au client (cf. client_master.h)
    //             . envoyer le résultat au client
    //END TODO
}

/************************************************************************
 * somme
 ************************************************************************/
void orderSum(Data *data)
{
    TRACE0("[master] ordre somme\n");
    myassert(data != NULL, "il faut l'environnement d'exécution");

    //TODO
    // - traiter le cas ensemble vide (pas de premier worker) : la somme est alors 0
    // - envoyer au premier worker ordre sum (cf. master_worker.h)
    // - recevoir accusé de réception venant du premier worker (cf. master_worker.h)
    // - recevoir résultat (la somme) venant du premier worker
    // - envoyer l'accusé de réception au client (cf. client_master.h)
    // - envoyer le résultat au client
    //END TODO
}

/************************************************************************
 * insertion d'un élément
 ************************************************************************/

//TODO voir si une fonction annexe commune à orderInsert et orderInsertMany est justifiée

void orderInsert(Data *data)
{
    //fermer les tubes inutiles
   
    
    close(data->worker_to_master[1]);
    close(data->master_to_worker[0]);

    TRACE0("[master] ordre insertion\n");
    myassert(data != NULL, "il faut l'environnement d'exécution");
    float elt ;
    read(data->client_to_master, &elt, sizeof(float));
    
    
    /*int order ;
    read(data->client_to_master,&order,sizeof(int));
    printf("order dans master est %d\n",order);
    write(data->master_to_worker[1], &order, sizeof(int));*/


    // Recevoir l'accusé de réception du worker
    int acknowledgment;
    read(data->worker_to_master[0], &acknowledgment, sizeof(int));
    // Envoyer l'accusé de réception au client
    write(data->master_to_client, &acknowledgment, sizeof(int));

    //TODO
    // - recevoir l'élément à insérer en provenance du client
    // - si ensemble vide (pas de premier worker)
    //  . créer le premier worker avec l'élément reçu du client
    if(fork() == 0){

        // Envoyer l'élément à insérer au premier worker
        char etchar[100];
        sprintf(etchar, "%f", elt);

        char etchar1[100];
        sprintf(etchar1, "%d",  data->worker_to_master[1]);

        char etchar2[100];
        sprintf(etchar2, "%d", data->master_to_worker[0]);

       //master to tous les workers
        char etchar3[100];
        sprintf(etchar3, "%d", data->worker_to_master[1]);

        char *tenvoyerauworker[6];  // Utilisation d'un tableau de 6 éléments

        tenvoyerauworker[0] = "worker";
        tenvoyerauworker[1] = etchar;//valeur
        tenvoyerauworker[2] = etchar1;//tube read
        tenvoyerauworker[3] = etchar2;//tube à ecrire
        tenvoyerauworker[4] = etchar3;//tube locale
        tenvoyerauworker[5] = NULL;
        // Assurez-vous que le chemin de l'exécutable "worker" est correct
        execv(tenvoyerauworker[0], tenvoyerauworker);
    // si on est là, execv obligatoirement renvoyé -1
    perror("Problème exec");
    }
    else {
    // Code du processus parent (master)
    int status;
    wait(&status); // Attendre que le processus fils (worker) se termine

    if (WIFEXITED(status)) {
        // Le processus fils s'est terminé normalement
        int exit_status = WEXITSTATUS(status);

        if (exit_status == 0) {
            // Actions supplémentaires si le processus fils s'est terminé normalement
            printf("Le processus fils s'est terminé normalement.\n");
        } else {
            // Actions supplémentaires en cas d'erreur lors de l'exécution du processus fils
            fprintf(stderr, "Le processus fils s'est terminé avec un code de sortie non nul : %d\n", exit_status);
        }
    } else {
        // Le processus fils s'est terminé de manière anormale
        // Gérer les erreurs si nécessaire
        fprintf(stderr, "Le processus fils ne s'est pas terminé correctement.\n");
    }
 }

    
    // - sinon
    //       . envoyer au premier worker ordre insertion (cf. master_worker.h)
    //       . envoyer au premier worker l'élément à insérer
    // - recevoir accusé de réception venant du worker concerné (cf. master_worker.h)
    // - envoyer l'accusé de réception au client (cf. client_master.h)
    //END TODO
}


/************************************************************************
 * insertion d'un tableau d'éléments
 ************************************************************************/
void orderInsertMany(Data *data)
{
    TRACE0("[master] ordre insertion tableau\n");
    myassert(data != NULL, "il faut l'environnement d'exécution");

    //TODO
    // - recevoir le tableau d'éléments à insérer en provenance du client
    // - pour chaque élément du tableau
    //       . l'insérer selon l'algo vu dans orderInsert (penser à factoriser le code)
    // - envoyer l'accusé de réception au client (cf. client_master.h)
    //END TODO

    
}


/************************************************************************
 * affichage ordonné
 ************************************************************************/
void orderPrint(Data *data)
{
    TRACE0("[master] ordre affichage\n");
    myassert(data != NULL, "il faut l'environnement d'exécution");

    //TODO
    // - traiter le cas ensemble vide (pas de premier worker)
    // - envoyer au premier worker ordre print (cf. master_worker.h)
    // - recevoir accusé de réception venant du premier worker (cf. master_worker.h)
    //   note : ce sont les workers qui font les affichages
    // - envoyer l'accusé de réception au client (cf. client_master.h)
    //END TODO
}


/************************************************************************
 * boucle principale de communication avec le client
 ************************************************************************/
void loop(Data *data)
{
    bool end = false;
    init(data);
    
    while (!end)
    {
        //TODO ouverture des tubes avec le client (cf. explications dans client.c)
          data->client_to_master = open("client_to_master",O_RDONLY); 
          int order ;
          read(data->client_to_master,&order,sizeof(int));
          printf(" order dans wile loop est %d",order);
          data->master_to_client = open("master_to_client",O_WRONLY); 
        
          //TODO pour que ça ne boucle pas, mais recevoir l'ordre du client
        switch(order)
        {
          case CM_ORDER_STOP:
            orderStop(data);
            end = true;
            break;
          case CM_ORDER_HOW_MANY:
            orderHowMany(data);
            break;
          case CM_ORDER_MINIMUM:
            orderMinimum(data);
            break;
          case CM_ORDER_MAXIMUM:
            orderMaximum(data);
            break;
          case CM_ORDER_EXIST:
            orderExist(data);
            break;
          case CM_ORDER_SUM:
            orderSum(data);
            break;
          case CM_ORDER_INSERT:
            orderInsert(data);
            break;
          case CM_ORDER_INSERT_MANY:
            orderInsertMany(data);
            break;
          case CM_ORDER_PRINT:
            orderPrint(data);
            break;
          default:
            myassert(false, "ordre inconnu");
            exit(EXIT_FAILURE);
            break;
        }

        //TODO fermer les tubes nommés
        //     il est important d'ouvrir et fermer les tubes nommés à chaque itération
        //     voyez-vous pourquoi ?
        //TODO attendre ordre du client avant de continuer (sémaphore pour une précédence)

        close(data->client_to_master);
        close(data->master_to_client);
        TRACE0("[master] fin ordre\n");
    }
}


/************************************************************************
 * Fonction principale
 ************************************************************************/

//TODO N'hésitez pas à faire des fonctions annexes ; si les fonctions main
//TODO et loop pouvaient être "courtes", ce serait bien

int main(int argc, char * argv[])
{
    if (argc != 1)
        usage(argv[0], NULL);

    TRACE0("[master] début\n");

    Data data;

    //TODO
    // - création des sémaphores
    // - création des tubes nommés
   
    int res = mkfifo("master_to_client",0644);
    myassert(res != -1 ,"toto");
    int ret = mkfifo("client_to_master",0644);
    myassert(ret != -1,"toto");

    loop(&data);
    
    //END TODO
  

    //TODO destruction des tubes nommés, des sémaphores, ...
    unlink("client_to_master");
    unlink("master_to_client");
    
    

    TRACE0("[master] terminaison\n");
    return EXIT_SUCCESS;
}
