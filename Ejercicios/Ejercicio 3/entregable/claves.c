#include "servicio.h"
#include "claves.h"

#define INIT 0
#define GET 1
#define SET 2
#define MODIFY 3
#define DELETE 4
#define EXIST 5

int get_host(char *dest)
{
    const char *host;
    host = getenv("IP_TUPLAS");
    if (host == NULL)
    {
        printf("Variable IP_TUPLAS no definida\n");
        return -1;
    }
    strcpy(dest, host);
    return 0;
}

int init() {
    char HOST[50];
    if (get_host(HOST) < 0)
    {
        printf("Error leyendo el valor de host\n");
        return -1;
    }

	CLIENT *clnt;
	enum clnt_stat retval_1;
	struct args_out result;
	struct args_in message;
    message.op = INIT;

    #ifndef	DEBUG
        clnt = clnt_create (HOST, SERVICIO, SERVICIOVER, "tcp");
        if (clnt == NULL) {
            clnt_pcreateerror (HOST);
            exit (1);
        }
    #endif	/* DEBUG */

    retval_1 = servicios_1(message, &result, clnt);
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
    }

    #ifndef	DEBUG
        clnt_destroy (clnt);
    #endif	 /* DEBUG */

    return result.code;
}

int set_value(int key, char *value1, int N_value2, double *V_value2) {
    char HOST[50];
    if (get_host(HOST) < 0)
    {
        printf("Error leyendo el valor de host\n");
        return -1;
    }

	CLIENT *clnt;
	enum clnt_stat retval_1;
	struct args_out result;
	struct args_in message;

    message.op = SET;
    message.key = key;
    strcpy(message.cadena, value1);
    message.N = N_value2;
    memcpy(message.vector, V_value2, N_value2 * sizeof(double));

    #ifndef	DEBUG
        clnt = clnt_create (HOST, SERVICIO, SERVICIOVER, "tcp");
        if (clnt == NULL) {
            clnt_pcreateerror (HOST);
            exit (1);
        }
    #endif	/* DEBUG */

    retval_1 = servicios_1(message, &result, clnt);
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
    }

    #ifndef	DEBUG
        clnt_destroy (clnt);
    #endif	 /* DEBUG */

    return result.code;
}

int get_value(int key, char *value1, int *N_value2, double *V_value2) {
    char HOST[50];
    if (get_host(HOST) < 0)
    {
        printf("Error leyendo el valor de host\n");
        return -1;
    }

	CLIENT *clnt;
	enum clnt_stat retval_1;
	struct args_out result;
	struct args_in message;

    message.op = GET;
    message.key = key;

    #ifndef	DEBUG
        clnt = clnt_create (HOST, SERVICIO, SERVICIOVER, "tcp");
        if (clnt == NULL) {
            clnt_pcreateerror (HOST);
            exit (1);
        }
    #endif	/* DEBUG */

    retval_1 = servicios_1(message, &result, clnt);
    strcpy(value1, result.cadena);
    *N_value2 = result.N;
    memcpy(V_value2, result.vector, result.N * sizeof(double));
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
    }

    #ifndef	DEBUG
        clnt_destroy (clnt);
    #endif	 /* DEBUG */

    return result.code;
}

int modify_value(int key, char *value1, int N_value2, double *V_value2) {
    char HOST[50];
    if (get_host(HOST) < 0)
    {
        printf("Error leyendo el valor de host\n");
        return -1;
    }

	CLIENT *clnt;
	enum clnt_stat retval_1;
	struct args_out result;
	struct args_in message;

    message.op = MODIFY;
    message.key = key;
    strcpy(message.cadena, value1);
    message.N = N_value2;
    memcpy(message.vector, V_value2, N_value2 * sizeof(double));

    #ifndef	DEBUG
        clnt = clnt_create (HOST, SERVICIO, SERVICIOVER, "tcp");
        if (clnt == NULL) {
            clnt_pcreateerror (HOST);
            exit (1);
        }
    #endif	/* DEBUG */

    retval_1 = servicios_1(message, &result, clnt);
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
    }

    #ifndef	DEBUG
        clnt_destroy (clnt);
    #endif	 /* DEBUG */

    return result.code;
}

int delete_key(int key) {
    char HOST[50];
    if (get_host(HOST) < 0)
    {
        printf("Error leyendo el valor de host\n");
        return -1;
    }

	CLIENT *clnt;
	enum clnt_stat retval_1;
	struct args_out result;
	struct args_in message;

    message.op = DELETE;
    message.key = key;

    #ifndef	DEBUG
        clnt = clnt_create (HOST, SERVICIO, SERVICIOVER, "tcp");
        if (clnt == NULL) {
            clnt_pcreateerror (HOST);
            exit (1);
        }
    #endif	/* DEBUG */

    retval_1 = servicios_1(message, &result, clnt);
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
    }

    #ifndef	DEBUG
        clnt_destroy (clnt);
    #endif	 /* DEBUG */

    return result.code;
}

int exist(int key) {
    char HOST[50];
    if (get_host(HOST) < 0)
    {
        printf("Error leyendo el valor de host\n");
        return -1;
    }

	CLIENT *clnt;
	enum clnt_stat retval_1;
	struct args_out result;
	struct args_in message;

    message.op = EXIST;
    message.key = key;

    #ifndef	DEBUG
        clnt = clnt_create (HOST, SERVICIO, SERVICIOVER, "tcp");
        if (clnt == NULL) {
            clnt_pcreateerror (HOST);
            exit (1);
        }
    #endif	/* DEBUG */

    retval_1 = servicios_1(message, &result, clnt);
    if (retval_1 != RPC_SUCCESS) {
        clnt_perror (clnt, "call failed");
    }

    #ifndef	DEBUG
        clnt_destroy (clnt);
    #endif	 /* DEBUG */

    return result.code;
}
