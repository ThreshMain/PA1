long long avg3(long long a, long long b, long long c) {
    long long reminder = a % 3 + b % 3 + c % 3;
    long long result = a / 3 + b / 3 + c / 3;
    result += reminder / 3;
    reminder %= 3;
    while (reminder < 0 && result > 0) {
        result--;
        reminder += 3;
    }
    while (reminder > 0 && result < 0) {
        result++;
        reminder -= 3;
    }
    return result;
}
