#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[])
{
    FILE *fp = fopen("time.txt", "r");
    FILE *output = fopen("output.txt", "w+a");
    if (!fp) {
        printf("ERROR opening input file orig.txt\n");
        exit(0);
    }
    int total_method = atoi(argv[1]);
    int runtime = atoi(argv[2]);

    // 95 Confidence interval
    double time_data[runtime];
    double mean;
    double var;
    double min;
    double max;
    double result;
    int count;

    for (int i = 0; i < total_method; i++) {
        time_data[runtime];
        mean = 0.0;
        var = 0.0;
        min = 0.0;
        max = 0.0;
        result = 0.0;
        count = 0;

        char method[30] = "";
        char tmp[30] = "";
        fscanf(fp, "%s", method);
        for (int k = 0 , pos = 0; k < strlen(method); k++, pos++) {
            if (method[k] == '_') {
                tmp[pos++] = '\\';
                tmp[pos++] = '\\';
                tmp[pos] = '_';
            } else
                tmp[pos] = method[k];
        }

        for (int j = 0; j < runtime; j++) {
            fscanf(fp, "%lf", &time_data[j]);
        }

        for (int i = 0; i < runtime; i++)
            mean += time_data[i];
        mean /= (double)runtime;

        for (int i = 0; i < runtime; i++)
            var += (time_data[i] - mean) * (time_data[i] - mean); //faster than pow
        var = sqrt(var / (double)runtime);

        min = mean - var * 2;
        max = mean + var * 2;

        for (int i = 0; i < runtime; i++) {
            if (time_data[i] >= min && time_data[i] <= max) {
                result += time_data[i];
                count++;
            }
        }
        fprintf(output, "%s %lf\n", tmp , result / (double)count);
    }
    fclose(fp);
    fclose(output);
    return 0;
}
