#ifndef POTTSSTATE_H_INCLUDED
#define POTTSSTATE_H_INCLUDED

typedef struct
{
    int q;
    int M;
    unsigned int *state;

}PottsState;


typedef struct{
    PottsState* x;
    unsigned int n;
    double beta;
}PottsMC;


//PottsState
PottsState* Potts_create(unsigned q0,unsigned size0,double beta);
void Potts_delete(PottsState* X);
double deltaenergy(const PottsState*x,unsigned k,unsigned newstate);
double energy(const PottsState*);
void change(PottsState*X,unsigned k,unsigned newstate);
unsigned Potts_state_statistic(PottsState*X,unsigned state);

//PottsMC
PottsMC * PottsMC_allocate(unsigned q,unsigned size0,double beta);
unsigned PottsMC_update(PottsMC *X);
void PottsMC_free(PottsMC *);

#endif // POTTSSTATE_H_INCLUDED
