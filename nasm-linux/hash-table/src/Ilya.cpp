int ER_ = 0;

#define CATCH_ERROR(func, type) \
    if ((ER_ = func) != 0) { \
        return (type)ER_; \
    }

enum VectorError {
    NO_ERROR,
    REALLOC_FAILED, 
    CANARY_RUINED
};

enum CodeError {
    VECTOR_ERROR
};

CodeError func(int a, int b) {
    return a;
}

VectorError funcc(int a, int b) {
    CATCH_ERROR(func(a, b), VectorError);
    return NO_ERROR;
}

int main() {
  CATCH_ERROR(func(a, b), int);
  CATCH_ERROR(funcc(a, b), int);
}