#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

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

    printf("Giorno pubblicazione: ");
    scanf("%ui", &day);
    printf("Mese pubblicazione: ");
    scanf("%ui", &month);
    printf("Anno pubblicazione: ");
    scanf("%ui", &year);

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

void librarian(MYSQL *conn, int library) {
    char command[20];
    int loop = 1;

    printf("\033[2J\033[H");
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
            printf("addbook - per aggiungere un libro\n");
            printf("4) Aggiungi Copia\n");
            printf("5) Aggiungi Utente\n");
            printf("6) Mostra Contatti Utente\n");
            printf("7) Mostra Copie Disponibili\n");
            printf("8) Mostra Copie Disponibili Esterne\n");
            printf("9) Mostra Copie Prestate\n");
            printf("mostra libri nel db????");
            printf("sposta copia")
            printf("r) Richiedi Prestito\n");
            printf("t) Richiedi Trasferimento\n");
            printf("quit - per uscire dall'applicazione\n");
            printf("clear - per pulire il terminale\n");
        } else if(!strcmp(command, "addbook")) {
            add_book(conn);
        } else if(!strcmp(command, "clear")) {
            printf("\033[2J\033[H");
        } else {
            printf("comando non riconosciuto, digita help per aiuto\n");
        }
    }
}
