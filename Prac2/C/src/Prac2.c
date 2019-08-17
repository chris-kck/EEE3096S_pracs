#include "Prac2.h"


extern double data [SAMPLE_COUNT];
extern double carrier[SAMPLE_COUNT];

double result [SAMPLE_COUNT];

int main(int argc, char**argv){
    printf("Running Unthreaded Test\n");
    printf("Precision sizeof %d\n", sizeof(float));

    tic(); // start the timer
    for (int i = 0;i<SAMPLE_COUNT;i++ ){
        result[i] = data[i] * carrier[i];
    }
    double t = toc();
    printf("Time: %lf ms\n",t/1e-3);
    printf("Result 6 is %.80F\n",result[6]);
    printf("End Unthreaded Test\n");
    return 0;
}
