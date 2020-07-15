#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

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

static void lend(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[4];
	
    int id;
	char cf[16];
    int length;

    printf("\033[2J\033[H");
    printf("Presta Copia\n");

	// Get the required information
    printf("\nCodice: ");
	getInteger(&id);
	printf("CF: ");
	getInput(16, cf, false);
    printf("Durata prestito [1/2/3]: ");
    getInteger(&length);

    if(length > 3 || length < 1) {
        finish_with_error(conn, "Tipo invalido, uscita...");
    }

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call prestito(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = cf;
	param[1].buffer_length = strlen(cf);

	param[2].buffer_type = MYSQL_TYPE_LONG;
	param[2].buffer = &length;
	param[2].buffer_length = sizeof(length);

    param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = &lib;
	param[3].buffer_length = sizeof(lib);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nel prestito.");
	} else {
		printf("Libro prestato correttamente\n");
	}

	mysql_stmt_close(prepared_stmt);
}


static void return_copy(MYSQL *conn) {
	MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[5];
	
    int id;
	char scaf[45];
    char rip[45];
    double pen;
    int late;

    printf("\033[2J\033[H");
    printf("Restituisci copia\n");

	// Get the required information
    printf("\nCodice: ");
	getInteger(&id);
	printf("Scaffale: ");
	getInput(45, scaf, false);
    printf("Ripiano: ");
	getInput(45, rip, false);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call restituisci_copia(?, ?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG; //IN
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;   //IN
	param[1].buffer = scaf;
	param[1].buffer_length = strlen(scaf);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;   //IN
	param[2].buffer = rip;
	param[2].buffer_length = strlen(rip);

    param[3].buffer_type = MYSQL_TYPE_DOUBLE; //OUT
	param[3].buffer = &pen;
	param[3].buffer_length = sizeof(pen);

    param[4].buffer_type = MYSQL_TYPE_LONG; //OUT
	param[4].buffer = &late;
	param[4].buffer_length = sizeof(late);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella restituzione.");
	}

    // Prepare output parameters
	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_DOUBLE; // OUT
	param[0].buffer = &pen;
	param[0].buffer_length = sizeof(pen);

	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &late;
	param[1].buffer_length = sizeof(late);

    if(mysql_stmt_bind_result(prepared_stmt, param)) {
		print_stmt_error(prepared_stmt, "Could not retrieve output parameter");
        goto out;
	}
	
	// Retrieve output parameter
	if(mysql_stmt_fetch(prepared_stmt)) {
		print_stmt_error(prepared_stmt, "Could not buffer results");
        goto out;
	}

    printf("Libro restituito correttamente\n");
    printf("PENALE: %5.1f\n", pen);
    printf("RITARDO: %d\n", late);

    out:
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

static void show_copies_external(MYSQL *conn) {
    MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	char query[45];

    printf("\033[2J\033[H");
    printf("Inserisci ricerca, premi invio per mostrare tutto\n");

	// Get the required information
	printf("\nQuery: ");
	getInput(45, query, false);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call copie_disponibili(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = query;
	param[0].buffer_length = strlen(query);

    param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &lib;
	param[1].buffer_length = sizeof(lib);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella ricerca delle copie.");
	}

	dump_result_set(conn, prepared_stmt, "Lista copie disponibili in altre biblioteche");

	mysql_stmt_close(prepared_stmt);
}

static void show_lend(MYSQL *conn) {
    MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];

    printf("\033[2J\033[H");

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call copie_prestate(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

    param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &lib;
	param[0].buffer_length = sizeof(lib);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella ricerca delle copie prestate.");
	}

	dump_result_set(conn, prepared_stmt, "Lista Copie prestate nella biblioteca");

	mysql_stmt_close(prepared_stmt);
}

static void user_contact(MYSQL *conn) {
    MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[1];
    int status;

    char cf[16];

    printf("\033[2J\033[H");
    printf("Inserisci codice fiscale: ");
    getInput(16, cf, false);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call contatti_utente(?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

    param[0].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[0].buffer = cf;
	param[0].buffer_length = strlen(cf);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella ricerca dei contatti utente.");
	}

    do {
		dump_result_set(conn, prepared_stmt, "");
		status = mysql_stmt_next_result(prepared_stmt);
		if (status > 0)
			finish_with_stmt_error(conn, prepared_stmt, "Unexpected condition", true);
	} while (status == 0);

	mysql_stmt_close(prepared_stmt);
}

static void move(MYSQL *conn) {
    MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[4];

	int id;
	char scaf[10];
	char rip[10];

    printf("\033[2J\033[H");
	printf("Muovi copia in uno scaffale e ripiano\n");

	printf("Codice: ");
	getInteger(&id);
	printf("Scaffale: ");
	getInput(10, scaf, false);
	printf("Ripiano: ");
	getInput(10, rip, false);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call sposta(?, ?, ?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[1].buffer = scaf;
	param[1].buffer_length = strlen(scaf);

	param[2].buffer_type = MYSQL_TYPE_VAR_STRING;
	param[2].buffer = rip;
	param[2].buffer_length = sizeof(rip);

    param[3].buffer_type = MYSQL_TYPE_LONG;
	param[3].buffer = &lib;
	param[3].buffer_length = sizeof(lib);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nello spostare copie.");
	} else {
		printf("Copia %d spostata correttamente in %s - %s\n", id, scaf, rip);
	}

	mysql_stmt_close(prepared_stmt);
}

static void request_transfer(MYSQL *conn) {
    MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	int id;

    printf("\033[2J\033[H");
	printf("Richiedi trasferimento\n");

	printf("Codice: ");
	getInteger(&id);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call richiesta_trasferimento(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);

    param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &lib;
	param[1].buffer_length = sizeof(lib);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nella richiesta trasferimento.");
	} else {
		printf("Trasferimento richiesto\n");
	}

	mysql_stmt_close(prepared_stmt);
}

static void return_transfer(MYSQL *conn) {
    MYSQL_STMT *prepared_stmt;
	MYSQL_BIND param[2];

	int id;

    printf("\033[2J\033[H");
	printf("Richiedi trasferimento\n");

	printf("Codice: ");
	getInteger(&id);

	// Prepare stored procedure call
	if(!setup_prepared_stmt(&prepared_stmt, "call ritorna_trasferimento(?, ?)", conn)) {
		finish_with_stmt_error(conn, prepared_stmt, "Impossibile inizializzare lo statement.\n", false);
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].buffer_length = sizeof(id);

    param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = &lib;
	param[1].buffer_length = sizeof(lib);

	if (mysql_stmt_bind_param(prepared_stmt, param) != 0) {
		finish_with_stmt_error(conn, prepared_stmt, "Errore nel binding dei parametri.\n", true);
	}

	// Run procedure
	if (mysql_stmt_execute(prepared_stmt) != 0) {
		print_stmt_error (prepared_stmt, "Errore nel ritorno del trasferimento.");
	} else {
		printf("Copia restituita\n");
	}

	mysql_stmt_close(prepared_stmt);
}

void librarian(MYSQL *conn, int library) {
    char command[20];

    printf("\033[2J\033[H");

    if(mysql_change_user(conn, "bibliotecario", "bibliotecario", "biblioteca")) {
		fprintf(stderr, "mysql_change_user() failed\n");
		exit(EXIT_FAILURE);
	}
    
    lib = library;
    printf("Connesso alla biblioteca %d\n", library);
    printf("Inserisci un comando, digita help per aiuto\n\n");

    while(1) {
		
    	printf("bibliotecario$ ");
        getInput(20, command, false);

        if(!strcmp(command, "quit")) {
            printf("Uscita...\n");
            return;
        } else if(!strcmp(command, "help")) {
			printf("*** Comandi bibliotecario ***\n");
            printf("lend - presta la copia\n");
            printf("return - restituisci copia\n");
            printf("addbook - aggiunge un libro\n");
            printf("addcopy- aggiunge copia di un libro\n");
            printf("adduser - aggiunge utente\n");
            printf("addcontact - aggiunge un contatto ad un utente\n");
            printf("contact - mostra contatti di un utente\n");
            printf("avble - mostra copia disponibile nella biblioteca attuale\n");
            printf("eavble - mostra copie disponibili in altre biblioteche\n");
            printf("showlend - mostra copie prestate\n");
            printf("showbooks - mostra libri nel sistema a partire dal Titolo\n");
            printf("request - richiedi trasferimento\n");
            printf("rtransfer - ritorna trasferimento\n");
            printf("move - sposta copie in ripiani e scaffali\n");
            printf("quit - per uscire dall'applicazione\n");
            printf("clear - per pulire il terminale\n");
        } else if(!strcmp(command, "addbook")) {
            add_book(conn);
        } else if(!strcmp(command, "addcopy")) {
            add_copy(conn);
        } else if(!strcmp(command, "lend")) {
            lend(conn);
        } else if(!strcmp(command, "showlend")) {
            show_lend(conn);
        } else if(!strcmp(command, "move")) {
            move(conn);
        } else if(!strcmp(command, "contact")) {
            user_contact(conn);
        } else if(!strcmp(command, "return")) {
            return_copy(conn);
        } else if(!strcmp(command, "adduser")) {
            add_user(conn);
        } else if(!strcmp(command, "avble")) {
            show_copies(conn);
        } else if(!strcmp(command, "eavble")) {
            show_copies_external(conn);
        } else if(!strcmp(command, "request")) {
            request_transfer(conn);
        } else if(!strcmp(command, "rtransfer")) {
            return_transfer(conn);
        } else if(!strcmp(command, "clear")) {
            printf("\033[2J\033[H");
        } else if(!strcmp(command, "showbooks")) {
            show_books(conn);
        } else {
            printf("comando %s non riconosciuto, digita help per aiuto\n", command);
        }
    }
}