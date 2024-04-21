struct set_value_args {
    int key;
    string value1<256>;
    int N_value2;
    double V_value2<32>;
};

struct get_value_args {
    int key;
};

struct get_value_result {
    int status;
    string value1<256>;
    int N_value2;
    double V_value2<32>;
};

struct modify_value_args {
    int key;
    string value1<256>;
    int N_value2;
    double V_value2<32>;
};

struct delete_key_args {
    int key;
};

struct exist_args {
    int key;
};

program TUPLE_PROG {
    version TUPLE_VERS {
        int INIT(void) = 1;
        int SET_VALUE(set_value_args) = 2;
        get_value_result GET_VALUE(get_value_args) = 3;
        int MODIFY_VALUE(modify_value_args) = 5;
        int DELETE_KEY(delete_key_args) = 6;
        int EXIST(exist_args) = 7;
    } = 1;
} = 0x20000001;
