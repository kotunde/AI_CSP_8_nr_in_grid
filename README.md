# AI_CSP_8_nr_in_grid

### Feladatleírás
A feladat a használt módszer számát a parancssori argumentumokon keresztül veszi be.
- 1 : nyers backtracking
- 2 : backtracking + MVR + forward checking
- 3 : backtracking + MVR + AC-3

Eredetileg a feladat 8 számra szól egy 3x4-es rácsban elhelyezve, de a feladat méretét a bemeneti fájl alapján lehet növelni.
A rácsot és a szomszédos cellákat (vízszintes, függőleges, átlós) gráfként kezelem, és a fájl tartama:

1 csomópontok_száma élek_száma

2 első_él_x első_él_y

3 második_él_x második_él_y

.

.

.


A feladat szabályai:

Helyezzük el a 1, 2, 3, 4, 5, 6, 7, 8 számokat a gráf 8 csomópontjára (rácsban) úgy, hogy a számsorozatban szomszédos számok ne
kerüljenek szomszédos csomópontokra (pozíciókra). Például a gráf két csomópontjának nem lehet 3 illetve 4 az értéke, ha a két 
csomópont szomszédos. 

### Eredmények 
A bemeneti fájl alapján az eredmények e következőek:

- `./a.out 1`

  Ertekadasok szama: 1404

- `./a.out 2`

  Ertekadasok szama: 110


- `./a.out 3`

  Ertekadasok szama: 56
