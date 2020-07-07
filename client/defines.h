#include <stdbool.h>
#include <mysql.h>

struct configuration {
	char *host;
	char *db_username;
	char *db_password;
	unsigned int port;
	char *database;

	char username[128];
	char password[128];
};

extern void print_stmt_error (MYSQL_STMT *stmt, char *message);
extern bool setup_prepared_stmt(MYSQL_STMT **stmt, char *statement, MYSQL *conn);
extern void print_error (MYSQL *conn, char *message);
extern char *getInput(unsigned int lung, char *stringa, bool hide);
void librarian(MYSQL *conn, int library);
char multiChoice(char *domanda, char choices[], int num);
extern void dump_result_set(MYSQL *conn, MYSQL_STMT *stmt, char *title);
extern void finish_with_error(MYSQL *conn, char *message);
extern void finish_with_stmt_error(MYSQL *conn, MYSQL_STMT *stmt, char *message, bool close_stmt);