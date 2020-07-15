#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "defines.h"

static void add_admin(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[5];

	char username[45];
	char password[45];
    char passwordtmp[45];

    printf("\033[2J\033[H");
    printf("Aggiungi admin\n");

	// Get the required information
    printf("\nUsername: ");
	getInput(45, username, false);
	printf("Password: ");
	getInput(45, password, true);
	printf("Retype password: ");
	getInput(45, passwordtmp, true);

    if(strcmp(password, passwordtmp)) {
        finish_with_error(conn, "Le password non sono uguali");
    }
 
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_admin(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = username;
	param[0].buffer_length = strlen(username);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = password;
	param[1].buffer_length = strlen(password);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nell'aggiunta dell'admin.");
	} else {
		printf("Admin %s aggiunto correttamente\n", username);
	}

	mysql_stmt_close(prepared_stmt);
}

void admin(MYSQL *conn) {
    char command[20];

    printf("\033[2J\033[H");
    
    if(mysql_change_user(conn, "admin", "admin", "biblioteca")) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

    printf("Connected as admin\n");
    while(1) {

        printf("admin$ ");
        getInput(20, command, false);

        if(!strcmp(command, "quit")) {
            printf("Uscita...\n");
            return;
        } else if(!strcmp(command, "help")) {
            printf("*** Admin commands ***\n");
            printf("addadmin - per aggiungere un admin\n");
            printf("addlibrary- per aggiungere una biblioteca\n");
            printf("addtime - per aggiungere un orario di una biblioteca\n");
            printf("addturn - per aggiungere un turno\n");
            printf("setturn - per assegnare un turno per un mese\n");
            printf("setsturn - per aggiungere un turno singolo\n");
            printf("accepttransfer - per accettare un trasferimento\n");
            printf("rejecttransfer - per rifiutare una richiesta di transferimento\n");
            printf("showrequests - per vedere le richieste di trasferimento\n");
            printf("avblelib - per vedere bibliotecari disponibili\n");
            printf("dcopies - per dismettere le copie\n");
            printf("rsick - per mettere in malattia un bibliotecario\n");
            printf("checkcoverage - per verificare la copertura per una biblioteca\n");
            printf("quit - per uscire dall'applicazione\n");
            printf("clear - per pulire lo schermo\n");
        }  else if(!strcmp(command, "clear")) {
            printf("\033[2J\033[H");
        }  else if(!strcmp(command, "addadmin")) {
            add_admin(conn);
        } 
    }
}