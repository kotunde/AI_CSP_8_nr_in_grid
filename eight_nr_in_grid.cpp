#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
#include <queue>
#include <string.h>
#include <math.h>

typedef struct node
{
    int fokszam;
    int * szomszedok;
    int ertek;
}node;

typedef struct el
{
    int u;
    int v;
}el;

int ertekadasok_szama = 0;
int* mrv_arr;

int mycmp(const void* a, const void* b)
{
    return (*(int*)a-*(int*)b);
}
int backtracking_felosztas(node*, int, int);
int rekurziv_backtracking_felosztas(node*, int, int*, int);
int rekurziv_backtracking_forwardchecking_felosztas(node*,int, int, int,int**);
void forward_checking(node*, int, int, int**, int);
int rekurziv_backtracking_ac3_felosztas(node* ,int, int,int, int**);
int ac3(node*, int, int, int** );
int remove_inconsistent_values(el,int,int **);
int values_are_consistent(int, int);
void domeniumbol_kizar_felhasznaltat(int**, int, int, int, int);
void rendez_valtozokat_legalis_ertek_szerint(node*,int,int* ,int** );
int kenyszer_teszt(node*, int, int*, int,int);
int sarok_cella(int,int,int,int);
int** allocate_mtrx(int n, int m);
void print_mtrx(int ** ,int n);
int** copy_mtrx(int** , int n);
void free_mtrx(int** ,int n);

int main(int argc, char * argv[])
{
    int cs;
    int e;
    int i,j,k,l,m;
    el* el_tomb;
    FILE * fin;

    int method = atoi(argv[1]);
	
    fin = fopen("input.txt","r");
    if(!fin)
    {
        printf("Bemeneti fajl hiba !");
        return -1;
    }

    fscanf(fin,"%i %i",&cs,&e);
    printf("%i %i\n",cs,e);

    el_tomb = (el*)calloc(e+1,sizeof(el));
    if(!el_tomb)
    {
        printf("El tomb helyfoglalasi hiba !");
        return -1;
    }

    for (i=1; i<e+1; ++i)
    {
        fscanf(fin,"%i %i",&el_tomb[i].u,&el_tomb[i].v);
        printf ("%i --> %i\n",el_tomb[i].u,el_tomb[i].v);
    }
    printf("\n");


    node* szomsz_list=(node*)calloc(cs+1,sizeof(node));
    if (!szomsz_list)
    {
        printf("Szomszedsagi lista helyfoglalasi hiba !");
        return -1;
    }

    for (i=1; i<e+1; ++i)
    {
        ++szomsz_list[el_tomb[i].u].fokszam;
        ++szomsz_list[el_tomb[i].v].fokszam;
    }

    for (int i=1; i<cs+1; ++i)
    {
        szomsz_list[i].szomszedok=(int*)calloc(szomsz_list[i].fokszam+1,sizeof(int));
        if (!szomsz_list[i].szomszedok)
        {
            printf("Kiszomszedok helyfoglalasi hiba az %i-dik csucsnak!",i);
            return -1;
        }
        szomsz_list[i].ertek = -1;
    }

    for (i=1; i<cs+1; ++i)
    {
        int ind1=0;
        for (j=1; j<e+1;++j)
        {
            if (el_tomb[j].u==i)
            {
                ++ind1;
                szomsz_list[i].szomszedok[ind1]=el_tomb[j].v;

            }
            else if (el_tomb[j].v==i)
            {
                ++ind1;
                szomsz_list[i].szomszedok[ind1]=el_tomb[j].u;
            }
        }
    }
	
    for (i=1; i<cs+1;++i)
    {
        qsort(szomsz_list[i].szomszedok,szomsz_list[i].fokszam+1,sizeof(int),mycmp);
    }

    printf("\n");
    for (i=1; i<cs+1; ++i)
    {
        printf("%i. csucs:\n",i);
        printf("Foszmszama: %i\n",szomsz_list[i].fokszam);
        printf("Szomszedok: ");
        for (j=1; j< szomsz_list[i].fokszam+1; ++j)
        {
            printf("%i ",szomsz_list[i].szomszedok[j]);
        }
        
        printf("\nertek:  %i\n",szomsz_list[i].ertek);
        printf("\n");
    }

    ///*********************************************************************************************************
    // Backtracking
    backtracking_felosztas(szomsz_list,cs,method);
    ///*********************************************************************************************************
    for(i = 0; i < cs+1; ++i)
    {
        free(szomsz_list[i].szomszedok);
    }

    free(szomsz_list);

    free(el_tomb);

    fclose(fin);

    return 0;
}

int backtracking_felosztas(node* szomsz_list,int cs, int modszer)
{
    int* felhasznalva = (int*)calloc(cs+1,sizeof(int));
    mrv_arr = (int*)calloc(cs+1,sizeof(int));

    int megoldas;
    if (modszer == 1)
    {
        megoldas = rekurziv_backtracking_felosztas(szomsz_list,cs,felhasznalva,1);
        free(felhasznalva);
    }
    else if (modszer == 2)
    {
        int** forwardchecking_available = allocate_mtrx(cs+1, cs+1);

        for (int i=1; i<(cs+1);++i)
        {
            for (int j=1; j<(cs+1); ++j)
            {
                forwardchecking_available[i][j] = 1;
            }
        }
        rendez_valtozokat_legalis_ertek_szerint(szomsz_list,cs,mrv_arr,forwardchecking_available);
        int ures_valtozok_szama = 8;
        megoldas = rekurziv_backtracking_forwardchecking_felosztas(szomsz_list,cs,ures_valtozok_szama,mrv_arr[1],forwardchecking_available);
        free_mtrx(forwardchecking_available,cs+1);
    }
    else if (modszer == 3)
    {
        int** ac3_available = allocate_mtrx(cs+1,cs+1);

        for (int i=1; i<(cs+1);++i)
        {
            for (int j=1; j<(cs+1); ++j)
            {
                ac3_available[i][j] = 1;
            }
        }
        int ures_valtozok_szama = 8;
        rendez_valtozokat_legalis_ertek_szerint(szomsz_list,cs,mrv_arr,ac3_available);
        megoldas = rekurziv_backtracking_ac3_felosztas(szomsz_list,cs,ures_valtozok_szama,mrv_arr[1],ac3_available);
        free_mtrx(ac3_available,cs+1);
    }

    if (!megoldas)
    {
        printf("Nincs megoldas\n");
        return false;
    }
    else
    {
        printf("\nMegoldas:\n");
        for (int i=1; i<cs+1; ++i)
        {
            printf("%i ",szomsz_list[i].ertek);
        }
        printf("\nErtekadasok szama: %i\n",ertekadasok_szama);
        return true;
    }
}

int rekurziv_backtracking_felosztas(node* szomsz_list, int cs, int* felhasznalva, int pozicio)
{
    printf("\n\n");
    for (int i=1; i<cs+1; ++i)
    {
        printf("%i ",felhasznalva[i]);
    }

    printf("\n");
    for (int i=1; i<cs+1; ++i)
    {
        printf("%i ",szomsz_list[i].ertek);
    }
    printf("\n");

    if(pozicio == cs+1)
    {
        return true;
    }
    else
    {
        for (int szam=1; szam<cs+1; ++szam)
        {
            ertekadasok_szama++;
            //printf("szam: %i,   felhasznalva: %i;\n",szam, felhasznalva[szam]);
            if(kenyszer_teszt(szomsz_list, cs, felhasznalva, pozicio, szam))
            {
                szomsz_list[pozicio].ertek = szam;
                felhasznalva[szam] = 1;

                int eredmeny = rekurziv_backtracking_felosztas(szomsz_list,cs,felhasznalva,pozicio+1);
                if (eredmeny == 1)
                {
                    return true;
                }
                else
                {
                    felhasznalva[szam] = 0;
                    szomsz_list[pozicio].ertek = -1;
                }

            }
        }
        return 0;
    }
}

int rekurziv_backtracking_forwardchecking_felosztas(node* szomsz_list,int cs, int ures_valtozok_szama,int pozicio,int ** forwardchecking_available)
{
    if( ures_valtozok_szama == 0)
    {
        return true;
    }
    else
    {
        for (int szam = 1; szam < cs + 1; ++szam)
        {
            int** forwardchecking_available_copy = copy_mtrx(forwardchecking_available,cs+1);
            //printf("szam: %i,   felhasznalva: %i;\n",szam, felhasznalva[szam]);
            if (!forwardchecking_available_copy[pozicio][szam])
            {
                printf("\nElutasitva: pozicio %i, szam %i\n",pozicio,szam);
            }
            else
            {
                ertekadasok_szama++;
            }
            if (forwardchecking_available_copy[pozicio][szam])
            {
                printf("Ertekadas:  pozicio %i, szam %i\n",pozicio,szam);
                szomsz_list[pozicio].ertek = szam;

                int muvelet_inverz = 0;
                domeniumbol_kizar_felhasznaltat(forwardchecking_available_copy,cs,pozicio,szam,muvelet_inverz);
                forward_checking(szomsz_list,pozicio,szam,forwardchecking_available_copy,muvelet_inverz);

                rendez_valtozokat_legalis_ertek_szerint(szomsz_list,cs,mrv_arr,forwardchecking_available_copy);
                int eredmeny = rekurziv_backtracking_forwardchecking_felosztas(szomsz_list, cs, ures_valtozok_szama -1,mrv_arr[1],forwardchecking_available_copy);
                free_mtrx(forwardchecking_available_copy,cs+1);

                if (eredmeny == 1)
                {
                    return true;
                }
                else
                {
                    szomsz_list[pozicio].ertek = -1;
                }

            }
        }
        return 0;
    }
}

void forward_checking(node* szomsz_list, int pozicio, int szam, int **forwardchecking_available, int muvelet_inverz)
{
    if(muvelet_inverz)
    {
        for (int i = 1; i < szomsz_list[pozicio].fokszam +1; ++i)
        {
            forwardchecking_available[szomsz_list[pozicio].szomszedok[i]][szam] = 1;
            if (szam > 1)
            {
                forwardchecking_available[szomsz_list[pozicio].szomszedok[i]][szam-1] = 1;
            }
            if (szam < 8)
            {
                forwardchecking_available[szomsz_list[pozicio].szomszedok[i]][szam+1] = 1;
            }
        }
    }
    else
    {
        for (int i = 1; i < szomsz_list[pozicio].fokszam + 1; ++i)
        {
            forwardchecking_available[szomsz_list[pozicio].szomszedok[i]][szam] = 0;
            if (szam > 1)
            {
                forwardchecking_available[szomsz_list[pozicio].szomszedok[i]][szam-1] = 0;
            }
            if (szam < 8)
            {
                forwardchecking_available[szomsz_list[pozicio].szomszedok[i]][szam+1] = 0;
            }
        }
    }
}

int rekurziv_backtracking_ac3_felosztas(node* szomsz_list,int cs, int ures_valtozok_szama,int pozicio,int** ac3_available)
{
    if(ures_valtozok_szama == 0)
    {
        return true;
    }
    else
    {
        for (int szam = 1; szam < cs + 1; ++szam)
        {
            int** ac3_available_copy = copy_mtrx(ac3_available,cs+1);
            //printf("szam: %i,   felhasznalva: %i;\n",szam, felhasznalva[szam]);
            if (!ac3_available_copy[pozicio][szam])
            {
                printf("\nElutasitva: pozicio %i, szam %i\n",pozicio,szam);
            }
            else
            {
                ertekadasok_szama++;
            }
            if (ac3_available_copy[pozicio][szam])
            {
                printf("Ertekadas:  pozicio %i, szam %i\n",pozicio,szam);
                szomsz_list[pozicio].ertek = szam;

                int muvelet_inverz = 0;
                domeniumbol_kizar_felhasznaltat(ac3_available_copy,cs,pozicio,szam,muvelet_inverz);

                int arcs_are_consistent = ac3(szomsz_list,cs,pozicio,ac3_available_copy);
                //print_mtrx(ac3_available_copy,cs+1);

                if (arcs_are_consistent)
                {
                    rendez_valtozokat_legalis_ertek_szerint(szomsz_list,cs,mrv_arr,ac3_available_copy);
                    int eredmeny = rekurziv_backtracking_ac3_felosztas(szomsz_list, cs, ures_valtozok_szama-1,mrv_arr[1],ac3_available_copy);
                    free_mtrx(ac3_available_copy,cs+1);
                    if (eredmeny == 1)
                    {

                        return true;
                    }
                    else
                    {
                        szomsz_list[pozicio].ertek = -1;
                    }
                }
                else
                {
                    szomsz_list[pozicio].ertek = -1;
                    free_mtrx(ac3_available_copy,cs+1);
                    //return 0;
                }

            }
        }
        return 0;
    }
}

int ac3(node* szomsz_list, int cs, int pozicio, int** ac3_available)
{
    std::queue<el> queue_of_arcs;

    for (int i=0; i<szomsz_list[pozicio].fokszam+1; ++i)
    {
        if (szomsz_list[szomsz_list[pozicio].szomszedok[i]].ertek == -1)
        {
            el uj_el;
            uj_el.u = szomsz_list[pozicio].szomszedok[i];
            uj_el.v = pozicio;
            queue_of_arcs.push(uj_el);
        }
    }

    while(!queue_of_arcs.empty())
    {
        el elso = queue_of_arcs.front();
        queue_of_arcs.pop();
        int result = remove_inconsistent_values(elso,cs,ac3_available);
        
        int values_were_removed = 1;
        int no_left_values = -1;
        
        if (result == values_were_removed)
        {
            for (int i=1; i<szomsz_list[elso.u].fokszam+1; ++i)
            {
                if (szomsz_list[szomsz_list[elso.u].szomszedok[i]].ertek == -1)
                {
                    el uj_el;
                    uj_el.u = szomsz_list[elso.u].szomszedok[i];
                    uj_el.v = elso.u;
                    queue_of_arcs.push(uj_el);
                }
            }
        }
        else if (result == no_left_values)
        {
            return 0;
        }
            
    }
    return 1;
}

int remove_inconsistent_values(el aktualis_el,int cs,int ** ac3_available)
{
    int not_available = 0;
    int nr_of_removed_values = 0;
    int tested_values = 0;
    int no_value_to_satisfy_constraints = 1;
    for (int i=1; i<cs+1; ++i)
    {
        if(ac3_available[aktualis_el.u][i] == 1)
        {
            no_value_to_satisfy_constraints = 1;
            tested_values++;

            for (int j=1; j<cs+1; ++j)
            {
                if(ac3_available[aktualis_el.v][j] == 1)
                {
                    if (values_are_consistent(i,j))
                    {
                        no_value_to_satisfy_constraints = 0;
                    }
                }
            }
            if (no_value_to_satisfy_constraints)
            {
                nr_of_removed_values++;
                ac3_available[aktualis_el.u][i] = not_available;
            }
        }
    }

    int consistent_values_left = tested_values - nr_of_removed_values;
    if (consistent_values_left == 0)
    {
        return -1;
    }
    else if (nr_of_removed_values > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int values_are_consistent(int x,int y)
{
    if (x == y || x == (y-1) || x == (y+1))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void domeniumbol_kizar_felhasznaltat(int** domain_table,int cs,int pozicio,int szam,int muvelet_inverz)
{
    if (muvelet_inverz)
    {
        int nem_felhasznalva = 1;
        for (int i=szam+1; i<cs+1; ++i)
        {
            domain_table[pozicio][i] = nem_felhasznalva;
        }
        for (int i=pozicio+1; i < cs+1; ++i)
        {
            domain_table[i][szam] = nem_felhasznalva;
        }
    }
    else
    {
        int felhasznalva = 0;
        for (int i=1; i<cs+1; ++i)
        {
            if (domain_table[pozicio][i] == 1 && i != szam)
            {
                domain_table[pozicio][i] = felhasznalva;
            }
        }
        for (int i=pozicio+1; i < cs+1; ++i)
        {
            if (domain_table[i][szam] == 1 && i != pozicio)
            {
                domain_table[i][szam] = felhasznalva;
            }
        }
    }
}

void rendez_valtozokat_legalis_ertek_szerint(node* szomsz_list,int cs,int* mrv_arr,int** domain_table)
{
    int* domenium_ertekszam =(int*)calloc(cs+1,sizeof(int));
    for (int i=1; i<cs+1; ++i)
    {
        for(int j=1; j<cs+1; ++j)
        {
            if(domain_table[i][j] == 1)
            {
                domenium_ertekszam[i] ++;
            }
        }
        if (domenium_ertekszam[i] == 1 && szomsz_list[i].ertek != -1)
        {
            int nincs_legalis_erteke = INT32_MAX;
            domenium_ertekszam[i] = nincs_legalis_erteke;
        }
    }

    for (int i=1; i<cs+1; ++i)
    {
        mrv_arr[i] = i;
    }

    int min_value,min_pos;
    for (int i=1; i<cs; ++i)
    {
        min_pos = i;
        for (int j=i+1; j<cs+1; ++j)
        {
            if(domenium_ertekszam[mrv_arr[j]] < domenium_ertekszam[mrv_arr[min_pos]])
            {
                min_pos = j;
            }
        }
        int temp = mrv_arr[i];
        mrv_arr[i] = mrv_arr[min_pos];
        mrv_arr[min_pos] = temp;
    }
}

int kenyszer_teszt(node* szomsz_list, int cs, int* felhasznalva, int pozicio, int szam)
{
    int kenyszerek_kielegitve = 1;
    if (felhasznalva[szam])
    {
        kenyszerek_kielegitve = !kenyszerek_kielegitve;
        return kenyszerek_kielegitve;
    }

    for(int i=1; i<szomsz_list[pozicio].fokszam+1; ++i)
    {
        if( (szomsz_list[szomsz_list[pozicio].szomszedok[i]].ertek !=-1) &&
            (szomsz_list[szomsz_list[pozicio].szomszedok[i]].ertek == szam - 1 ||
             szomsz_list[szomsz_list[pozicio].szomszedok[i]].ertek == szam + 1))
        {
            kenyszerek_kielegitve = !kenyszerek_kielegitve;
            return kenyszerek_kielegitve;
        }
    }
    return kenyszerek_kielegitve;
}

int sarok_cella(int i, int j, int n, int m)
{
    if ((i==0 && j==0) || (i==n && j==0) || (i==0 && j==m) || (i==n && j==m))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int** allocate_mtrx(int n, int m)
{
    int **mtrx = (int**)calloc(n, sizeof(int*));
    if (mtrx == NULL)
    {
        printf("Allocation failed");
        return 0;
    }
    for (int i=0; i<n; ++i)
    {
        mtrx[i] = (int*)calloc(m, sizeof(int));
        if (mtrx[i] == NULL)
        {
            printf("Allocation failed");
            return 0;
        }
    }
    return mtrx;
}

void free_mtrx(int** mtrx,int n)
{
    for(int i = 0; i < n; ++i)
    {
        free(mtrx[i]);
    }

    free(mtrx);
}

void print_mtrx(int ** mtrx,int n)
{
    printf("- - - - - - - - - \n\n");
    for (int i=1; i<n; ++i)
    {
        for (int j=1; j<n; ++j)
        {
            printf("%2d ",mtrx[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int** copy_mtrx(int** mtrxFrom, int n)
{
    int **mtrxTo = allocate_mtrx(n,n);

    for (int i=0; i<n; ++i)
    {
        for (int j=0; j<n; ++j)
        {
            mtrxTo[i][j] = mtrxFrom[i][j];
        }
    }
    return mtrxTo;
}
