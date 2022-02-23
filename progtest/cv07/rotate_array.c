
int gcd(int a,int b){
    int tmp;
    while (b != 0)
    {
        tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

void rotateArray(int array[], int arrayLen, int rotateBy) {
    rotateBy %= arrayLen;
    if (rotateBy < 0) rotateBy += arrayLen;
    for (int start_index = 0; start_index < gcd(arrayLen,rotateBy); start_index++) {
        int index = start_index;
        int tmp = array[index];
        int current;
        do {
            index = (index + rotateBy) % arrayLen;
            index = index < 0 ? index + arrayLen : index;
            current = array[index];
            array[index] = tmp;
            tmp = current;
        } while (index != start_index);
    }
}
