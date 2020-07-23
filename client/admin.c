#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "defines.h"

char command[20];

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
	if(getInput(45, username, false) < 0) {
		return;
	}
	
	printf("Password: ");
	if(getInput(45, password, true) < 0) {
		return;
	}

	if(printf("Ripeti password: ") < 0) {
		return;
	}
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

    printf("Aggiungi biblioteca\n");

	// Get the required information
    printf("\nCodice: ");
	if(getInteger(&id) < 0){
		return;
	}

	printf("Nome: ");
	if(getInput(45, name, false) < 0){
		return;
	}

    printf("Citta: ");
	if(getInput(45, city, false) < 0){
		return;
	}

	printf("Via: ");
	if(getInput(45, street, false) < 0){
		return;
	}

    printf("Numero: ");
	if(getInput(45, number, false) < 0){
		return;
	}

    printf("Responsabile: ");
	if(getInput(45, resp, false) < 0){
		return;
	}
	
    printf("Telefono: ");
	if(getInput(12, telephone, false) < 0){
		return;
	}

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

static void add_librarian(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[8];

	char cf[16];
    char name[45];
    char surname[45];
    char pob[45];	//Place of birth
    MYSQL_TIME *date;	//Date of birth
	date = malloc(sizeof(MYSQL_TIME));
	char title[45];
    char username[45];
	char password[45];
	char passwordtmp[45];

    printf("Aggiungi bibliotecario\n");

	// Get the required information
	printf("CF: ");
	if(getInput(16, cf, false) < 0) {
		return;
	}

    printf("Nome: ");
	if(getInput(45, name, false) < 0) {
		return;
	}

	printf("Cognome: ");
	if(getInput(45, surname, false) < 0) {
		return;
	}

    printf("Luogo di nascita: ");
	if(getInput(45, pob, false) < 0) {
		return;
	}
    
	printf("Inserisci data di nascita(formato dd/mm/yyyy): ");
	if(getDate(date) < 0){
		return;
	}

	printf("Titolo di studio: ");
	if(getInput(45, title, false) < 0) {
		return;
	}

	printf("Username: ");
	if(getInput(45, username, false) < 0) {
		return;
	}

	printf("Password: ");
	if(getInput(45, password, true) < 0) {
		return;
	}

	printf("Ripeti password: ");
	if(getInput(45, passwordtmp, true) < 0) {
		return;
	}

    if(strcmp(password, passwordtmp)) {
        printf("Le passowod non corrispondono\n");
		return;
    }

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_bibliotecario(?, ?, ?, ?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));


	/*`aggiungi_bibliotecario` (IN var_CF VARCHAR(16), IN var_nome VARCHAR(45), 
IN var_cognome VARCHAR(45), IN var_luogo_nascita VARCHAR(45), 
IN var_data_nascita DATE, IN var_titolo_studio VARCHAR(45), IN var_username VARCHAR(45), IN var_password CHAR(45))*/

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = cf;
	param[0].buffer_length = strlen(cf);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = name;
	param[1].buffer_length = strlen(name);

    param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = surname;
	param[2].buffer_length = strlen(surname);

    param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = pob;
	param[3].buffer_length = strlen(pob);

    param[4].buffer_type = MYSQL_TYPE_DATE;
	param[4].buffer = date;
	param[4].buffer_length = sizeof(*date);

    param[5].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[5].buffer = title;
	param[5].buffer_length = strlen(title);

    param[6].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[6].buffer = username;
	param[6].buffer_length = strlen(username);

	param[7].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[7].buffer = password;
	param[7].buffer_length = strlen(password);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nell'aggiunta del bibliotecario.");
	} else {
		printf("Bibliotecario %s aggiunto correttamente\n", username);
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_time(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[4];

    //IN var_biblioteca INT, IN var_giorno VARCHAR(15), IN var_inizio TIME, IN var_fine TIME

	int id;
	char day[15];
    MYSQL_TIME *start;
    MYSQL_TIME *end;

    start = malloc(sizeof(MYSQL_TIME));
    end = malloc(sizeof(MYSQL_TIME));

    printf("\033[2J\033[H");
    printf("Aggiungi orario di una biblioteca\n");

	// Get the required information
    printf("\nCodice: ");
	if(getInteger(&id) < 0) {
		return;
	}

	printf("Giorno: ");
	if(getInput(13, day, false) < 0) {
		return;
	}

    printf("Inserisci orario di apertura(formato xx:xx): ");
    if(getTime(start) < 0) {
		return;
	}

    printf("Inserisci orario di chiusura(formato xx:xx): ");
    if(getTime(end) < 0) {
		return;
	}
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
	param[2].buffer = start;
	param[2].buffer_length = sizeof(*start);

    param[3].buffer_type = MYSQL_TYPE_TIME;
	param[3].buffer = end;
	param[3].buffer_length = sizeof(*end);

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
    MYSQL_TIME *start;
    MYSQL_TIME *end;

    start = malloc(sizeof(MYSQL_TIME));
    end = malloc(sizeof(MYSQL_TIME));

    printf("\033[2J\033[H");
    printf("Aggiungi turno di una biblioteca\n");

	// Get the required information
    printf("\nCodice Turno: ");
	if(getInteger(&id) < 0) {
		return;
	}

    printf("Codice Biblioteca: ");
	if(getInteger(&idLib) < 0){
		return;
	}

	printf("Giorno: ");
	if(getInput(13, day, false) < 0){
		return;
	}

    printf("Inserisci orario di inizio(formato xx:xx): ");
    if(getTime(start) < 0) {
		return;
	}

    printf("Inserisci orario di fine(formato xx:xx): ");
    if(getTime(end) < 0) {
		return;
	}
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
	param[3].buffer = start;
	param[3].buffer_length = sizeof(*start);

    param[4].buffer_type = MYSQL_TYPE_TIME;
	param[4].buffer = end;
	param[4].buffer_length = sizeof(*end);

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

static void set_turn(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];

    //assegna_turnoWIP` (IN var_codice_turno INT, IN var_cf VARCHAR(16), IN var_biblioteca INT)

	int idTurn;
	char cf[16];
    int idLib;

    printf("Assegna turno per il mese\n");

	// Get the required information
    printf("\nCodice Turno: ");
	if(getInteger(&idTurn) < 0){
		return;
	}

    printf("CF Bibliotecario: ");
	if(getInput(16, cf, false) < 0){
		return;
	}

    printf("Codice Biblioteca: ");
	if(getInteger(&idLib) < 0){
		return;
	}

	// Prepare stored procedure call
    //TODO Remove WIP
	if(!setup_prepared_stmt(&prepared_stmt, "call assegna_turnoWIP(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &idTurn;
	param[0].buffer_length = sizeof(idTurn);

    param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = cf;
	param[1].buffer_length = strlen(cf);


    param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &idLib;
	param[2].buffer_length = sizeof(idLib);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella assegnazione del turno.");
	} else {
		printf("Bibliotecario %s assegnato al turno %d-%d per il mese.\n", cf, idTurn, idLib);
	}

	mysql_stmt_close(prepared_stmt);
}

static void set_single_turn(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[4];

	int idTurn;
	char cf[16];
    int idLib;
    MYSQL_TIME *date;
	date = malloc(sizeof(MYSQL_TIME));

    printf("Assegna turno per il mese\n");

	// Get the required information
    printf("\nCodice Turno: ");
	if(getInteger(&idTurn) < 0){
		return;
	}

    printf("CF Bibliotecario: ");
	if(getInput(16, cf, false) < 0){
		return;
	}

    printf("Codice Biblioteca: ");
	if(getInteger(&idLib) < 0){
		return;
	}


    printf("Inserisci data turno(formato dd/mm/yyyy): ");
    if(getDate(date) < 0) {
		return;
	}

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call assegna_turno_singolo(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

    param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &idLib;
	param[0].buffer_length = sizeof(idLib);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &idTurn;
	param[1].buffer_length = sizeof(idTurn);

    param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = cf;
	param[2].buffer_length = strlen(cf);

    param[3].buffer_type = MYSQL_TYPE_DATE;
	param[3].buffer = date;
	param[3].buffer_length = sizeof(*date);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella assegnazione del turno.");
	} else {
		printf("Bibliotecario %s assegnato al turno %d-%d per il giorno %d.\n", cf, idTurn, idLib, date->day);
	}

	mysql_stmt_close(prepared_stmt);
}

static void show_request(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];

    int id;

    printf("Vedi richieste di trasferimento per una biblioteca\n");

	// Get the required information
    printf("\nCodice: ");
	if(getInteger(&id) < 0){
		return;
	}

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
	if(getInteger(&idBook) < 0){
		return;
	}

    printf("Codice Biblioteca: ");
	if(getInteger(&idLib) < 0) {
		return;
	}

	// Prepare stored procedure call
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
	if(getInteger(&idBook) < 0) {
		return;
	}

    printf("\nCodice Biblioteca: ");
	if(getInteger(&idLib) < 0) {
		return;
	}

	// Prepare stored procedure call
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
	if(getInteger(&id) < 0) {
		return;
	}

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

static void sick_lib(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[5];

    char cf[16];
	int idLib;
	int idTurn;
	MYSQL_TIME *date;
	date = malloc(sizeof(MYSQL_TIME));
	char reason[45];

    printf("\nRichiesta malattina\n");

	// Get the required information
	printf("\nCF: ");
	if(getInput(16, cf, false) < 0) {
		return;
	}

    printf("Codice Biblioteca: ");
	if(getInteger(&idLib) < 0) {
		return;
	}
	
	printf("Codice Turno: ");
	if(getInteger(&idTurn) < 0) {
		return;
	}

    printf("Inserisci data(formato dd/mm/yyyy): ");
    if(getDate(date) < 0) {
		return;
	}

	printf("Motivo: ");
	if(getInput(45, reason, false) < 0) {
		return;
	}
	
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call richiesta_malattia(?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = cf;
	param[0].buffer_length = strlen(cf);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &idLib;
	param[1].buffer_length = sizeof(idLib);

	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &idTurn;
	param[2].buffer_length = sizeof(idTurn);

	param[3].buffer_type = MYSQL_TYPE_DATE;
	param[3].buffer = date;
	param[3].buffer_length = sizeof(*date);

	param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = reason;
	param[4].buffer_length = strlen(reason);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella richiesta di malattia.");
	} else {
        printf("Bibliotecario messo in malattia.\n");
    }

	mysql_stmt_close(prepared_stmt);
}

static void show_time(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];

    int id;

    printf("\n");
    printf("Orari biblioteca\n");

	// Get the required information
    printf("\nCodice Biblioteca: ");
	if(getInteger(&id) < 0) return;

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call orari_biblioteca(?)", conn)) {
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
		print_stmt_error (prepared_stmt, "Errore nella richiesta degli orari.");
	}

    char buff[50];
    sprintf(buff, "Orari biblioteca %d", id);
    dump_result_set(conn, prepared_stmt, buff);

	mysql_stmt_close(prepared_stmt);
}

static void avble_lib(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];

    MYSQL_TIME *date;
	MYSQL_TIME *start;
	MYSQL_TIME *end;

	date = malloc(sizeof(MYSQL_TIME));
    start = malloc(sizeof(MYSQL_TIME));
    end = malloc(sizeof(MYSQL_TIME));

    printf("\n");
    printf("Ricerca bibliotecari disponibili\n");

	// Get the required information
    printf("Inserisci data turno(formato dd/mm/yyyy): ");
	if(getDate(date) < 0) return;

	printf("Inserisci orario di apertura(formato xx:xx): ");
    if(getTime(start) < 0) return;

    printf("Inserisci orario di chiusura(formato xx:xx): ");
    if(getTime(end) < 0) return;
    getchar();

	// Prepare stored procedure call
	//TODO REMOVE WIP
	if(!setup_prepared_stmt(&prepared_stmt, "call bibliotecari_disponibiliWIP(?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_DATE;
	param[0].buffer = date;
	param[0].buffer_length = sizeof(*date);

	param[1].buffer_type = MYSQL_TYPE_TIME;
	param[1].buffer = start;
	param[1].buffer_length = sizeof(*start);

	param[2].buffer_type = MYSQL_TYPE_TIME;
	param[2].buffer = end;
	param[2].buffer_length = sizeof(*end);


	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella ricerca dei bibliotecari disponibili.");
	}

    char buff[100];
    sprintf(buff, "Bibliotecari disponibili il giorno %d/%d/%d %u:%u-%u:%u", date->day, date->month, date->year, start->hour, start->minute, end->hour, end->minute);
    dump_result_set(conn, prepared_stmt, buff);

	mysql_stmt_close(prepared_stmt);
}

static void check_coverage(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[3];

    int id;

	//`verifica_coperturaWIP` (IN var_biblioteca INT)

    printf("\nCopertura biblioteca\n");

	printf("Codice biblioteca: ");
	if(getInteger(&id) < 0) return;


	// Prepare stored procedure call
	//TODO REMOVE WIP
	if(!setup_prepared_stmt(&prepared_stmt, "call verifica_coperturaWIP(?)", conn)) {
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
		print_stmt_error (prepared_stmt, "Errore nella ricerca della copertura della biblioteca.");
	}

    char buff[100];
    sprintf(buff, "Copertura per la biblioteca %d", id);
    dump_result_set(conn, prepared_stmt, buff);

	mysql_stmt_close(prepared_stmt);
}

void handler(int sig) {
	printf("\nAnnullamento...\n");
}

void admin(MYSQL *conn, char *username) {

    printf("\033[2J\033[H");
    
    if(mysql_change_user(conn, "admin", "admin", "biblioteca")) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}

	struct sigaction act, sa;
 
	memset (&act, '\0', sizeof(act));
 
	act.sa_handler = handler;
	act.sa_flags = SA_INTERRUPT;

    printf("Connected as admin\n");
    while(1) {

        printf("%s-admin$ ", username);
        getInput(20, command, false);

		sigaction(SIGINT, &act, NULL);

        if(!strcmp(command, "quit")) {
            printf("Uscita...\n");
            return;
        } else if(!strcmp(command, "help")) {
            printf("*** Admin commands ***\n");
            printf("addadmin - per aggiungere un admin\n");
            printf("addlibrary- per aggiungere una biblioteca\n");
			printf("addlibrarian- per aggiungere un bibliotecario\n");
            printf("addtime - per aggiungere un orario di una biblioteca\n");
            printf("addturn - per aggiungere un turno\n");
            printf("setturn - per assegnare un turno per un mese\n");
            printf("setsturn - per aggiungere un turno singolo\n");
            printf("accepttransfer - per accettare un trasferimento\n");
            printf("rejecttransfer - per rifiutare una richiesta di transferimento\n");
            printf("showrequest - per vedere le richieste di trasferimento\n");
            printf("avblelib - per vedere bibliotecari disponibili\n");
            printf("dcopies - per dismettere le copie\n");
            printf("sick - per mettere in malattia un bibliotecario\n");
            printf("checkcoverage - per verificare la copertura per una biblioteca\n");
            printf("showtime - mostra l'orario di una biblioteca\n");
            printf("quit - per uscire dall'applicazione\n");
            printf("clear - per pulire lo schermo\n");
			printf("*********************\n");
        }  else if(!strcmp(command, "clear")) {
            printf("\033[2J\033[H");
        }  else if(!strcmp(command, "addadmin")) {
            add_admin(conn);
        }  else if(!strcmp(command, "addtime")) {
            add_time(conn);
        }  else if(!strcmp(command, "addlibrary")) {
            add_library(conn);
        }  else if(!strcmp(command, "addlibrarian")) {
            add_librarian(conn);
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
        }  else if(!strcmp(command, "setturn")) {
            set_turn(conn);
        }  else if(!strcmp(command, "setsturn")) {
            set_single_turn(conn);
        }  else if(!strcmp(command, "showtime")) {
            show_time(conn);
        }  else if(!strcmp(command, "avblelib")) {
            avble_lib(conn);
        }  else if(!strcmp(command, "sick")) {
            sick_lib(conn);
        }  else if(!strcmp(command, "checkcoverage")) {
            check_coverage(conn);
        } else {
            printf("comando %s non riconosciuto, digita help per aiuto\n", command);
        }

		sigaction(SIGINT, &sa, NULL);
    }
}
