void process_statistics() {
    for (int i = 1; i <= 3; i++) {
        char base_path[256];
        sprintf(base_path, "DATA/Scenario%d", i);
        
        // Processing each type of file
        const char* types[] = {
            "uniform_integers",
            "uniform_real",
            "normal_integer",
            "normal_real",
            "truncated_normal_integer",
            "truncated_normal_real"
        };
        
        for (int j = 0; j < 6; j++) {
            char filename[512];
            sprintf(filename, "%s/%s.txt", base_path, types[j]);
            calculate_statistics_from_file(filename, N);  // N from the scenario
        }
    }
}

void calculate_statistics_from_file(const char* filename, int N) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s'\n", filename);
        return;
    }

    double* data = (double*)malloc(N * sizeof(double));
    if (!data) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return;
    }

    int count = 0;
    while (count < N && fscanf(file, "%lf", &data[count]) == 1) {
        count++;
    }

    double mean = calculate_mean(data, count);
    double std_dev = calculate_std_dev(data, count, mean);

    printf("%s:\n", filename);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n\n", std_dev);

    free(data);
    fclose(file);
}

double calculate_mean(double* data, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += data[i];
    }
    return sum / n;
}

double calculate_std_dev(double* data, int n, double mean) {
    double sum_squared_diff = 0.0;
    for (int i = 0; i < n; i++) {
        double diff = data[i] - mean;
        sum_squared_diff += diff * diff;
    }
    return sqrt(sum_squared_diff / (n - 1));
}

// After generating all the random numbers
//"process_statistics();"
