#include <ios>
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

/*
 * Baralho Alho
 * Isadora so sabe embaralhar dessa maneira:
 * carta i -> P[i] (1-based index)
 *   e.g. Carta: 3 -> sera colocada em Baralho[3]
 * suponha P = [3,2,4,1] -> primeira carta vai para P[3] no baralho
 *                          segunda carta vai para P[2] no baralho
 *                          terceira vai para P[4] no baralho
 *                          quarta vai para P[1]
 * INPUT: 4 linhas
 * 1 linha: um inteiro N
 * 2 linha: N inteiros -> configuracao inicial do baralho.
 * 3 linha: N inteiros -> configuracao final desejada
 * 4 linha: N inteiros -> para onde cada carta do baralho inicial vai 
 *                                  (baseado no indice de cada carta)
 * OUTPUT: um inteiro sendo o numero de vezes que o embaralhamento deve acontecer
 *  para chegar na configuracao final, se nao for possivel, imprimir "IMPOSSIVEL",
 *  se o numero for maior que 10^9 imprimir "DEMAIS".
 *
 *  O problema pode ser decomposto em três fases fundamentais:
 *  Decomposição em Ciclos: A permutação P é um grafo funcional.
 *  Como cada índice aponta para exatamente um outro, ela forma ciclos isolados.
 *  Se uma carta está no ciclo C de tamanho L, ela só vai transitar pelas L posições desse ciclo.
 *  Busca do Shift (r) e Período (m): Para cada ciclo, pegamos os valores iniciais (A) e desejados (B).
 *  Precisamos descobrir quantos embaralhamentos (r) alinham A com B.
 *  Como pode haver cartas repetidas, o padrão pode se repetir antes do ciclo completo.
 *  O "período" (m) dita de quantos em quantos passos essa configuração volta a se repetir nesse ciclo.
 *  Merge com CRT Generalizado: Cada ciclo nos dá uma congruência: k == r_i mod(m_i).
 *  Juntamos todas elas passo a passo.
 *  Se em algum momento duas equações entrarem em contradição, a resposta é IMPOSSIVEL.
 */

constexpr i32 MAX_R = 1e9;

static inline vector<i32> concat(vector<i32>& a, vector<i32>& b) {
    vector<i32> result;
    result.reserve(a.size() + b.size());

    result.insert(result.end(), a.begin(), a.end());
    result.insert(result.end(), b.begin(), b.end());

    return result;
}

static inline i32 find_first_occurrence(vector<i32> a, vector<i32> b) {
    auto it = search(b.begin(), b.end(), a.begin(), a.end());

    if (it == b.end()) {
        return -1;
    }

    // Distancia entre o inicio de b e o iterator é o nosso indice
    return distance(b.begin(), it);
}

static inline i32 find_minimum_period(vector<i32> a) {
    i32 L = a.size();
    if (L <= 1) return L;

    // Criamos um vector sem o primeiro elemento (pulamos a primeira ocorrencia obvia)
    vector<i32> aa = a;
    aa.insert(aa.end(), a.begin(), a.end());

    // Agora podemos buscar o vector [a] dentro de [aa] porém pulando o index 0
    auto it = search(aa.begin() + 1, aa.end(), a.begin(), a.end());

    return distance(aa.begin(), it);
}

static inline i64 extended_gcd(i64 a, i64 b, i64& x, i64& y) {
    if (a == 0) {
        x = 0; y = 1;
        return b;
    }

    i64 x1, y1;

    i64 gcd = extended_gcd(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return gcd;
}
                          

int main(void) {
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    i32 N; cin >> N;
    vector<i32> A(N, 0);
    vector<i32> B(N, 0);
    vector<i32> P(N, 0);

    for (i32 i = 0; i < N; ++i) {
        i32 k; cin >> k;
        A[i] = k;
    } 

    for (i32 i = 0; i < N; ++i) {
        i32 k; cin >> k;
        B[i] = k;
    }

    for (i32 i = 0; i < N; ++i) {
        i32 k; cin >> k;
        P[i] = k - 1; // -1 por conta de ser 1-based no problema
    }
    
    vector<u8> visited(N, 0);
    i64 M = 1;
    i64 R = 0;

    for (i32 i = 0; i < N; ++i) {
        if (visited[i]) continue;

        vector<i32> cicle_a, cicle_b;
        i32 current = i;
        while (!visited[current]) {
            visited[current] = true;
            cicle_a.emplace_back(A[current]);
            cicle_b.emplace_back(B[current]);
            current = P[current];
        }

        i32 r = find_first_occurrence(cicle_a, concat(cicle_b, cicle_b));

        if (r < 0) {
            cout << "IMPOSSIVEL\n";
            return 0;
        }

        // O periodo m é a diferença de saltos para a mesma string se repetir
        i32 m = find_minimum_period(cicle_a);

        // merge das equações, CRT generalizado
        i64 x, y;
        i64 g = extended_gcd(M, m, x, y);

        i64 delta = r - R;
        if ((delta % g) != 0) {
            cout << "IMPOSSIVEL\n";
            return 0;
        }

        i64 reduced_mod = m / g;

        i64 t = (delta / g) % reduced_mod;

        t = (t * x) % reduced_mod;
        t = (t + reduced_mod) % reduced_mod;

        R = R + t * M;
        M = M * reduced_mod; // Isso é o LCM(M, m)
        R = R % M;
    }

    if (R > MAX_R) {
        cout << "DEMAIS\n";
    } else {
        cout << R;
    }

    return 0;
}
