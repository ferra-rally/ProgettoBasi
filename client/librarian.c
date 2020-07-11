#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

int lib = 0;

static void add_book(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[5];

	char isbn[20];
	char title[45];
	char author[45];
    char edithor[45];
    MYSQL_TIME  date;

    printf("\033[2J\033[H");
    printf("Aggiungi libro\n");

	// Get the required information
	printf("\nISBN: ");
	getInput(20, isbn, false);
	printf("Titolo: ");
	getInput(45, title, false);
	printf("Autore: ");
	getInput(45, author, false);
    printf("Editore: ");
	getInput(45, edithor, false);
    // TODO considerare situazioni di errore

    unsigned int day, month, year;

    printf("Inserisci data di pubblicazione(formato dd/mm/yyyy): ");
    scanf("%d/%d/%d",&day,&month,&year);
    getchar();

    date.day = day;
    date.month = month;
    date.year = year;
 
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_libro(?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = isbn;
	param[0].buffer_length = strlen(isbn);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = title;
	param[1].buffer_length = strlen(title);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = author;
	param[2].buffer_length = strlen(author);

    param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = edithor;
	param[3].buffer_length = strlen(edithor);

    param[4].buffer_type = MYSQL_TYPE_DATE;
	param[4].buffer = &date;
	param[4].buffer_length = sizeof(date);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nell'aggiunta del libro.");
	} else {
		printf("Libro aggiunto correttamente\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_copy(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[5];

	int id;
	char isbn[20];
    char scaf[10];
    char rip[10];

    printf("\033[2J\033[H");
    printf("Aggiungi copia\n");

	// Get the required information
    printf("\nCodice: ");
	getInteger(&id);
	printf("ISBN: ");
	getInput(20, isbn, false);
	printf("Scaffale: ");
	getInput(10, scaf, false);
	printf("Ripiano: ");
	getInput(10, rip, false);
 
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_copia(?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = isbn;
	param[1].buffer_length = strlen(isbn);

	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &lib;
	param[2].buffer_length = sizeof(lib);

    param[3].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[3].buffer = scaf;
	param[3].buffer_length = strlen(scaf);

    param[4].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[4].buffer = rip;
	param[4].buffer_length = strlen(rip);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nell'aggiunta della copia.");
	} else {
		printf("Copia aggiunta correttamente\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void add_user(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[5];
	
    char cf[16];
	char name[45];
    char surname[45];
    int type;
    char contact[45];

    printf("\033[2J\033[H");
    printf("Aggiungi utente\n");

	// Get the required information
    printf("\nCF: ");
	getInput(16, cf, false);
	printf("Nome: ");
	getInput(45, name, false);
	printf("Cognome: ");
	getInput(45, surname, false);

	printf("*** Contatto preferito ***\n");
    printf("1 - per e-mail\n");
    printf("2 - per telefono\n");
    printf("3 - per cellulare\n");
	printf("Selezionare tipo: ");
    getInteger(&type);

    if(type > 3 || type < 1) {
        finish_with_error(conn, "Tipo invalido, uscita...");
    }
    printf("Inserire contatto: ");
    getInput(45, contact, false);
 
	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call aggiungi_utente(?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

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
	param[3].buffer = contact;
	param[3].buffer_length = strlen(contact);

    param[4].buffer_type = MYSQL_TYPE_LONG;
	param[4].buffer = &type;
	param[4].buffer_length = sizeof(type);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nell'aggiunta dell'utente.");
	} else {
		printf("Utente aggiunto correttamente\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void show_books(MYSQL *conn) {
    MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];

	char query[45];

    printf("\033[2J\033[H");
    printf("Inserisci ricerca, premi invio per mostrare tutto\n");

	// Get the required information
	printf("\nQuery: ");
	getInput(20, query, false);
    // TODO considerare situazioni di errore

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call trova_libri(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = query;
	param[0].buffer_length = strlen(query);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella ricerca di libri.");
	}

	dump_result_set(conn, prepared_stmt, "Lista libri nel sistema");

	mysql_stmt_close(prepared_stmt);
}

static void show_copies(MYSQL *conn) {
    MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	char query[45];

    printf("\033[2J\033[H");
    printf("Inserisci ricerca, premi invio per mostrare tutto\n");

	// Get the required information
	printf("\nQuery: ");
	getInput(45, query, false);
    // TODO considerare situazioni di errore

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call copie_disponibili_biblioteca(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

    param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &lib;
	param[0].buffer_length = sizeof(lib);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = query;
	param[1].buffer_length = strlen(query);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella ricerca delle copie.");
	}

	dump_result_set(conn, prepared_stmt, "Lista Copie nella biblioteca");

	mysql_stmt_close(prepared_stmt);
}

void librarian(MYSQL *conn, int library) {
    char command[20];
    int loop = 1;

    printf("\033[2J\033[H");
    
    if(mysql_change_user(conn, "bibliotecario", "bibliotecario", "biblioteca")) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}
    
    lib = library;
    printf("Connesso alla biblioteca %d\n", library);
    printf("Inserisci un comando, digita help per aiuto\n");

    while(loop) {
        printf("bibliotecario: ");
        getInput(20, command, false);

        if(!strcmp(command, "quit")) {
            loop = 0;
            printf("Uscita...");
            return;
        } else if(!strcmp(command, "help")) {
            printf("1) Presta Copia\n");
            printf("2) Restituisci Copia\n");
            printf("addbook - aggiunge un libro\n");
            printf("addcopy- aggiunge copia di un libro\n");
            printf("adduser - aggiunge utente\n");
            printf("addcontact - aggiunge un contatto ad un utenteWIIWIWIWIWP\n");
            printf("6) Mostra Contatti Utente\n");
            printf("avble - mostra copia disponibile nella biblioteca attuale\n");
            printf("8) Mostra Copie Disponibili Esterne\n");
            printf("9) Mostra Copie Prestate\n");
            printf("showbooks - mostra libri nel sistema a partire dal Titolo\n");
            printf("sposta copia");
            printf("r) Richiedi Prestito\n");
            printf("t) Richiedi Trasferimento\n");
            printf("quit - per uscire dall'applicazione\n");
            printf("clear - per pulire il terminale\n");
        } else if(!strcmp(command, "addbook")) {
            add_book(conn);
        } else if(!strcmp(command, "addcopy")) {
            add_copy(conn);
        } else if(!strcmp(command, "adduser")) {
            add_user(conn);
        } else if(!strcmp(command, "avble")) {
            show_copies(conn);
        } else if(!strcmp(command, "clear")) {
            printf("\033[2J\033[H");
        } else if(!strcmp(command, "showbooks")) {
            show_books(conn);
        } else {
            printf("comando %s non riconosciuto, digita help per aiuto\n", command);
        }
    }
}
