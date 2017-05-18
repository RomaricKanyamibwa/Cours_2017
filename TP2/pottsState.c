# include <gsl/gsl_rng.h> // pour les générateurs
# include <gsl/gsl_randist.h> // pour les lois usuelles
# include <stdio.h> //pour l’affichage
# include <time.h> //pour lire l’heure
# include "pottsState.h"
#include <math.h>




PottsState* Potts_create(unsigned int q0,unsigned size0,double beta)
{
    PottsState*X=malloc(sizeof(PottsState));
    unsigned int *s=malloc(sizeof(int)*q0);
    int i=0;
    if(beta>0)
    {
        s[0]=size0;
        printf("s[%d]=%d\n",0,s[0]);
        for(i=1;i<q0;i++)
            {
                s[i]=0;
                printf("s[%d]=%d\n",i,s[i]);
            }
    }
    else
    {
        for(i=0;i<q0-1;i++)
        {
            s[i]=size0/q0;
            printf("s[%d]=%d\n",i,s[i]);
        }
        s[q0-1]=size0-i*size0/q0;
        printf("s[%d]=%d\n",i,s[i]);

    }
    X->M=size0;
    X->q=q0;
    X->state=s;
    return X;
}


PottsMC * PottsMC_allocate(unsigned q,unsigned size0,double beta)
{
    PottsMC* P=malloc(sizeof(PottsMC));
    PottsState* X=Potts_create(q,size0,beta);
    P->beta=beta;
    P->n=0;
    P->x=X;
    return P;
}

void Potts_delete(PottsState* X)
{
    free(X->state);
    free(X);
}

void PottsMC_free(PottsMC* X)
{
    Potts_delete(X->x);
    free(X);
}

unsigned int PottsMC_update(PottsMC*X,gsl_rng * G)
{
    //choix de l'individu
    int indiv=gsl_rng_uniform_int (G,X->x->M);
    int k=0,s=0;
    do
    {
        s+=X->x->state[k];
        k++;

    }while(s<indiv);
    k--;
    // choix de la nouvelle couleur
    int l=gsl_rng_uniform_int (G,X->x->q);
    int Nk=X->x->state[k],Nl=X->x->state[l];
    double pi_yx=exp(-X->beta*(Nl-Nk+1));
    printf("Nk=%d\n",Nk);
    printf("Nl=%d\n",Nl);
    //Acceptation ou refus de la configuration
    if(pi_yx>1)
    {
        X->x->state[k]--;
        X->x->state[l]++;
        X->n++;
        return 1;
    }else
    {
        double u=gsl_rng_uniform(G);
        if(pi_yx>=u)
        {
            X->x->state[k]--;
            X->x->state[l]++;
            X->n++;
            return 1;
        }
        else
            return 0;
    }
}


int main(void)
{
    gsl_rng * G= gsl_rng_alloc(gsl_rng_mt19937);
    gsl_rng_set(G,time(NULL));

    unsigned int M=50;
    double beta=1.0/50.0;
    unsigned int q=3;

    PottsMC *X=PottsMC_allocate(q,M,beta);
    FILE *fp;
    char* filename="PottsMC.txt";
    fp = fopen(filename, "w+");
    int i,j,n=10000;
    for(i=0;i<n;i++)
    {
        if(PottsMC_update(X,G))
        {
            fprintf(fp, "%d ",X->n);
            for(j=0;j<X->x->q;j++)
            {
                fprintf(fp,"%d ",X->x->state[j]);
            }
            fprintf(fp,"\n");
        }
    }


    PottsMC_free(X);
    gsl_rng_free(G);

    return 1;
}
