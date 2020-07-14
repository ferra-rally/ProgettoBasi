#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"

static MYSQL *conn;

typedef enum {
	ADMIN = 1,
	LIBRARIAN,
	FAILED_LOGIN
} role_t;

//TODO refactor library to int
static role_t attempt_login(MYSQL *conn, char *username, char *password, int *library) {
	MYSQL_STMT *login_procedure;
	
	MYSQL_BIND param[4]; // Used both for input and output
	int role = 0;

	if(!setup_prepared_stmt(&login_procedure, "call login(?, ?, ?, ?)", conn)) {
		print_stmt_error(login_procedure, "Unable to initialize login statement\n");
		//printf("Unable to initialize login statement\n");
		goto err2;
	}

	// Prepare parameters
	memset(param, 0, sizeof(param));
	
	param[0].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param[0].buffer = username;
	param[0].buffer_length = strlen(username);

	param[1].buffer_type = MYSQL_TYPE_VAR_STRING; // IN
	param[1].buffer = password;
	param[1].buffer_length = strlen(password);

	param[2].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[2].buffer = &role;
	param[2].buffer_length = sizeof(role);

	param[3].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[3].buffer = library;
	param[3].buffer_length = sizeof(library);

	if (mysql_stmt_bind_param(login_procedure, param) != 0) { // Note _param
		print_stmt_error(login_procedure, "Could not bind parameters for login");
		goto err;
	}

	// Run procedure
	if (mysql_stmt_execute(login_procedure) != 0) {
		print_stmt_error(login_procedure, "Could not execute login procedure");
		goto err;
	}

	// Prepare output parameters
	memset(param, 0, sizeof(param));
	param[0].buffer_type = MYSQL_TYPE_LONG; // OUT
	param[0].buffer = &role;
	param[0].buffer_length = sizeof(role);

	// If librarian store the current library
	param[1].buffer_type = MYSQL_TYPE_LONG;
	param[1].buffer = library;
	param[1].buffer_length = sizeof(library);

	if(mysql_stmt_bind_result(login_procedure, param)) {
		print_stmt_error(login_procedure, "Could not retrieve output parameter");
		goto err;
	}
	
	// Retrieve output parameter
	if(mysql_stmt_fetch(login_procedure)) {
		print_stmt_error(login_procedure, "Could not buffer results");
		goto err;
	}

	mysql_stmt_close(login_procedure);
	return role;

    err:
	mysql_stmt_close(login_procedure);
    err2:
	return FAILED_LOGIN;
}

int main(void){
	char username[128];
	char password[128];
	int library = -1;

	conn = mysql_init (NULL);
	if (conn == NULL) {
		fprintf (stderr, "mysql_init() failed (probably out of memory)\n");
		exit(EXIT_FAILURE);
	}

	if (mysql_real_connect(conn, "localhost", "login", "login", "biblioteca", 3306, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS) == NULL) {
		fprintf (stderr, "mysql_real_connect() failed\n");
		mysql_close (conn);
		exit(1);
	}

	printf("Username: ");
	getInput(128, username, false);
	printf("Password: ");
	getInput(128, password, true);

	int role = attempt_login(conn, username, password, &library);

	switch(role) {
		case 1:
			printf("Connecting as admin...WIP\n");
			break;
		case 2:
			//TODO remove hardcoded 1
			/*if(library > 0) {*/
				librarian(conn, 1);
				/*
			} else {
				printf("No turn selected\n");
			}*/
			break;
		default:
			printf("Invalid login\n");
	}

}
