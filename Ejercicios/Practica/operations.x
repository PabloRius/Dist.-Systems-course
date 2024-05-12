
struct operation_data {
    string username<>;
    string operation<>;
    string datetime<>;
};

program OPERATIONS_PROG {
    version OPERATIONS_VERS {
        void PRINT_OPERATION(operation_data) = 1;
    } = 1;
} = 0x20000001;
