struct args_in {
    int op;
    int key;
    char cadena[256];
    int N;
    double vector[32];
};

struct args_out {
    int code;
    char cadena[256];
    int N;
    double vector[32];
};

program SERVICIO {
    version SERVICIOVER {
        struct args_out SERVICIOS ( struct args_in ) = 1;
    } = 1;
} = 99;