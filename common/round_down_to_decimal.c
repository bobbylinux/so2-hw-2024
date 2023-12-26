/**
 * Questa funzione serve per arrotondare i double a due cifre dopo la virgola.
 * @param number
 * @param decimalPlaces
 * @return double
 */
double round_down_to_decimal(double number, int decimalPlaces) {
    double multiplier = 1;
    for (int i = 0; i < decimalPlaces; i++) {
        multiplier *= 10;
    }

    return ((int) (number * multiplier)) / multiplier;
}

