#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "defines.h"

static void add_admin(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];

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

static void add_library(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[7];

	int id;
	char name[45];
    char city[45];
    char street[45];
    char number[45];
    char resp[45];
    char telephone[12];

    printf("\033[2J\033[H");
    printf("Aggiungi biblioteca\n");

	// Get the required information
    printf("\nCodice: ");
	getInteger(&id);
	printf("Nome: ");
	getInput(45, name, false);
    printf("Citta: ");
	getInput(45, city, false);
	printf("Via: ");
	getInput(45, street, false);
    printf("Numero: ");
	getInput(45, number, false);
    printf("Responsabile: ");
	getInput(45, resp, false);
    printf("Telefono: ");
	getInput(12, telephone, false);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_biblioteca(?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = name;
	param[1].buffer_length = strlen(name);

    param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = city;
	param[2].buffer_length = strlen(city);

    param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = street;
	param[3].buffer_length = strlen(street);

    param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = number;
	param[4].buffer_length = strlen(number);

    param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[5].buffer = resp;
	param[5].buffer_length = strlen(resp);

    param[6].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[6].buffer = telephone;
	param[6].buffer_length = strlen(telephone);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nell'aggiunta della biblioteca.");
	} else {
		printf("Biblioteca %d aggiunta correttamente\n", id);
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_time(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[4];

    //IN var_biblioteca INT, IN var_giorno VARCHAR(15), IN var_inizio TIME, IN var_fine TIME

	int id;
	char day[15];
    MYSQL_TIME start;
    MYSQL_TIME end;

    printf("\033[2J\033[H");
    printf("Aggiungi orario di una biblioteca\n");

	// Get the required information
    printf("\nCodice: ");
	getInteger(&id);
	printf("Giorno: ");
	getInput(13, day, false);

//TODO FIX NEGATIVE TIME

    printf("Inserisci orario di apertura(formato xx:xx): ");
    scanf("%u:%u",&start.hour, &start.minute);

    printf("Inserisci orario di chiusura(formato xx:xx): ");
    scanf("%u:%u",&end.hour, &end.minute);
    getchar();

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_orario(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = day;
	param[1].buffer_length = strlen(day);

    param[2].buffer_type = MYSQL_TYPE_TIME;
	param[2].buffer = &start;
	param[2].buffer_length = sizeof(start);

    param[3].buffer_type = MYSQL_TYPE_TIME;
	param[3].buffer = &end;
	param[3].buffer_length = sizeof(end);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nell'aggiunta dell'orario.");
	} else {
		printf("Orario per la biblioteca %d aggiunto correttamente\n", id);
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_turn(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[5];

    //IN var_codice INT, IN var_biblioteca INT, IN var_giorno VARCHAR(15), IN var_inizio TIME, IN var_fine TIME

	int id;
    int idLib;
	char day[15];
    MYSQL_TIME start;
    MYSQL_TIME end;

    printf("\033[2J\033[H");
    printf("Aggiungi turno di una biblioteca\n");

	// Get the required information
    printf("\nCodice Turno: ");
	getInteger(&id);
    printf("Codice Biblioteca: ");
	getInteger(&idLib);
	printf("Giorno: ");
	getInput(13, day, false);

    printf("Inserisci orario di inizio(formato xx:xx): ");
    scanf("%u:%u",&start.hour, &start.minute);

    printf("Inserisci orario di fine(formato xx:xx): ");
    scanf("%u:%u",&end.hour, &end.minute);
    getchar();

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_turno(?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);

    param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &idLib;
	param[1].buffer_length = sizeof(idLib);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = day;
	param[2].buffer_length = strlen(day);

    param[3].buffer_type = MYSQL_TYPE_TIME;
	param[3].buffer = &start;
	param[3].buffer_length = sizeof(start);

    param[4].buffer_type = MYSQL_TYPE_TIME;
	param[4].buffer = &end;
	param[4].buffer_length = sizeof(end);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nell'aggiunta del turno.");
	} else {
		printf("Turno %d per la biblioteca %d aggiunto correttamente\n", id, idLib);
	}

	mysql_stmt_close(prepared_stmt);
}

static void show_request(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];

    int id;

    printf("\033[2J\033[H");
    printf("Vedi richieste di trasferimento per una biblioteca\n");

	// Get the required information
    printf("\nCodice: ");
	getInteger(&id);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call vedi_richieste(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella ricerca delle richieste di trasferimento.");
	}

	dump_result_set(conn, prepared_stmt, "Lista richieste di trasferimenti");

	mysql_stmt_close(prepared_stmt);
}

static void accept_transfer(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

    int idBook;
    int idLib;

    printf("\n");
    printf("Accetta trasferimento\n");

	// Get the required information
    printf("\nCodice Copia: ");
	getInteger(&idBook);
    printf("\nCodice Biblioteca: ");
	getInteger(&idLib);

	// Prepare stored procedure call
    //TODO remove WIP
	if(!setup_prepared_stmt(&prepared_stmt, "call accetta_richiesta(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &idBook;
	param[0].buffer_length = sizeof(idBook);

    param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &idLib;
	param[1].buffer_length = sizeof(idLib);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella accettazione del trasferimento.");
	} else {
        printf("Trasferimento avvenuto con successo\n");
    }

	mysql_stmt_close(prepared_stmt);
}

static void reject_transfer(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

    int idBook;
    int idLib;

    printf("\n");
    printf("Rifiuta trasferimento\n");

	// Get the required information
    printf("\nCodice Copia: ");
	getInteger(&idBook);
    printf("\nCodice Biblioteca: ");
	getInteger(&idLib);

	// Prepare stored procedure call
    //TODO remove WIP
	if(!setup_prepared_stmt(&prepared_stmt, "call rifiuta_richiesta(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &idBook;
	param[0].buffer_length = sizeof(idBook);

    param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &idLib;
	param[1].buffer_length = sizeof(idLib);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nel rifiuto del trasferimento.");
	} else {
        printf("Trasferimento rifiutato\n");
    }

	mysql_stmt_close(prepared_stmt);
}

static void dismiss_copies(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];

    int id;

    printf("\n");
    printf("Dismetti Copie\n");

	// Get the required information
    printf("\nCodice Biblioteca: ");
	getInteger(&id);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call dismetti_copie(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nel dismettere le copie.");
	} else {
        printf("Copie dismesse.\n");
    }

	mysql_stmt_close(prepared_stmt);
}

void admin(MYSQL *conn, char *username) {
    char command[20];
    printf("\033[2J\033[H");
    
    if(mysql_change_user(conn, "admin", "admin", "biblioteca")) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

    printf("Connected as admin\n");
    while(1) {

        printf("%s-admin$ ", username);
        getInput(20, command, false);

        if(!strcmp(command, "quit")) {
            printf("Uscita...\n");
            return;
        } else if(!strcmp(command, "help")) {
            printf("*** Admin commands ***\n");
            printf("addadmin - per aggiungere un admin\n");
            printf("addlibrary- per aggiungere una biblioteca\n");
            printf("WIPaddtime - per aggiungere un orario di una biblioteca\n");
            printf("addturn - per aggiungere un turno\n");
            printf("Xsetturn - per assegnare un turno per un mese\n");
            printf("Xsetsturn - per aggiungere un turno singolo\n");
            printf("accepttransfer - per accettare un trasferimento\n");
            printf("rejecttransfer - per rifiutare una richiesta di transferimento\n");
            printf("showrequest - per vedere le richieste di trasferimento\n");
            printf("Xavblelib - per vedere bibliotecari disponibili\n");
            printf("dcopies - per dismettere le copie\n");
            printf("Xrsick - per mettere in malattia un bibliotecario\n");
            printf("Xcheckcoverage - per verificare la copertura per una biblioteca\n");
            printf("quit - per uscire dall'applicazione\n");
            printf("clear - per pulire lo schermo\n");
        }  else if(!strcmp(command, "clear")) {
            printf("\033[2J\033[H");
        }  else if(!strcmp(command, "addadmin")) {
            add_admin(conn);
        }  else if(!strcmp(command, "addtime")) {
            add_time(conn);
        }  else if(!strcmp(command, "addlibrary")) {
            add_library(conn);
        }  else if(!strcmp(command, "showrequest")) {
            show_request(conn);
        }  else if(!strcmp(command, "accepttransfer")) {
            accept_transfer(conn);
        }  else if(!strcmp(command, "rejecttransfer")) {
            reject_transfer(conn);
        }  else if(!strcmp(command, "rejecttransfer")) {
            reject_transfer(conn);
        }  else if(!strcmp(command, "dcopies")) {
            dismiss_copies(conn);
        }  else if(!strcmp(command, "addturn")) {
            add_turn(conn);
        } else {
            printf("comando %s non riconosciuto, digita help per aiuto\n", command);
        }
    }
}
