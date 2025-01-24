#include <stdio.h>// For standard I/O operations
#include <math.h>// For mathematical functions
#include <stdlib.h>// For memory allocation and exit() function
#include <time.h>// For seeding the random number generator
#include <string.h>// For string manipulation
#include <sys/stat.h> // For mkdir()
#include <errno.h>

#define M_PI 3.14159265358979323846

#define frand() (rand() / (double)RAND_MAX) // Uniform random number in [0, 1)
#define nrand() (sqrt(-2 * log(frand())) * cos(2 * M_PI * frand())) // Normal
// random number
// Number of bins for histograms
#define HISTOGRAM_BINS 50// Function prototypes
void generate_random_numbers_to_file(const char* filename, int type, double m, double M, double mu, double sigma, int N);
void calculate_statistics_from_file(const char* filename, int N);
void generate_histogram_from_file(const char* input_filename, const char*
output_filename, int bins, double min, double max);
int generate_uniform_integer(double m, double M);
double generate_uniform_real(double m, double M);
double generate_normal_real(double mu, double sigma);
int generate_normal_integer(double mu, double sigma);
double generate_truncated_normal_real(double m, double M, double mu, double
sigma);
int generate_truncated_normal_integer(double m, double M, double mu, double
sigma);
double calculate_mean(double* data, int n);
double calculate_std_dev(double* data, int n, double mean);
int create_directory(const char* path);
int main() {
srand(time(NULL)); // Seed the random number generator
// Define scenarios: {mu, sigma, m, M, N}
double scenarios[3][5] = {
{5, 1, 1, 8, 20},
{pow(2, 10), pow(2, 8), 1, 2000, 200000},
{pow(2, 12), 1.3 * pow(2, 10), 1, 8100, 2000000}
};
// Paths for directories and files
char* subfolders[3] = {"DATA/Scenario1", "DATA/Scenario2", "DATA/Scenario3"};
char* histogram_folder = "HISTOGRAM";

// Loop through each scenario to process random numbers
for (int i = 0; i < 3; i++) {
double mu = scenarios[i][0];
double sigma = scenarios[i][1];
double m = scenarios[i][2];
double M = scenarios[i][3];
int N = (int)scenarios[i][4];
// Process random number generation, statistics, and histograms// Utilize appropriate function calls (detailed below) for each step.
}
return 0;
}
         
int create_directory(const char* path)
{
    int result = mkdir(path, 0777);//0777 allows full read write access
    if ( result  == -1) {
        //errno is a spec var that tells why last operation failed
        // EEXIST means that the dir already exists
        if (errno == EEXIST) {
            return 0;
        } else {
            fprintf(stderr, "Error creating directory '%s': %s\n", path, strerror(errno));//strerror(errno) converts error code into human readable message
            return -1;
        }
    }
}


void generate_random_numbers_to_file(const char* filename, int type, double m,
double M, double mu, double sigma, int N)
{
    FILE* file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s' for writing.\n", filename);
        return;
    }

    // Generate random numbers based on the type
    for (int i = 0; i < N; i++) {
        double random_number = 0;

        switch (type) {
            case 1: 
                random_number = generate_uniform_integer(m, M);
                break;
            case 2: 
                random_number = generate_uniform_real(m, M);
                break;
            case 3: 
                random_number = generate_normal_real(mu, sigma);
                break;
            case 4: 
                random_number = generate_normal_integer(mu, sigma);
                break;
            case 5: 
                random_number = generate_truncated_normal_real(m, M, mu, sigma);
                break;
            case 6: 
                random_number = generate_truncated_normal_integer(m, M, mu, sigma);
                break;
            default:
                fprintf(stderr, "Error: Invalid random number generator type '%d'.\n", type);
                fclose(file);
                return;
        }

        fprintf(file, "%.6f\n", random_number);
    }

    fclose(file);
} 

void calculate_statistics_from_file(const char* filename, int N)
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file '%s' for reading.\n", filename);
        return;
    }

    double sum = 0.0;          // To store the sum of numbers
    double sum_of_squares = 0.0; // To store the sum of squares of numbers
    int count = 0;             // To track how many numbers have been read
    double number;

    // Read numbers from the file
    while (count < N && fscanf(file, "%lf", &number) == 1) {
        sum += number;                    // Add to the sum
        sum_of_squares += number * number; // Add to the sum of squares
        count++;
    }

    // Check if we read the expected number of numbers
    if (count < N) {
        fprintf(stderr, "Warning: Expected %d numbers but only found %d in '%s'.\n", N, count, filename);
    }

    // Calculate mean
    double mean = sum / count;

    // Calculate standard deviation
    double variance = (sum_of_squares - (sum * sum) / count) / count;
    double std_dev = sqrt(variance);

    // Print results
    printf("File: %s\n", filename);
    printf("Mean: %.6f\n", mean);
    printf("Standard Deviation: %.6f\n", std_dev);

    // Close the file
    fclose(file);

}



void generate_histogram_from_file(const char* input_filename, const char*
output_filename, int bins, double min, double max)
{
    FILE* input_file = fopen(input_filename, "r");
    if (!input_file) {
        fprintf(stderr, "Error: Could not open input file '%s'.\n", input_filename);
        return;
    }

    // Open the output file for writing
    FILE* output_file = fopen(output_filename, "w");
    if (!output_file) {
        fprintf(stderr, "Error: Could not open output file '%s'.\n", output_filename);
        fclose(input_file);
        return;
    }

    // Initialize histogram bins
    int* histogram = (int*)calloc(bins, sizeof(int));
    if (!histogram) {
        fprintf(stderr, "Error: Memory allocation failed for histogram bins.\n");
        fclose(input_file);
        fclose(output_file);
        return;
    }

    // Calculate bin width
    double bin_width = (max - min) / bins;

    // Read numbers from the input file and populate the histogram
    double number;
    while (fscanf(input_file, "%lf", &number) == 1) {
        if (number >= min && number < max) {
            int bin_index = (int)((number - min) / bin_width);
            if (bin_index >= bins) bin_index = bins - 1; // Edge case for max
            histogram[bin_index]++;
        }
    }

    // Write histogram to the output file
    for (int i = 0; i < bins; i++) {
        double bin_start = min + i * bin_width;
        double bin_end = bin_start + bin_width;
        fprintf(output_file, "%.2f - %.2f: %d\n", bin_start, bin_end, histogram[i]);
    }

    // Clean up
    free(histogram);
    fclose(input_file);
    fclose(output_file);
}



int generate_uniform_integer(double m, double M)
{
    int lower_bound = (int)m;
    int upper_bound = (int)M;

    return lower_bound + rand() % (upper_bound - lower_bound + 1);
}



double generate_uniform_real(double m, double M)
{
        return m + (frand() * (M - m)); 
}



int generate_normal_integer(double mu, double sigma)
{
    double normal_value = mu + (sigma * nrand());  
    return (int)round(normal_value);  
}


double generate_normal_real(double mu, double sigma)
{
        return mu + (sigma * nrand()); 
}



int generate_truncated_normal_integer(double m, double M, double mu, double
sigma)
{
     int value;
    do {
        value = generate_normal_integer(mu, sigma);  
    } while (value < m || value > M);  
    return value;
}


double generate_truncated_normal_real(double m, double M, double mu, double
sigma)
{
    double value;
    do {
        value = generate_normal_real(mu, sigma);  
    } while (value < m || value > M);  
    return value;
}



double calculate_mean(double* data, int n)
{
    double sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += data[i];  
    }
    return sum / n;  
}



double calculate_std_dev(double* data, int n, double mean)
{
    double sum_squared_diff = 0.0;
    for (int i = 0; i < n; i++) {
        sum_squared_diff += pow(data[i] - mean, 2);  
    }
    return sqrt(sum_squared_diff / (n - 1));  
